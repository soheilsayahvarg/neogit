#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_commit(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("invalid input\n");
    }
    if (!strcmp(argv[2], "-m"))
    {
        if (strlen(argv[3]) > MAX_COMMIT_MESSAGE_LENGHT)
        {
            printf("max commit message is %d characters\n", MAX_COMMIT_MESSAGE_LENGHT);
            return 0;
        }
    }
    return 1;
}

int run_set_message(int argc, char *argv[])
{
    return 1;
}

int run_replace_message(int argc, char *argv[])
{
    return 1;
}

int run_remove_message(int argc, char *argv[])
{
    return 1;
}