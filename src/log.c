#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_log(int argc, char *argv[])
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

    if (argc == 2 || (argc == 4 && strcmp(argv[2], "-n") == 0))
    {
        int number_of_commit_to_show = last_commit_id;
        if (argc == 4)
        {
            sscanf(argv[3], "%d", &number_of_commit_to_show);
        }
        if (number_of_commit_to_show > last_commit_id || number_of_commit_to_show < 1)
        {
            printf("error: invalid number of commits to show\n");
            return 0;
        }
        for (int i = last_commit_id; i > last_commit_id - number_of_commit_to_show; i--)
        {
            printf("commit %d:\n", i);
            char commit_data_address[MAX_ADDRESS_LENGHT];
            strcpy(commit_data_address, neogit_dir_address);
            strcat(commit_data_address, "commits_data/commit ");
            char string_i[MAX_NUMBERS_DIGITS];
            sprintf(string_i, "%d", i);
            strcat(commit_data_address, string_i);

            FILE *commit_data_file;
            if ((fopen(commit_data_address, "r")) == NULL)
            {
                printf("not found commit data\n");
                return 0;
            }
            char line_in_commit_data[MAX_LINE_IN_FILES_LENGTH];
            while (fgets(line_in_commit_data, sizeof(line_in_commit_data), commit_data_file))
            {
                printf("%s\n", line_in_commit_data);
            }
            printf("\n");
            fclose(commit_data_file);
        }
    }
}