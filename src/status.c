#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_status(int argc, char *argv[])
{
    char neogit_dir_address[MAX_ADDRESS_LENGHT];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }

    char repository_address[MAX_ADDRESS_LENGHT];
    strcpy(repository_address, neogit_dir_address);
    repository_address[strlen(repository_address) - strlen(".neogit/")] = '\0';

    char stage_address[MAX_ADDRESS_LENGHT];
    strcpy(stage_address, neogit_dir_address);
    strcat(stage_address, "stage/");

    // get last commit id in branch
    char branch_name[MAX_BRANCH_NAME_LENGHT];
    read_branch_name(branch_name);
    int last_commit_id_in_branch = 0;

    int last_commit_id = 0;
    char last_commit_id_address[MAX_ADDRESS_LENGHT];
    strcpy(last_commit_id_address, neogit_dir_address);
    strcat(last_commit_id_address, "last_commit_id");
    FILE *last_commit_id_file = fopen(last_commit_id_address, "r");
    fscanf(last_commit_id_file, "%d", &last_commit_id);

    for (int i = last_commit_id; i > 0; i--)
    {
        char commit_data_address[MAX_ADDRESS_LENGHT];
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

    char commit_address[MAX_ADDRESS_LENGHT];
    strcpy(commit_address, neogit_dir_address);
    strcat(commit_address, "commits_files/commit ");
    char commit_id_string[MAX_NUMBERS_DIGITS];
    sprintf(commit_id_string, "%d/", last_commit_id_in_branch);
    strcat(commit_address, commit_id_string);

    return check_status(repository_address, stage_address, commit_address);
}

// TODO : check file permissions
int check_status(char repository_address[], char stage_address[], char commit_address[])
{
    static int find_untrack_filess = 0;

    DIR *repository_dir;
    DIR *stage_dir;
    DIR *commit_dir;
    struct dirent *entry;

    char file_repository_address[MAX_ADDRESS_LENGHT], file_stage_address[MAX_ADDRESS_LENGHT], file_commit_address[MAX_ADDRESS_LENGHT];

    if ((repository_dir = opendir(repository_address)) != NULL)
    {
        while ((entry = readdir(repository_dir)) != NULL)
        {
            if (entry->d_name[0] == '.')
            {
                continue;
            }

            strcpy(file_repository_address, repository_address);
            strcat(file_repository_address, entry->d_name);
            strcpy(file_stage_address, stage_address);
            strcat(file_stage_address, entry->d_name);
            strcpy(file_commit_address, commit_address);
            strcat(file_commit_address, entry->d_name);

            if (entry->d_type == 4)
            {
                strcat(file_repository_address, "/");
                strcat(file_stage_address, "/");
                strcat(file_commit_address, "/");
                check_status(file_repository_address, file_stage_address, file_commit_address);
            }
            else
            {
                int compare_repository_and_stage = compare_file(file_repository_address, file_stage_address);
                if (compare_repository_and_stage == 0)
                {
                    printf("%s +\n", entry->d_name);
                    find_untrack_filess = 1;
                    continue;
                }
                if (compare_repository_and_stage == 1)
                {
                    printf("%s +M\n", entry->d_name);
                    find_untrack_filess = 1;
                    continue;
                }
                int compare_repository_and_commit = compare_file(file_repository_address, file_commit_address);
                if (compare_repository_and_commit == 0)
                {
                    // printf("%s -\n", entry->d_name);
                    continue;
                }
                if (compare_repository_and_commit == 1)
                {
                    printf("%s -M\n", entry->d_name);
                    find_untrack_filess = 1;
                    continue;
                }

                printf("%s -A\n", entry->d_name);
                find_untrack_filess = 1;
                continue;
            }
        }
    }

    if ((stage_dir = opendir(stage_address)) != NULL)
    {
        while ((entry = readdir(stage_dir)) != NULL)
        {
            if (entry->d_name[0] == '.')
            {
                continue;
            }

            strcpy(file_repository_address, repository_address);
            strcat(file_repository_address, entry->d_name);
            strcpy(file_stage_address, stage_address);
            strcat(file_stage_address, entry->d_name);
            strcpy(file_commit_address, commit_address);
            strcat(file_commit_address, entry->d_name);

            if (entry->d_type == 4)
            {
                strcat(file_repository_address, "/");
                strcat(file_stage_address, "/");
                strcat(file_commit_address, "/");
                check_status(file_repository_address, file_stage_address, file_commit_address);
            }
            else
            {
                int compare_repository_and_stage = compare_file(file_repository_address, file_stage_address);
                if (compare_repository_and_stage == -1)
                {
                    printf("%s +D\n", entry->d_name);
                    // find_untrack_filess = 1;
                }
            }
        }
    }
    if ((commit_dir = opendir(commit_address)) != NULL)
    {
        while ((entry = readdir(commit_dir)) != NULL)
        {
            if (entry->d_name[0] == '.')
            {
                continue;
            }

            strcpy(file_repository_address, repository_address);
            strcat(file_repository_address, entry->d_name);
            strcpy(file_stage_address, stage_address);
            strcat(file_stage_address, entry->d_name);
            strcpy(file_commit_address, commit_address);
            strcat(file_commit_address, entry->d_name);

            if (entry->d_type == 4)
            {
                strcat(file_repository_address, "/");
                strcat(file_stage_address, "/");
                strcat(file_commit_address, "/");
                check_status(file_repository_address, file_stage_address, file_commit_address);
            }

            else
            {
                int compare_repository_and_commit = compare_file(file_repository_address, file_commit_address);
                if (compare_repository_and_commit == -1)
                {
                    printf("%s -D\n", entry->d_name);
                    // find_untrack_filess = 1;
                }
            }
        }
    }
    return find_untrack_filess;
}

int compare_file(char file_path_1[], char file_path_2[])
{
    FILE *file1 = fopen(file_path_1, "r");
    FILE *file2 = fopen(file_path_2, "r");

    if (file1 == NULL)
    {
        return -1;
    }
    if (file2 == NULL)
    {
        return -2;
    }

    int char1, char2;
    do
    {
        char1 = fgetc(file1);
        char2 = fgetc(file2);
        if (char1 != char2)
        {
            fclose(file1);
            fclose(file2);
            return 1;
        }

    } while (char1 != EOF && char2 != EOF);
    fclose(file1);
    fclose(file2);
    return 0;
}
