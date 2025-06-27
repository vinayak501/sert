#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>

void create_dir(const char *path)
{
    if (mkdir(path, 0700) == -1)
    {
        if (errno != EEXIST)
        {
            perror(path);
            exit(1);
        }
    }
}

void write_file(const char *path, const char *content)
{
    FILE *file = fopen(path, "w");
    if (!file)
    {
        perror(path);
        exit(1);
    }
    fprintf(file, "%s", content);
    fclose(file);
}

void init_repository()
{
    create_dir(".sert");

    create_dir(".sert/objects");
    create_dir(".sert/objects/info");
    create_dir(".sert/objects/pack");

    write_file(".sert/HEAD", "ref: refs/heads/master\n");

    write_file(".sert/config", "[core]\n\trepositoryformatversion = 0\n");

    write_file(".sert/index", "");

    create_dir(".sert/refs");
    create_dir(".sert/refs/heads");

    printf("Initialized empty SERT repository in .sert/\n");
}