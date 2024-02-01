#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_branch(int argc, char *argv[])
{
    char neogit_dir_address[MAX_ADDRESS_LENGHT];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }

    if (argc == 2)
    {
        char branch_address[MAX_ADDRESS_LENGHT];
        strcpy(branch_address, neogit_dir_address);
        strcat(branch_address, "branch");
        char branches_address[MAX_ADDRESS_LENGHT];
        strcpy(branches_address, neogit_dir_address);
        strcat(branches_address, "branches");

        FILE *branch_file, *branches_file;
        branch_file = fopen(branch_address, "r");
        branches_file = fopen(branches_address, "r");

        if (branch_file == NULL || branches_file == NULL)
        {
            printf("not found branch file\n");
            return 0;
        }

        char line_in_branch[MAX_BRANCH_NAME_LENGHT];
        fgets(line_in_branch, sizeof(line_in_branch), branch_file);
        printf("your branch is %s\n", line_in_branch);
        printf("all branches:\n");
        while (fgets(line_in_branch, sizeof(line_in_branch), branches_file))
        {
            printf("%s", line_in_branch);
        }

        fclose(branch_file);
        fclose(branches_file);
        return 1;
    }

    if (argc == 3)
    {
        return add_branch(argv[2], neogit_dir_address);
    }

    printf("invalid input\n");
    return 0;
}

int add_branch(char branch_name[], char neogit_dir_address[])
{
    char branches_address[MAX_ADDRESS_LENGHT];
    strcpy(branches_address, neogit_dir_address);
    strcat(branches_address, "branches");

    FILE *branches_file;
    branches_file = fopen(branches_address, "r");

    char line_in_branches[MAX_BRANCH_NAME_LENGHT];
    while (fgets(line_in_branches, sizeof(line_in_branches), branches_file))
    {
        if (line_in_branches[strlen(line_in_branches) - 1] == '\n')
        {
            line_in_branches[strlen(line_in_branches) - 1] = '\0';
        }

        if (!strcmp(line_in_branches, branch_name))
        {
            printf("you have this branch\n");
            return 0;
        }
    }
    branches_file = fopen(branches_address, "a");
    fprintf(branches_file, "%s\n", branch_name);
    fclose(branches_file);

    // create a new commit for new branch
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

    // make commit datas file
    char new_commit_data_address[MAX_ADDRESS_LENGHT];
    strcpy(new_commit_data_address, neogit_dir_address);
    strcat(new_commit_data_address, "commits_data/commit ");
    strcat(new_commit_data_address, last_commit_id_string);
    new_commit_data_address[strlen(new_commit_data_address) - 1] = '\0';

    FILE *commit_data_file = fopen(new_commit_data_address, "w");
    fprintf(commit_data_file, "username : %s, useremail : %s\n", username, useremail);
    fprintf(commit_data_file, "branch : %s\n", branch_name);
    fprintf(commit_data_file, "creat branch %s\n", branch_name);
    time_t now = time(NULL);
    fprintf(commit_data_file, "date : %s\n", asctime(localtime(&now)));
    fclose(commit_data_file);

    printf("create \"%s\" branch\n", branch_name);
    return 1;
}

int read_branch(char branch_name[])
{
    char neogit_dir_address[MAX_ADDRESS_LENGHT];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }

    char branch_address[MAX_ADDRESS_LENGHT];
    strcpy(branch_address, neogit_dir_address);
    strcat(branch_address, "branch");
    FILE *branch_file;

    if ((branch_file = fopen(branch_address, "r")) == NULL)
    {
        printf("not found branch file\n");
        return 0;
    }
    fgets(branch_name, MAX_BRANCH_NAME_LENGHT, branch_file);
    if (branch_name[strlen(branch_name) - 1] == '\n')
    {
        branch_name[strlen(branch_name) - 1] = '\0';
    }
    return 1;
}
