#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_commit(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("invalid input\n");
        return 0;
    }

    if (!strcmp(argv[2], "-m"))
    {
        if (strlen(argv[3]) > MAX_COMMIT_MESSAGE_LENGHT)
        {
            printf("max commit message is %d characters\n", MAX_COMMIT_MESSAGE_LENGHT);
            return 0;
        }

        return creat_commit(argv[3]);
    }
    if (!strcmp(argv[2], "-s"))
    {
        char neogit_dir_address[MAX_ADDRESS_LENGHT];

        if (find_neogit_dir(neogit_dir_address) != 1)
        {
            printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
            return 0;
        }
        strcat(neogit_dir_address, "commits_shortcuts/");
        strcat(neogit_dir_address, argv[3]);
        FILE *shortcut_file = fopen(neogit_dir_address, "r");
        if (shortcut_file != NULL)
        {
            char shortcut_message[MAX_COMMIT_MESSAGE_LENGHT + 1];
            fgets(shortcut_message, sizeof(shortcut_message), shortcut_file);
            if (shortcut_message[strlen(shortcut_message) - 1] == '\n')
            {
                shortcut_message[strlen(shortcut_message) - 1] = '\0';
            }
            return creat_commit(shortcut_message);
        }
        printf("not found shortcut\n");
        return 0;
    }
    printf("invalid input\n");
    return 0;
}
// TODO
int creat_commit(char message[])
{
    printf("messeage : \"%s\"\n", message);
    int last_commit_id = 0;

    char neogit_dir_address[MAX_ADDRESS_LENGHT];

    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }
    return 1;
}

int run_set_message(int argc, char *argv[])
{
    if (argc != 6 || strcmp(argv[2], "-m") != 0 || strcmp(argv[4], "-s") != 0)
    {
        printf("invalid input\n");
        return 0;
    }
    if (strlen(argv[3]) > MAX_COMMIT_MESSAGE_LENGHT)
    {
        printf("max commit message is %d characters\n", MAX_COMMIT_MESSAGE_LENGHT);
        return 0;
    }

    char neogit_dir_address[MAX_ADDRESS_LENGHT];

    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }
    strcat(neogit_dir_address, "commits_shortcuts/");
    strcat(neogit_dir_address, argv[5]);
    FILE *shortcut_file = fopen(neogit_dir_address, "w");
    fprintf(shortcut_file, "%s\n", argv[3]);
    fclose(shortcut_file);

    printf("set \"%s\" shortcut to \"%s\"\n", argv[5], argv[3]);
    return 1;
}

int run_replace_message(int argc, char *argv[])
{
    char neogit_dir_address[MAX_ADDRESS_LENGHT];

    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }
    strcat(neogit_dir_address, "commits_shortcuts/");
    strcat(neogit_dir_address, argv[5]);
    FILE *shortcut_file = fopen(neogit_dir_address, "r");
    if (shortcut_file != NULL)
    {
        return run_set_message(argc, argv);
    }
    printf("not found shortcut\n");
    return 0;
}

int run_remove_message(int argc, char *argv[])
{
    if (argc != 4 || strcmp(argv[2], "-s") != 0)
    {
        printf("invalid input");
        return 0;
    }

    char neogit_dir_address[MAX_ADDRESS_LENGHT];

    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }
    strcat(neogit_dir_address, "commits_shortcuts/");
    strcat(neogit_dir_address, argv[3]);
    FILE *shortcut_file = fopen(neogit_dir_address, "r");
    if (shortcut_file != NULL)
    {
        char command[MAX_BASH_COMMAND];
        strcpy(command, "rm -f \"");
        strcat(command, neogit_dir_address);
        strcat(command, "\"");
        system(command);
        printf("remove \"%s\" shortcut\n", argv[3]);
        return 1;
    }
    printf("not found shortcut\n");
    return 0;
}