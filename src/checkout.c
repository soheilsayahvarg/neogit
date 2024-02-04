#include "includes.h"
#include "prototypes.h"
#include "defines.h"

// TODO: user can't checkout when he/she isn't in the HEAD
int run_checkout(int argc, char *argv[])
{
    char neogit_dir_address[MAX_ADDRESS_LENGTH];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }

    // check untrack files with status
    if (run_status(argc, argv) == 1)
    {
        printf("you cant checkout, first use commit to save changes\n");
        return 0;
    }

    int last_commit_id = 0;
    char last_commit_id_address[MAX_ADDRESS_LENGTH];
    strcpy(last_commit_id_address, neogit_dir_address);
    strcat(last_commit_id_address, "last_commit_id");
    FILE *last_commit_id_file = fopen(last_commit_id_address, "r");
    fscanf(last_commit_id_file, "%d", &last_commit_id);

    // get last commit id in branch
    char branch_name[MAX_BRANCH_NAME_LENGTH];
    read_branch_name(branch_name);

    int last_commit_id_in_branch = 0;
    for (int i = last_commit_id; i > 0; i--)
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

    if (argc != 3)
    {
        printf("invalid input\n");
        return 0;
    }

    if (!strncmp(argv[2], "commit ", strlen("commit ")))
    {
        int commit_number = 0;
        sscanf(argv[2], "commit %d", &commit_number);
        if (commit_number <= 0 || commit_number > last_commit_id)
        {
            printf("commit %d is not a valid commit number\n", commit_number);
            return 0;
        }

        // change branch name
        char commit_data_address[MAX_ADDRESS_LENGTH];
        strcpy(commit_data_address, neogit_dir_address);
        strcat(commit_data_address, "commits_data/commit ");
        char commit_number_string[MAX_NUMBERS_DIGITS];
        sprintf(commit_number_string, "%d", commit_number);
        strcat(commit_data_address, commit_number_string);

        FILE *commit_data_file = fopen(commit_data_address, "r");
        char line_in_commit_data[MAX_LINE_IN_FILES_LENGTH];
        fgets(line_in_commit_data, sizeof(line_in_commit_data), commit_data_file);
        fgets(line_in_commit_data, sizeof(line_in_commit_data), commit_data_file);

        char branch_name[MAX_BRANCH_NAME_LENGTH];
        sscanf(line_in_commit_data, "branch : %s ", branch_name);
        if (branch_name[strlen(branch_name) - 1] == ',')
        {
            branch_name[strlen(branch_name) - 1] = '\0';
        }

        char branch_address[MAX_ADDRESS_LENGTH];
        strcpy(branch_address, neogit_dir_address);
        strcat(branch_address, "branch");
        FILE *branch_file = fopen(branch_address, "w");
        fprintf(branch_file, "%s\n", branch_name);
        fclose(branch_file);

        return checkout_to_commit(commit_number);
    }

    if (!strcmp(argv[2], "HEAD"))
    {
        return checkout_to_commit(last_commit_id_in_branch);
    }

    if (!strncmp(argv[2], "HEAD-", strlen("HEAD-")))
    {
        int head_number = 0;
        sscanf(argv[2], "HEAD-%d", &head_number);
        if (head_number < 0 || head_number >= last_commit_id_in_branch)
        {
            printf("error: invalid HEAD number\n");
            return 0;
        }
        for (int i = last_commit_id_in_branch;; i--)
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
                head_number--;
            }
            if (head_number == 0)
            {
                return checkout_to_commit(i);
            }
            if (i == 0)
            {
                printf("error: invalid HEAD number\n");
                return 0;
            }
        }
    }

    // checkout to branch
    for (int i = last_commit_id; i > 0; i--)
    {
        FILE *commit_data_file;
        char commit_data_address[MAX_ADDRESS_LENGTH];
        strcpy(commit_data_address, neogit_dir_address);
        strcat(commit_data_address, "commits_data/commit ");
        char string_number_of_commit[MAX_NUMBERS_DIGITS];
        sprintf(string_number_of_commit, "%d", i);
        strcat(commit_data_address, string_number_of_commit);
        if ((commit_data_file = fopen(commit_data_address, "r")) == NULL)
        {
            printf("not found commit data\n");
            continue;
        }
        char line_in_commit_data[MAX_LINE_IN_FILES_LENGTH];
        fgets(line_in_commit_data, sizeof(line_in_commit_data), commit_data_file);
        fgets(line_in_commit_data, sizeof(line_in_commit_data), commit_data_file);
        if (strstr(line_in_commit_data, argv[2]) != NULL)
        {
            if (checkout_to_commit(i) == 1)
            {
                char branch_address[MAX_ADDRESS_LENGTH];
                strcpy(branch_address, neogit_dir_address);
                strcat(branch_address, "branch");
                FILE *branch_file = fopen(branch_address, "w");
                fprintf(branch_file, "%s\n", argv[2]);
                printf("checkout to branch %s\n", argv[2]);
                return 1;
            }
            return 0;
        }
    }

    printf("branch name does not exist\n");
    return 0;
}

int checkout_to_commit(int commit_number)
{
    char neogit_dir_address[MAX_ADDRESS_LENGTH];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }

    // delete all files in repository
    char repository_address[MAX_ADDRESS_LENGTH];
    strcpy(repository_address, neogit_dir_address);
    repository_address[strlen(repository_address) - strlen(".neogit/")] = '\0';

    DIR *dir;
    struct dirent *entry;
    char command[MAX_BASH_COMMAND];

    dir = opendir(repository_address);
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
        {
            continue;
        }
        if (entry->d_type == 4)
        {
            strcpy(command, "rm -rf \"");
        }
        else
        {
            strcpy(command, "rm -f \"");
        }
        strcat(command, repository_address);
        strcat(command, entry->d_name);
        strcat(command, "\"");
        system(command);
    }

    // copy files from commit
    char commit_files_address[MAX_ADDRESS_LENGTH];
    strcpy(commit_files_address, neogit_dir_address);
    strcat(commit_files_address, "commits_files/commit ");
    char commit_id_string[MAX_NUMBERS_DIGITS];
    sprintf(commit_id_string, "%d/", commit_number);
    strcat(commit_files_address, commit_id_string);
    dir = opendir(commit_files_address);
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
        {
            continue;
        }
        if (entry->d_type == 4)
        {
            strcpy(command, "cp -r \"");
        }
        else
        {
            strcpy(command, "cp \"");
        }
        strcat(command, commit_files_address);
        strcat(command, entry->d_name);
        strcat(command, "\" \"");
        strcat(command, repository_address);
        strcat(command, "\"");
        system(command);
    }

    printf("checkout to commit %d\n", commit_number);
    return 1;
}
