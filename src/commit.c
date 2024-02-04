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
        if (strlen(argv[3]) > MAX_COMMIT_MESSAGE_LENGTH)
        {
            printf("max commit message is %d characters\n", MAX_COMMIT_MESSAGE_LENGTH);
            return 0;
        }

        return creat_commit(argv[3]);
    }
    if (!strcmp(argv[2], "-s"))
    {
        char neogit_dir_address[MAX_ADDRESS_LENGTH];
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
            char shortcut_message[MAX_COMMIT_MESSAGE_LENGTH + 1];
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

int creat_commit(char message[])
{
    struct dirent *entry;
    DIR *dir;
    char command[MAX_BASH_COMMAND];

    char username[MAX_USERNAME_LENGTH];
    char useremail[MAX_USEREMAIL_LENGTH];
    if (!read_user_config(username, useremail))
    {
        printf("first set username and useremail with neogit config user.mame or user.email\n");
        return 0;
    }

    char branch_name[MAX_BRANCH_NAME_LENGTH];
    if (!read_branch_name(branch_name))
    {
        return 0;
    }

    char neogit_dir_address[MAX_ADDRESS_LENGTH];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }

    // check user is in HEAD
    char user_is_in_HEAD_address[MAX_ADDRESS_LENGTH];
    strcpy(user_is_in_HEAD_address, neogit_dir_address);
    strcat(user_is_in_HEAD_address, "user is in HEAD");
    FILE *user_is_in_HEAD_file;
    if ((user_is_in_HEAD_file = fopen(user_is_in_HEAD_address, "r")) == NULL)
    {
        printf("you can't commit, first checkout to HEAD\n");
        return 0;
    }
    fclose(user_is_in_HEAD_file);

    // delete all_stage
    char all_stage_address[MAX_ADDRESS_LENGTH];
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
    char last_commit_id_address[MAX_ADDRESS_LENGTH];
    strcpy(last_commit_id_address, neogit_dir_address);
    strcat(last_commit_id_address, "last_commit_id");
    FILE *last_commit_id_file = fopen(last_commit_id_address, "r");
    fscanf(last_commit_id_file, "%d", &last_commit_id);
    last_commit_id++;
    fclose(last_commit_id_file);
    last_commit_id_file = fopen(last_commit_id_address, "w");
    fprintf(last_commit_id_file, "%d\n", last_commit_id);

    // get last commit id in branch
    int last_commit_id_in_branch = 0;
    for (int i = last_commit_id - 1; i > 0; i--)
    {
        char commit_data_address[MAX_ADDRESS_LENGTH];
        strcpy(commit_data_address, neogit_dir_address);
        strcat(commit_data_address, "commits_data/commit ");
        char commit_id_string[MAX_NUMBERS_DIGITS];
        sprintf(commit_id_string, "%d", i);
        strcat(commit_data_address, commit_id_string);

        FILE *commit_data_file = fopen(commit_data_address, "r");
        char line_in_commit_data[MAX_LINE_IN_FILES_LENGTH];
        fgets(line_in_commit_data, sizeof(line_in_commit_data), commit_data_file);
        fgets(line_in_commit_data, sizeof(line_in_commit_data), commit_data_file);
        if (strstr(line_in_commit_data, branch_name) != NULL)
        {
            last_commit_id_in_branch = i;
            break;
        }
    }

    // make dir new commit
    char new_commit_files_address[MAX_ADDRESS_LENGTH];
    strcpy(new_commit_files_address, neogit_dir_address);
    strcat(new_commit_files_address, "commits_files/commit ");
    char last_commit_id_string[MAX_NUMBERS_DIGITS];
    sprintf(last_commit_id_string, "%d/", last_commit_id);
    strcat(new_commit_files_address, last_commit_id_string);
    mkdir(new_commit_files_address, 0755);

    char file_path_1[MAX_ADDRESS_LENGTH];
    char file_path_2[MAX_ADDRESS_LENGTH];
    if (last_commit_id > 1)
    {
        // copy old commit
        char old_commit_files_address[MAX_ADDRESS_LENGTH];
        strcpy(old_commit_files_address, neogit_dir_address);
        strcat(old_commit_files_address, "commits_files/commit ");
        char old_commit_id_string[MAX_NUMBERS_DIGITS];
        sprintf(old_commit_id_string, "%d/", last_commit_id_in_branch);
        strcat(old_commit_files_address, old_commit_id_string);
        dir = opendir(old_commit_files_address);

        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_name[0] == '.')
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
    char stage_files_address[MAX_ADDRESS_LENGTH];
    strcpy(stage_files_address, neogit_dir_address);
    strcat(stage_files_address, "stage/");
    dir = opendir(stage_files_address);

    char command_delete[MAX_BASH_COMMAND];
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
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

    // delete add directory files
    char add_address[MAX_ADDRESS_LENGTH];
    strcpy(add_address, neogit_dir_address);
    strcat(add_address, "add/");
    strcpy(command, "rm -rf \"");
    strcat(command, add_address);
    strcat(command, "\"");
    system(command);
    if (mkdir(add_address, 0755) != 0)
    {
        return 0;
    }

    // change last add
    char last_add_address[MAX_ADDRESS_LENGTH];
    strcpy(last_add_address, neogit_dir_address);
    strcat(last_add_address, "last_add");
    FILE *last_add_file = fopen(last_add_address, "w");
    fprintf(last_add_file, "%d\n", 0);
    fclose(last_add_file);

    // make commit datas file
    char new_commit_data_address[MAX_ADDRESS_LENGTH];
    strcpy(new_commit_data_address, neogit_dir_address);
    strcat(new_commit_data_address, "commits_data/commit ");
    strcat(new_commit_data_address, last_commit_id_string);
    new_commit_data_address[strlen(new_commit_data_address) - 1] = '\0';

    FILE *commit_data_file = fopen(new_commit_data_address, "w");
    fprintf(commit_data_file, "username : %s, useremail : %s\n", username, useremail);
    fprintf(commit_data_file, "branch : %s\n", branch_name);
    fprintf(commit_data_file, "message : %s\n", message);
    time_t now = time(NULL);
    fprintf(commit_data_file, "date : %s\n", asctime(localtime(&now)));
    fclose(commit_data_file);

    // make message file
    char message_address[MAX_ADDRESS_LENGTH];
    strcpy(message_address, neogit_dir_address);
    strcat(message_address, "commits_message/");
    strcat(message_address, message);
    FILE *message_file = fopen(message_address, "w");

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
    if (strlen(argv[3]) > MAX_COMMIT_MESSAGE_LENGTH)
    {
        printf("max commit message is %d characters\n", MAX_COMMIT_MESSAGE_LENGTH);
        return 0;
    }

    char neogit_dir_address[MAX_ADDRESS_LENGTH];
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
    char neogit_dir_address[MAX_ADDRESS_LENGTH];
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

    char neogit_dir_address[MAX_ADDRESS_LENGTH];
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
