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
        char stage_files_address[MAX_ADDRESS_LENGTH];
        strcpy(stage_files_address, neogit_dir_address);
        strcat(stage_files_address, "stage/");

        return pre_commit_from_stage(stage_files_address);
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

int pre_commit_from_stage(char dir_address[])
{
    static int dont_find_failed = 1;

    DIR *dir = opendir(dir_address);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
        {
            continue;
        }
        char file_address[MAX_ADDRESS_LENGTH];
        strcpy(file_address, dir_address);
        strcat(file_address, entry->d_name);
        if (entry->d_type == 4)
        {
            dont_find_failed &= pre_commit_from_stage(file_address);
            continue;
        }

        dont_find_failed &= check_pre_commit(file_address, entry->d_name);
    }

    return dont_find_failed;
}

int check_pre_commit(char file_address[], char file_name[])
{
    int dont_find_failed_hook = 1;

    char neogit_dir_address[MAX_ADDRESS_LENGTH];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }

    printf("%s\n", file_name);

    char applied_hooks_address[MAX_ADDRESS_LENGTH];
    strcpy(applied_hooks_address, neogit_dir_address);
    strcat(applied_hooks_address, "pre-commit/applied hooks");
    FILE *applied_hooks_file = fopen(applied_hooks_address, "r");

    char line_in_hooks_file[MAX_HOOKS_ID_LENGTH];
    while (fgets(line_in_hooks_file, sizeof(line_in_hooks_file), applied_hooks_file))
    {
        if (line_in_hooks_file[strlen(line_in_hooks_file) - 1] == '\n')
        {
            line_in_hooks_file[strlen(line_in_hooks_file) - 1] = '\0';
        }

        int check_hook_result = 2;

        if (!strcmp(line_in_hooks_file, "todo-check"))
        {
            check_hook_result = hook_todo_check(file_address);
        }
        else if (!strcmp(line_in_hooks_file, "eof-blank-space"))
        {
            check_hook_result = hook_eof_blank_space(file_address);
        }
        else if (!strcmp(line_in_hooks_file, "format-check"))
        {
            check_hook_result = hook_format_check(file_address);
        }
        else if (!strcmp(line_in_hooks_file, "balance-braces"))
        {
            check_hook_result = hook_balance_braces(file_address);
        }
        else if (!strcmp(line_in_hooks_file, "indentation-check"))
        {
            continue;
        }
        else if (!strcmp(line_in_hooks_file, "static-error-check"))
        {
            continue;
        }
        else if (!strcmp(line_in_hooks_file, "file-size-check"))
        {
            continue;
        }
        else if (!strcmp(line_in_hooks_file, "character-limit"))
        {
            continue;
        }
        else if (!strcmp(line_in_hooks_file, "time-limit"))
        {
            continue;
        }

        if (check_hook_result == 1)
        {
            printf("\"%s\".............................." GREEN "PASSED\n" RESET, line_in_hooks_file);
        }
        else if (check_hook_result == 0)
        {
            printf("\"%s\".............................." BLUE "SKIPPED\n" RESET, line_in_hooks_file);
        }
        else if (check_hook_result == -1)
        {
            printf("\"%s\".............................." RED "FAILED\n" RESET, line_in_hooks_file);
            dont_find_failed_hook = 0;
        }
    }
    printf("\n");
    fclose(applied_hooks_file);
}

int hook_todo_check(char file_address[])
{

    if (strncmp(file_address + strlen(file_address) - strlen(".c"), ".c", strlen(".c")) == 0 ||
        strncmp(file_address + strlen(file_address) - strlen(".cpp"), ".cpp", strlen(".cpp")) == 0)
    {
        FILE *file = fopen(file_address, "r");
        char line[MAX_LINE_IN_FILES_LENGTH];
        while (fgets(line, sizeof(line), file))
        {
            if (strstr(line, "TODO") != NULL)
            {
                return -1;
            }
        }
        return 1;
    }

    if (strncmp(file_address + strlen(file_address) - strlen(".txt"), ".txt", strlen(".txt")) == 0)
    {
        FILE *file = fopen(file_address, "r");
        char line[MAX_LINE_IN_FILES_LENGTH];
        while (fgets(line, sizeof(line), file))
        {
            if (strstr(line, "TODO") != NULL)
            {
                return -1;
            }
        }
        return 1;
    }

    return 0;
}

int hook_eof_blank_space(char file_address[])
{
    if (strncmp(file_address + strlen(file_address) - strlen(".c"), ".c", strlen(".c")) == 0 ||
        strncmp(file_address + strlen(file_address) - strlen(".cpp"), ".cpp", strlen(".cpp")) == 0 ||
        strncmp(file_address + strlen(file_address) - strlen(".txt"), ".txt", strlen(".txt")) == 0)
    {
        FILE *file = fopen(file_address, "r");
        char line[MAX_LINE_IN_FILES_LENGTH];
        int dont_find_blank_space = 1;

        while (fgets(line, sizeof(line), file))
        {
            if (!strcmp(line, "\n"))
            {
                dont_find_blank_space = -1;
                continue;
            }
            if (line[strlen(line) - 1] == '\n')
            {
                line[strlen(line) - 1] = '\0';
            }
            if (line[strlen(line) - 1] == ' ')
            {
                dont_find_blank_space = -1;
                continue;
            }
            dont_find_blank_space = 1;
            continue;
        }
        return dont_find_blank_space;
    }
    return 0;
}

int hook_format_check(char file_address[])
{
    if (strncmp(file_address + strlen(file_address) - strlen(".c"), ".c", strlen(".c")) == 0 ||
        strncmp(file_address + strlen(file_address) - strlen(".cpp"), ".cpp", strlen(".cpp")) == 0 ||
        strncmp(file_address + strlen(file_address) - strlen(".txt"), ".txt", strlen(".txt")) == 0 ||
        strncmp(file_address + strlen(file_address) - strlen(".mp3"), ".mp3", strlen(".mp3")) == 0 ||
        strncmp(file_address + strlen(file_address) - strlen(".mp4"), ".mp4", strlen(".mp4")) == 0 ||
        strncmp(file_address + strlen(file_address) - strlen(".wav"), ".wav", strlen(".wav")) == 0 ||
        strncmp(file_address + strlen(file_address) - strlen(".exe"), ".exe", strlen(".exe")) == 0)
    {
        return 1;
    }
    return -1;
}

int hook_balance_braces(char file_address[])
{
    if (strncmp(file_address + strlen(file_address) - strlen(".c"), ".c", strlen(".c")) == 0 ||
        strncmp(file_address + strlen(file_address) - strlen(".cpp"), ".cpp", strlen(".cpp")) == 0 ||
        strncmp(file_address + strlen(file_address) - strlen(".txt"), ".txt", strlen(".txt")) == 0)
    {
        FILE *file = fopen(file_address, "r");
        int braces = 0, brackets = 0, parentheses = 0;
        char character = '!';

        while ((character = fgetc(file)) != EOF)
        {
            if (character == '{')
            {
                braces++;
            }
            else if (character == '}')
            {
                braces--;
            }
            else if (character == '[')
            {
                brackets++;
            }
            else if (character == ']')
            {
                brackets--;
            }
            else if (character == '(')
            {
                parentheses++;
            }
            else if (character == ')')
            {
                parentheses--;
            }
        }

        if (braces == 0 && brackets == 0 && parentheses == 0)
        {
            return 1;
        }

        return -1;
    }
    return 0;
}
