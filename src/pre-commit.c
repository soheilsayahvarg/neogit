#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_pre_commit(int argc, char *argv[])
{

    char neogit_dir_address[MAX_ADDRESS_LENGTH];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }

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
        char hooks_list_address[MAX_ADDRESS_LENGTH];
        strcpy(hooks_list_address, neogit_dir_address);
        strcat(hooks_list_address, "pre-commit/hooks list");
        FILE *hooks_list_file = fopen(hooks_list_address, "r");

        char line_in_hooks_file[MAX_HOOKS_ID_LENGTH];
        while (fgets(line_in_hooks_file, sizeof(line_in_hooks_file), hooks_list_file))
        {
            printf("%s", line_in_hooks_file);
        }

        fclose(hooks_list_file);
        return 1;
    }

    if (argc == 4 && strcmp(argv[2], "applied") == 0 && strcmp(argv[3], "hooks") == 0)
    {
        char applied_hooks_address[MAX_ADDRESS_LENGTH];
        strcpy(applied_hooks_address, neogit_dir_address);
        strcat(applied_hooks_address, "pre-commit/applied hooks");
        FILE *applied_hooks_file = fopen(applied_hooks_address, "r");

        char line_in_hooks_file[MAX_HOOKS_ID_LENGTH];
        while (fgets(line_in_hooks_file, sizeof(line_in_hooks_file), applied_hooks_file))
        {
            printf("%s", line_in_hooks_file);
        }
        fclose(applied_hooks_file);
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
        return add_hook(argv[4]);
    }

    if (argc == 5 && strcmp(argv[2], "remove") == 0 && strcmp(argv[3], "hook") == 0)
    {
        return remove_hook(argv[4]);
    }

    printf("invalid input\n");
    return 0;
}

int add_hook(char hook_id[])
{

    char neogit_dir_address[MAX_ADDRESS_LENGTH];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }

    // check hook-id in hooks list
    char hooks_list_address[MAX_ADDRESS_LENGTH];
    strcpy(hooks_list_address, neogit_dir_address);
    strcat(hooks_list_address, "pre-commit/hooks list");
    FILE *hooks_list_file = fopen(hooks_list_address, "r");

    int find_hook_id_in_hooks_list = 0;
    char line_in_hooks_file[MAX_HOOKS_ID_LENGTH];
    while (fgets(line_in_hooks_file, sizeof(line_in_hooks_file), hooks_list_file))
    {
        if (line_in_hooks_file[strlen(line_in_hooks_file) - 1] == '\n')
        {
            line_in_hooks_file[strlen(line_in_hooks_file) - 1] = '\0';
        }
        if (!strcmp(line_in_hooks_file, hook_id))
        {
            find_hook_id_in_hooks_list = 1;
            break;
        }
    }
    fclose(hooks_list_file);

    if (!find_hook_id_in_hooks_list)
    {
        printf("invalid hook id\n");
        return 0;
    }

    // check hook-id in applied hooks
    char applied_hooks_address[MAX_ADDRESS_LENGTH];
    strcpy(applied_hooks_address, neogit_dir_address);
    strcat(applied_hooks_address, "pre-commit/applied hooks");
    FILE *applied_hooks_file = fopen(applied_hooks_address, "r");

    int find_hook_id_in_applied_hooks = 0;
    while (fgets(line_in_hooks_file, sizeof(line_in_hooks_file), applied_hooks_file))
    {
        if (line_in_hooks_file[strlen(line_in_hooks_file) - 1] == '\n')
        {
            line_in_hooks_file[strlen(line_in_hooks_file) - 1] = '\0';
        }
        if (!strcmp(line_in_hooks_file, hook_id))
        {
            find_hook_id_in_applied_hooks = 1;
            break;
        }
    }
    fclose(applied_hooks_file);

    if (find_hook_id_in_applied_hooks)
    {
        printf("hook_id is already in applied hooks\n");
        return 0;
    }

    applied_hooks_file = fopen(applied_hooks_address, "a");
    fprintf(applied_hooks_file, "%s\n", hook_id);
    fclose(applied_hooks_file);
    printf("add hook %s in applied hooks\n", hook_id);
    return 1;
}

int remove_hook(char hook_id[])
{
    char neogit_dir_address[MAX_ADDRESS_LENGTH];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }

    char applied_hooks_address[MAX_ADDRESS_LENGTH];
    strcpy(applied_hooks_address, neogit_dir_address);
    strcat(applied_hooks_address, "pre-commit/applied hooks");
    FILE *applied_hooks_file = fopen(applied_hooks_address, "r");

    char new_applied_hooks_address[MAX_ADDRESS_LENGTH];
    strcpy(new_applied_hooks_address, neogit_dir_address);
    strcat(new_applied_hooks_address, "pre-commit/new applied hooks");
    FILE *new_applied_hooks_file = fopen(new_applied_hooks_address, "w");

    int find_hook_id_in_applied_hooks = 0;
    char line_in_hooks_file[MAX_HOOKS_ID_LENGTH];
    while (fgets(line_in_hooks_file, sizeof(line_in_hooks_file), applied_hooks_file))
    {
        if (line_in_hooks_file[strlen(line_in_hooks_file) - 1] == '\n')
        {
            line_in_hooks_file[strlen(line_in_hooks_file) - 1] = '\0';
        }
        if (!strcmp(line_in_hooks_file, hook_id))
        {
            find_hook_id_in_applied_hooks = 1;
            continue;
        }
        fprintf(new_applied_hooks_file, "%s\n", line_in_hooks_file);
    }
    fclose(new_applied_hooks_file);
    fclose(applied_hooks_file);

    if (!find_hook_id_in_applied_hooks)
    {
        printf("can't find hook id in applied hooks\n");
        remove(new_applied_hooks_address);
        return 0;
    }

    remove(applied_hooks_address);
    rename(new_applied_hooks_address, applied_hooks_address);

    printf("remove hook %s from applied hooks\n", hook_id);
    return 1;
}
