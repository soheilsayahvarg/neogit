#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_grep(int argc, char *argv[])
{

    char neogit_dir_address[MAX_ADDRESS_LENGTH];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }

    char file_address[MAX_ADDRESS_LENGTH];
    getcwd(file_address, sizeof(file_address));

    if (argc < 6)
    {
        printf("invalid input\n");
        return 0;
    }
    if (strcmp(argv[2], "-f") != 0 || strcmp(argv[4], "-p") != 0)
    {
        printf("invalid input\n");
        return 0;
    }

    strcat(file_address, argv[3]);

    int show_number_of_line = 0;
    if (strcmp(argv[argc - 1], "-n") == 0)
    {
        show_number_of_line = 1;
    }

    int last_commit_id = 0;
    char last_commit_id_address[MAX_ADDRESS_LENGTH];
    strcpy(last_commit_id_address, neogit_dir_address);
    strcat(last_commit_id_address, "last_commit_id");
    FILE *last_commit_id_file = fopen(last_commit_id_address, "r");
    fscanf(last_commit_id_file, "%d", &last_commit_id);

    if (strcmp(argv[argc - 2 - show_number_of_line], "-c") == 0)
    {
        int commit_id = 0;
        sscanf(argv[argc - 1 - show_number_of_line], "commit %d", &commit_id);
        if (commit_id <= 0 || commit_id > last_commit_id)
        {
            printf("invalid commit id\n");
            return 0;
        }
        char file_address_in_commit[MAX_ADDRESS_LENGTH];
        strcpy(file_address_in_commit, neogit_dir_address);
        strcat(file_address_in_commit, "commits_files/commit ");
        char commit_number_string[MAX_NUMBERS_DIGITS];
        sprintf(commit_number_string, "%d", commit_id);
        strcat(file_address_in_commit, commit_number_string);
        strcat(file_address_in_commit, "/");

        strcat(file_address_in_commit, file_address + strlen(file_address_in_commit) - strlen(".neogit/commits_files/commit / ") - strlen(commit_number_string));
        strcpy(file_address, file_address_in_commit);
    }
    printf("'%s'", file_address);
}