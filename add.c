#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <openssl/sha.h>
#include <sys/stat.h>
#include <unistd.h>
#include "utils.h"

void store_blob(const char *hash, const char *data, size_t len)
{
    char dir_path[256], file_path[256];
    snprintf(dir_path, sizeof(dir_path), ".sert/objects/%.2s", hash);
    snprintf(file_path, sizeof(file_path), ".sert/objects/%.2s/%.38s", hash, hash + 2);

    mkdir(dir_path, 0700);
    FILE *f = fopen(file_path, "wb");
    if (!f)
    {
        perror("Error opening file");
        return;
    }
    fwrite(data, 1, len, f);
    fclose(f);
}

void addfile(const char *filename)
{
    size_t content_len;
    char *file_content = read_file(filename, &content_len);
    if (!file_content)
    {
        fprintf(stderr, "Failed to read file: %s\n", filename);
        return;
    }

    char header[64];
    snprintf(header, sizeof(header), "blob %zu", content_len);

    size_t full_len = sizeof(header) + 1 + content_len;

    char *blob_data = malloc(full_len);
    if (!blob_data)
    {
        perror("malloc failed");
        free(file_content);
        return;
    }

    memcpy(blob_data, header, sizeof(header));

    blob_data[sizeof(header)] = '\0';
    memcpy(blob_data + sizeof(header) + 1, file_content, content_len);

    char hash_str[SHA_DIGEST_LENGTH * 2 + 1];
    compute_hash(blob_data, full_len, hash_str);

    store_blob(hash_str, blob_data, full_len);

    printf("added %s as blob %s \n", filename, hash_str);

    FILE *index = fopen(".sert/index", "a");
    if (!index)
    {
        perror("There was error opening index file");
        return;
    }
    fprintf(index, "%s %s \n", filename, hash_str);

    free(file_content);
    free(blob_data);
}