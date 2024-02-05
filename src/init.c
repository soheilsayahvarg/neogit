#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int find_neogit_dir(char cwd[])
{
    if (getcwd(cwd, MAX_ADDRESS_LENGTH) == NULL)
    {
        printf("error getcwd\n");
        return 0;
    }

    char first_cwd[MAX_ADDRESS_LENGTH];
    strcpy(first_cwd, cwd);

    int exists = 0;

    char tmp_cwd[MAX_ADDRESS_LENGTH];
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
    char cwd[MAX_ADDRESS_LENGTH];

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

    char branch_address[MAX_ADDRESS_LENGTH];
    strcpy(branch_address, neogit_dir_address);
    strcat(branch_address, "branch");
    char branches_address[MAX_ADDRESS_LENGTH];
    strcpy(branches_address, neogit_dir_address);
    strcat(branches_address, "branches");

    branch = fopen(branch_address, "w");
    fputs("master\n", branch);
    branches = fopen(branches_address, "w");
    fputs("master\n", branches);
    fclose(branch);
    fclose(branches);

    // make stage
    char stage_address[MAX_ADDRESS_LENGTH];
    strcpy(stage_address, neogit_dir_address);
    strcat(stage_address, "stage/");
    if (mkdir(stage_address, 0755) != 0)
    {
        return 0;
    }

    // make add command for reset undo
    char add_address[MAX_ADDRESS_LENGTH];
    strcpy(add_address, neogit_dir_address);
    strcat(add_address, "add/");
    if (mkdir(add_address, 0755) != 0)
    {
        return 0;
    }
    char last_add_address[MAX_ADDRESS_LENGTH];
    strcpy(last_add_address, neogit_dir_address);
    strcat(last_add_address, "last_add");
    FILE *last_add_file = fopen(last_add_address, "w");
    fprintf(last_add_file, "%d\n", 0);
    fclose(last_add_file);

    // make commits
    char commits_files_address[MAX_ADDRESS_LENGTH];
    strcpy(commits_files_address, neogit_dir_address);
    strcat(commits_files_address, "commits_files/");
    if (mkdir(commits_files_address, 0755) != 0)
    {
        return 0;
    }
    char commits_data_address[MAX_ADDRESS_LENGTH];
    strcpy(commits_data_address, neogit_dir_address);
    strcat(commits_data_address, "commits_data/");
    if (mkdir(commits_data_address, 0755) != 0)
    {
        return 0;
    }
    char commits_shortcuts_address[MAX_ADDRESS_LENGTH];
    strcpy(commits_shortcuts_address, neogit_dir_address);
    strcat(commits_shortcuts_address, "commits_shortcuts/");
    if (mkdir(commits_shortcuts_address, 0755) != 0)
    {
        return 0;
    }
    char commits_message_address[MAX_ADDRESS_LENGTH];
    strcpy(commits_message_address, neogit_dir_address);
    strcat(commits_message_address, "commits_message/");
    if (mkdir(commits_message_address, 0755) != 0)
    {
        return 0;
    }
    char is_user_in_the_neogit_commits_message_dir[MAX_ADDRESS_LENGTH];
    strcpy(is_user_in_the_neogit_commits_message_dir, commits_message_address);
    strcat(is_user_in_the_neogit_commits_message_dir, "is_user_in_the_neogit_commits_message_dir");
    FILE *is_user_in_the_neogit_commits_message_dir_file = fopen(is_user_in_the_neogit_commits_message_dir, "w");
    fclose(is_user_in_the_neogit_commits_message_dir_file);

    char last_commit_id_address[MAX_ADDRESS_LENGTH];
    strcpy(last_commit_id_address, neogit_dir_address);
    strcat(last_commit_id_address, "last_commit_id");
    FILE *last_commit_id_file = fopen(last_commit_id_address, "w");
    fprintf(last_commit_id_file, "%d\n", 0);
    fclose(last_commit_id_file);

    // make user is in HEAD
    char user_is_in_HEAD_address[MAX_ADDRESS_LENGTH];
    strcpy(user_is_in_HEAD_address, neogit_dir_address);
    strcat(user_is_in_HEAD_address, "user is in HEAD");
    FILE *user_is_in_HEAD_file = fopen(user_is_in_HEAD_address, "w");
    fclose(user_is_in_HEAD_file);

    // make tags
    char tags_address[MAX_ADDRESS_LENGTH];
    strcpy(tags_address, neogit_dir_address);
    strcat(tags_address, "tags/");
    if (mkdir(tags_address, 0755) != 0)
    {
        return 0;
    }

    // make pre-commit
    char pre_commit_address[MAX_ADDRESS_LENGTH];
    strcpy(pre_commit_address, neogit_dir_address);
    strcat(pre_commit_address, "pre-commit/");
    if (mkdir(pre_commit_address, 0755) != 0)
    {
        return 0;
    }

    char hooks_list_address[MAX_ADDRESS_LENGTH];
    strcpy(hooks_list_address, neogit_dir_address);
    strcat(hooks_list_address, "pre-commit/hooks list");
    FILE *hooks_list_file = fopen(hooks_list_address, "w");
    fprintf(hooks_list_file, "todo-check\neof-blank-space\nformat-check\nbalance-braces\nindentation-check\nstatic-error-check\nfile-size-check\ncharacter-limit\ntime-limit\n");
    fclose(hooks_list_file);

    char applied_hooks_address[MAX_ADDRESS_LENGTH];
    strcpy(applied_hooks_address, neogit_dir_address);
    strcat(applied_hooks_address, "pre-commit/applied hooks");
    FILE *applied_hooks_file = fopen(applied_hooks_address, "w");
    fclose(applied_hooks_file);

    char all_format_address[MAX_ADDRESS_LENGTH];
    strcpy(all_format_address, neogit_dir_address);
    strcat(all_format_address, "pre-commit/all format");
    FILE *all_format_file = fopen(all_format_address, "w");
    fprintf(all_format_file, ".txt .c .cpp .mp3 .mp4 .wav .exe\n");
    fclose(all_format_file);

    // make alias
    char alias_address[MAX_ADDRESS_LENGTH];
    strcpy(alias_address, neogit_dir_address);
    strcat(alias_address, "alias/");
    if (mkdir(alias_address, 0755) != 0)
    {
        return 0;
    }

    return 1;
}
