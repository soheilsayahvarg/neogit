#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int find_neogit_dir(char cwd[])
{
    if (getcwd(cwd, MAX_ADDRESS_LENGHT) == NULL)
    {
        printf("error getcwd\n");
        return 0;
    }

    char first_cwd[MAX_ADDRESS_LENGHT];
    strcpy(first_cwd, cwd);

    int exists = 0;

    char tmp_cwd[MAX_ADDRESS_LENGHT];
    struct dirent *entry;
    do
    {
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            printf("error opening current directory\n");
            chdir(first_cwd);
            return 0;
        }
        while ((entry = readdir(dir)) != NULL)
        {
            if (entry->d_type == 4 && strcmp(entry->d_name, ".neogit") == 0)
            {
                exists = 1;
                break;
            }
        }
        closedir(dir);

        if (getcwd(tmp_cwd, sizeof(tmp_cwd)) == NULL)
        {
            printf("error getcwd\n");
            chdir(first_cwd);
            return 0;
        }

        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
            {
                chdir(first_cwd);
                return 0;
            }
        }

        if (strcmp(tmp_cwd, "/") == 0)
        {
            chdir(first_cwd);
            return -1;
        }

        if (exists == 1)
        {
            strcpy(cwd, tmp_cwd);
            strcat(cwd, "/.neogit/");
            chdir(first_cwd);
            return 1;
        }

    } while (strcmp(tmp_cwd, "/") != 0);

    if (chdir(cwd) != 0)
    {
        chdir(first_cwd);
        return 0;
    }
    chdir(first_cwd);
    return -1;
}

int run_init(int argc, char *argv[])
{
    char cwd[MAX_ADDRESS_LENGHT];

    int find = find_neogit_dir(cwd);

    if (find == 1)
    {
        printf("neogit repository has already initialized\n");
        return 0;
    }

    if (find == -1)
    {
        strcat(cwd, "/.neogit/");

        if (mkdir(cwd, 0755) != 0)
        {
            return 0;
        }
        printf("create neogit\n");
        return make_neogit_dir(cwd);
    }

    printf("bug!\n");
    return 0;
}

int make_neogit_dir(char neogit_dir_address[])
{
    // make branch
    FILE *branch, *branches;

    char branch_address[MAX_ADDRESS_LENGHT];
    strcpy(branch_address, neogit_dir_address);
    strcat(branch_address, "branch");
    char branches_address[MAX_ADDRESS_LENGHT];
    strcpy(branches_address, neogit_dir_address);
    strcat(branches_address, "branches");

    branch = fopen(branch_address, "w");
    fputs("master\n", branch);
    branches = fopen(branches_address, "w");
    fputs("master\n", branches);
    fclose(branch);
    fclose(branches);

    // make stage
    char stage_address[MAX_ADDRESS_LENGHT];
    strcpy(stage_address, neogit_dir_address);
    strcat(stage_address, "stage/");
    if (mkdir(stage_address, 0755) != 0)
    {
        return 0;
    }
    // make add command for reset undo
    char add_address[MAX_ADDRESS_LENGHT];
    strcpy(add_address, neogit_dir_address);
    strcat(add_address, "add/");
    if (mkdir(add_address, 0755) != 0)
    {
        return 0;
    }
    char last_add_address[MAX_ADDRESS_LENGHT];
    strcpy(last_add_address, neogit_dir_address);
    strcat(last_add_address, "last_add");
    FILE *last_add_file = fopen(last_add_address, "w");
    fprintf(last_add_file, "%d\n", 0);

    // make commits
    char commits_files_address[MAX_ADDRESS_LENGHT];
    strcpy(commits_files_address, neogit_dir_address);
    strcat(commits_files_address, "commits_files/");
    if (mkdir(commits_files_address, 0755) != 0)
    {
        return 0;
    }
    char commits_data_address[MAX_ADDRESS_LENGHT];
    strcpy(commits_data_address, neogit_dir_address);
    strcat(commits_data_address, "commits_data/");
    if (mkdir(commits_data_address, 0755) != 0)
    {
        return 0;
    }
    char commits_shortcuts_address[MAX_ADDRESS_LENGHT];
    strcpy(commits_shortcuts_address, neogit_dir_address);
    strcat(commits_shortcuts_address, "commits_shortcuts/");
    if (mkdir(commits_shortcuts_address, 0755) != 0)
    {
        return 0;
    }
    char commits_message_address[MAX_ADDRESS_LENGHT];
    strcpy(commits_message_address, neogit_dir_address);
    strcat(commits_message_address, "commits_message/");
    if (mkdir(commits_message_address, 0755) != 0)
    {
        return 0;
    }
    char is_user_in_the_neogit_commits_message_dir[MAX_ADDRESS_LENGHT];
    strcpy(is_user_in_the_neogit_commits_message_dir, commits_message_address);
    strcat(is_user_in_the_neogit_commits_message_dir, "is_user_in_the_neogit_commits_message_dir");
    FILE *is_user_in_the_neogit_commits_message_dir_file = fopen(is_user_in_the_neogit_commits_message_dir, "w");

    char last_commit_id_address[MAX_ADDRESS_LENGHT];
    strcpy(last_commit_id_address, neogit_dir_address);
    strcat(last_commit_id_address, "last_commit_id");
    FILE *last_commit_id_file = fopen(last_commit_id_address, "w");
    fprintf(last_commit_id_file, "%d\n", 0);

    // make stage
    char tags_address[MAX_ADDRESS_LENGHT];
    strcpy(tags_address, neogit_dir_address);
    strcat(tags_address, "tags/");
    if (mkdir(tags_address, 0755) != 0)
    {
        return 0;
    }

    // make alias
    char alias_address[MAX_ADDRESS_LENGHT];
    strcpy(alias_address, neogit_dir_address);
    strcat(alias_address, "alias/");
    if (mkdir(alias_address, 0755) != 0)
    {
        return 0;
    }

    return 1;
}
