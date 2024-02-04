#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_revert(int argc, char *argv[])
{
    if (argc == 2 || argc > 5)
    {
        printf("invalid input\n");
        return 0;
    }

    if (!strcmp(argv[2], "-n"))
    {
        if (argc == 3)
        {
            return 1;
        }
        if (argc == 4)
        {
            return 1;
        }

        printf("invalid input\n");
        return 0;
    }

    if (argc == 3)
    {
        if (!strncmp(argv[argc - 1], "commit ", strlen("commit ")))
        {
            return 1;
        }

        if (!strncmp(argv[argc - 1], "", strlen("commit ")))
        {
            return 1;
        }
    }

    if (argc == 5 && strcmp(argv[2], "-m") == 0)
    {
        if (!strncmp(argv[argc - 1], "commit ", strlen("commit ")))
        {
            return 1;
        }
    }

    printf("invalid input\n");
    return 0;
}
