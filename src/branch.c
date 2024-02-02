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

    // edit last commit

    // read last commit id
    int last_commit_id = 0;
    char last_commit_id_address[MAX_ADDRESS_LENGHT];
    strcpy(last_commit_id_address, neogit_dir_address);
    strcat(last_commit_id_address, "last_commit_id");
    FILE *last_commit_id_file = fopen(last_commit_id_address, "r");
    fscanf(last_commit_id_file, "%d", &last_commit_id);
    char last_commit_id_string[MAX_NUMBERS_DIGITS];
    sprintf(last_commit_id_string, "%d", last_commit_id);

    // open commit datas file
    char commit_data_address[MAX_ADDRESS_LENGHT];
    strcpy(commit_data_address, neogit_dir_address);
    strcat(commit_data_address, "commits_data/commit ");
    strcat(commit_data_address, last_commit_id_string);
    FILE *old_commits_data_file = fopen(commit_data_address, "r");

    // make new commit data file
    char new_commit_data_address[MAX_ADDRESS_LENGHT];
    strcpy(new_commit_data_address, commit_data_address);
    strcat(new_commit_data_address, "new");
    FILE *new_commit_data_file = fopen(new_commit_data_address, "w");

    char line_in_commit_data[MAX_LINE_IN_FILES_LENGTH];
    while (fgets(line_in_commit_data, sizeof(line_in_commit_data), old_commits_data_file))
    {
        if (line_in_commit_data[strlen(line_in_commit_data) - 1] == '\n')
        {
            line_in_commit_data[strlen(line_in_commit_data) - 1] = '\0';
        }
        fprintf(new_commit_data_file, "%s", line_in_commit_data);
        if (strncmp(line_in_commit_data, "branch : ", strlen("branch : ")) == 0)
        {
            fprintf(new_commit_data_file, ", %s", branch_name);
        }
        fprintf(new_commit_data_file, "\n");
    }
    fclose(new_commit_data_file);
    fclose(old_commits_data_file);
    remove(commit_data_address);
    rename(new_commit_data_address, commit_data_address);
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
