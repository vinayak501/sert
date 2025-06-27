#include <stdio.h>
#include <string.h>
#include "init.h"
#include "add.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  sert init\n");
        fprintf(stderr, "  sert add <filename>\n");
        return 1;
    }

    if (strcmp(argv[1], "init") == 0)
    {
        init_repository();
    }
    else if (strcmp(argv[1], "add") == 0)
    {
        if (argc < 3)
        {
            fprintf(stderr, "Error: No filename given to add.\n");
            return 1;
        }

        // Loop over all files passed after "add"
        for (int i = 2; i < argc; ++i)
        {
            addfile(argv[i]);
        }
    }
    else if (strcmp(argv[1], "cat-object") == 0)
    {
        if (argc != 3)
        {
            fprintf(stderr, "Usage: sert cat-object <hash>\n");
            return 1;
        }
        cat_object(argv[2]);
    }
    else
    {
        fprintf(stderr, "Unknown command: %s\n", argv[1]);
        fprintf(stderr, "Available commands: init, add\n");
        return 1;
    }

    return 0;
}
