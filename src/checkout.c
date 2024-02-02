#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_checkout(int argc, char *argv[])
{
    char neogit_dir_address[MAX_ADDRESS_LENGHT];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }
    int last_commit_id = 0;
    char last_commit_id_address[MAX_ADDRESS_LENGHT];
    strcpy(last_commit_id_address, neogit_dir_address);
    strcat(last_commit_id_address, "last_commit_id");
    FILE *last_commit_id_file = fopen(last_commit_id_address, "r");
    fscanf(last_commit_id_file, "%d", &last_commit_id);

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
        return checkout_to_commit(commit_number);
    }

    if (!strcmp(argv[2], "HEAD"))
    {
        return checkout_to_commit(last_commit_id);
    }

    if (!strncmp(argv[2], "HEAD-", strlen("HEAD-")))
    {
        int head_number = 0;
        sscanf(argv[2], "HEAD-%d", &head_number);
        if (head_number < 0 || head_number >= last_commit_id)
        {
            printf("error: invalid HEAD number\n");
            return 0;
        }
        return checkout_to_commit(last_commit_id - head_number);
    }

    for (int i = last_commit_id; i > 0; i--)
    {
        FILE *commit_data_file;
        char commit_data_address[MAX_ADDRESS_LENGHT];
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
            return checkout_to_commit(i);
        }
    }

    printf("branch name does not exist\n");
    return 0;
}

int checkout_to_commit(int commit_number)
{
    printf("commit number : %d\n", commit_number);
    return 1;
}
