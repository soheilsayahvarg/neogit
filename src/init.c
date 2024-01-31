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
    fputs("master", branch);
    branches = fopen(branches_address, "w");
    fputs("master", branches);
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

    // make commits
    char commits_address[MAX_ADDRESS_LENGHT];
    strcpy(commits_address, neogit_dir_address);
    strcat(commits_address, "commits/");
    if (mkdir(commits_address, 0755) != 0)
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