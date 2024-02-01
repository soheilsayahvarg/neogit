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
    struct dirent *entry;
    DIR *dir;
    char command[MAX_BASH_COMMAND];

    char username[MAX_USERNAME_LENGHT];
    char useremail[MAX_USEREMAIL_LENGTH];
    if (!read_user_config(username, useremail))
    {
        printf("first set username and useremail with neogit config user.mame or user.email\n");
        return 0;
    }

    char neogit_dir_address[MAX_ADDRESS_LENGHT];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }
    // delete all_stage
    char all_stage_address[MAX_ADDRESS_LENGHT];
    strcpy(all_stage_address, neogit_dir_address);
    strcat(all_stage_address, "all_stage");
    FILE *all_stage_file;
    if ((all_stage_file = fopen(all_stage_address, "r")) == NULL)
    {
        printf("stage is empty\n");
        return 0;
    }
    fclose(all_stage_file);
    strcpy(command, "rm -f \"");
    strcat(command, all_stage_address);
    strcat(command, "\"");
    system(command);

    // read last commit id
    int last_commit_id = 0;
    char last_commit_id_address[MAX_ADDRESS_LENGHT];
    strcpy(last_commit_id_address, neogit_dir_address);
    strcat(last_commit_id_address, "last_commit_id");
    FILE *last_commit_id_file = fopen(last_commit_id_address, "r");
    fscanf(last_commit_id_file, "%d", &last_commit_id);
    last_commit_id++;
    fclose(last_commit_id_file);
    last_commit_id_file = fopen(last_commit_id_address, "w");
    fprintf(last_commit_id_file, "%d\n", last_commit_id);

    // make dir new commit
    char new_commit_files_address[MAX_ADDRESS_LENGHT];
    strcpy(new_commit_files_address, neogit_dir_address);
    strcat(new_commit_files_address, "commits_files/commit ");
    char last_commit_id_string[MAX_NUMBERS_DIGITS];
    sprintf(last_commit_id_string, "%d/", last_commit_id);
    strcat(new_commit_files_address, last_commit_id_string);
    mkdir(new_commit_files_address, 0755);

    char file_path_1[MAX_ADDRESS_LENGHT];
    char file_path_2[MAX_ADDRESS_LENGHT];
    if (last_commit_id > 1)
    {
        // copy old commit
        char old_commit_files_address[MAX_ADDRESS_LENGHT];
        strcpy(old_commit_files_address, neogit_dir_address);
        strcat(old_commit_files_address, "commits_files/commit ");
        char old_commit_id_string[MAX_NUMBERS_DIGITS];
        sprintf(old_commit_id_string, "%d/", last_commit_id - 1);
        strcat(old_commit_files_address, old_commit_id_string);
        dir = opendir(old_commit_files_address);

        while ((entry = readdir(dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            strcpy(file_path_1, old_commit_files_address);
            strcat(file_path_1, entry->d_name);
            strcpy(file_path_2, new_commit_files_address);

            if (entry->d_type == 4)
            {
                strcpy(command, "cp -r \"");
                strcat(command, file_path_1);
                strcat(command, "\" \"");
                strcat(command, file_path_2);
                strcat(command, "\"");
            }
            else
            {
                strcpy(command, "cp \"");
                strcat(command, file_path_1);
                strcat(command, "\" \"");
                strcat(command, file_path_2);
                strcat(command, entry->d_name);
                strcat(command, "\"");
            }
            system(command);
        }
    }

    // copy stage
    char stage_files_address[MAX_ADDRESS_LENGHT];
    strcpy(stage_files_address, neogit_dir_address);
    strcat(stage_files_address, "stage/");
    dir = opendir(stage_files_address);

    char command_delete[MAX_BASH_COMMAND];
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }
        strcpy(file_path_1, stage_files_address);
        strcat(file_path_1, entry->d_name);
        strcpy(file_path_2, new_commit_files_address);

        if (entry->d_type == 4)
        {
            strcpy(command, "cp -r \"");
            strcat(command, file_path_1);
            strcat(command, "\" \"");
            strcat(command, file_path_2);
            strcat(command, "\"");

            strcpy(command_delete, "rm -rf \"");
            strcat(command_delete, file_path_1);
            strcat(command_delete, "\"");
        }
        else
        {
            strcpy(command, "cp \"");
            strcat(command, file_path_1);
            strcat(command, "\" \"");
            strcat(command, file_path_2);
            strcat(command, entry->d_name);
            strcat(command, "\"");

            strcpy(command_delete, "rm -f \"");
            strcat(command_delete, file_path_1);
            strcat(command_delete, "\"");
        }

        system(command);
        system(command_delete);
    }

    printf("commit files\n");
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
