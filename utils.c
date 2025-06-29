#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <unistd.h>

char *read_file(const char *path, size_t *out_len)
{
    FILE *file = fopen(path, "rb");
    if (!file)
    {
        perror("Error While reading the file");
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    size_t len = ftell(file);
    rewind(file);

    char *buffer = malloc(len + 1);
    if (!buffer)
    {
        perror("malloc failed");
        fclose(file);
        return NULL;
    }
    size_t total_read = 0;
    size_t bytes_read;
    while (total_read < len)
    {
        bytes_read = fread(buffer + total_read, 1, len - total_read, file);
        if (bytes_read == 0)
        {
            if (feof(file))
            {
                break;
            }
            else if (ferror(file))
            {
                perror("Error reading file");
                free(buffer);
                fclose(file);
                return NULL;
            }
        }
        total_read += bytes_read;
    }

    buffer[total_read] = '\0';
    fclose(file);

    if (out_len)
    {
        *out_len = total_read;
    }

    return buffer;
}

void compute_hash(const char *data, size_t len, char *out_hash_str)
{
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1((unsigned char *)data, len, hash);
    for (int i = 0; i < SHA_DIGEST_LENGTH; ++i)
    {
        sprintf(out_hash_str + i * 2, "%02x", hash[i]);
    }
}

void cat_object(const char *hash)
{
    if (strlen(hash) != 40)
    {
        fprintf(stderr, "Invalid hash length. Expected 40 characters.\n");
        exit(1);
    }

    char path[256];
    snprintf(path, sizeof(path), ".sert/objects/%.2s/%s", hash, hash + 2);

    FILE *file = fopen(path, "rb");
    if (!file)
    {
        perror("Failed to open object file");
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);
    rewind(file);

    char *buffer = malloc(filesize + 1);
    fread(buffer, 1, filesize, file);
    fclose(file);
    buffer[filesize] = '\0';

    char *content = strchr(buffer, '\0');
    if (!content)
    {
        fprintf(stderr, "Invalid blob format (no null byte).\n");
        free(buffer);
        exit(1);
    }
    content++; // skip null byte

    size_t content_size = filesize - (content - buffer);
    fwrite(content, 1, content_size, stdout);
    printf("\n");
}