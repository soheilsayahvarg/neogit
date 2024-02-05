#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_pre_commit(int argc, char *argv[])
{
    if (argc == 2)
    {
        // pre-commit
        return 1;
    }

    if (argc == 3)
    {
        if (strcmp(argv[2], "-u"))
        {
            printf("invalid input\n");
            return 0;
        }
        // pre-commit -u
    }

    if (argc == 4 && strcmp(argv[2], "hooks") == 0 && strcmp(argv[3], "list") == 0)
    {
        return 1;
    }

    if (argc == 4 && strcmp(argv[2], "applied") == 0 && strcmp(argv[3], "hooks") == 0)
    {
        return 1;
    }

    if (!strcmp(argv[2], "-f"))
    {
        return 1;
    }

    if (argc != 5)
    {
        printf("invalid input\n");
        return 0;
    }

    if (argc == 5 && strcmp(argv[2], "add") == 0 && strcmp(argv[3], "hook") == 0)
    {
        return 1;
    }

    if (argc == 5 && strcmp(argv[2], "remove") == 0 && strcmp(argv[3], "hook") == 0)
    {
        return 1;
    }

    printf("invalid input\n");
    return 0;
}