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
    fclose(last_commit_id_file);

    if (argc == 2 || (argc == 4 && strcmp(argv[2], "-n") == 0))
    {
        int number_of_commits_to_show = last_commit_id;
        if (argc == 4)
        {
            sscanf(argv[3], "%d", &number_of_commits_to_show);
        }
        if (number_of_commits_to_show > last_commit_id || number_of_commits_to_show < 1)
        {
            printf("error: invalid number of commits to show\n");
            return 0;
        }
        for (int i = last_commit_id; i > last_commit_id - number_of_commits_to_show; i--)
        {
            show_commit_data(i);
        }
        return 1;
    }

    if (argc == 4 && strcmp(argv[2], "-branch") == 0)
    {
        int branch_does_exits = 0;
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
            if (strstr(line_in_commit_data, argv[3]) != NULL)
            {
                branch_does_exits = 1;
                show_commit_data(i);
            }
        }
        if (!branch_does_exits)
        {
            printf("branch name does not exist\n");
        }
        return 1;
    }

    if (argc == 4 && strcmp(argv[2], "-author") == 0)
    {
        int author_does_exits = 0;
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
            char author_name[MAX_BRANCH_NAME_LENGHT];
            sscanf(line_in_commit_data, "username : %s", author_name);
            author_name[strlen(author_name) - 1] = '\0';
            if (!strcmp(author_name, argv[3]))
            {
                author_does_exits = 1;
                show_commit_data(i);
            }
        }
        if (!author_does_exits)
        {
            printf("author does not exist\n");
        }
        return 1;
    }

    if (argc == 4 && ((strcmp(argv[2], "-since") == 0) || (strcmp(argv[2], "-before") == 0)))
    {
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
            fgets(line_in_commit_data, sizeof(line_in_commit_data), commit_data_file);
            fgets(line_in_commit_data, sizeof(line_in_commit_data), commit_data_file);

            if (line_in_commit_data[strlen(line_in_commit_data) - 1] == '\n')
            {
                line_in_commit_data[strlen(line_in_commit_data) - 1] = '\0';
            }

            memmove(line_in_commit_data, line_in_commit_data + strlen("date : "), strlen(line_in_commit_data) - strlen("date : ") + 1);
            if ((strcmp(argv[2], "-before") == 0) ^ (comprator_time(line_in_commit_data, argv[3]) == 1))
            {
                show_commit_data(i);
            }
        }
        return 1;
    }
    // TODO: whildcard to serarch
    if (argc >= 4 && strcmp(argv[2], "-search") == 0)
    {
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
            fgets(line_in_commit_data, sizeof(line_in_commit_data), commit_data_file);

            for (int j = 3; j < argc; j++)
            {
                if (strstr(line_in_commit_data, argv[j]) != NULL)
                {
                    show_commit_data(i);
                    break;
                }
            }
        }
        return 1;
    }

    printf("invliad input\n");
    return 0;
}

int show_commit_data(int number_of_commit)
{
    FILE *commit_data_file;

    char neogit_dir_address[MAX_ADDRESS_LENGHT];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }

    printf("commit %d:\n", number_of_commit);
    char commit_data_address[MAX_ADDRESS_LENGHT];
    strcpy(commit_data_address, neogit_dir_address);
    strcat(commit_data_address, "commits_data/commit ");
    char string_number_of_commit[MAX_NUMBERS_DIGITS];
    sprintf(string_number_of_commit, "%d", number_of_commit);
    strcat(commit_data_address, string_number_of_commit);

    if ((commit_data_file = fopen(commit_data_address, "r")) == NULL)
    {
        printf("not found commit data\n");
        return 0;
    }

    char line_in_commit_data[MAX_LINE_IN_FILES_LENGTH];
    while (fgets(line_in_commit_data, sizeof(line_in_commit_data), commit_data_file))
    {
        printf("%s", line_in_commit_data);
    }
    fclose(commit_data_file);
}
