#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int find_neogit_dir(char cwd[])
{
    int exists = 0;

    if (getcwd(cwd, MAX_ADDRESS_LENGHT) == NULL)
    {
        printf("error getcwd\n");
        return 0;
    }

    char tmp_cwd[MAX_ADDRESS_LENGHT];
    struct dirent *entry;
    do
    {
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            printf("error opening current directory\n");
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
            return 0;
        }

        if (strcmp(tmp_cwd, "/") != 0)
        {
            if (chdir("..") != 0)
            {
                return 0;
            }
        }

        if (strcmp(tmp_cwd, "/") == 0)
        {
            return -1;
        }

        if (exists == 1)
        {
            strcpy(cwd, tmp_cwd);
            strcat(cwd, "/.neogit/");
            return 1;
        }

    } while (strcmp(tmp_cwd, "/") != 0);

    if (chdir(cwd) != 0)
    {
        return 0;
    }

    return -1;
}

int run_init(int argc, char *argv[])
{
    char cwd[MAX_ADDRESS_LENGHT];

    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        printf("error getcwd\n");
        return 0;
    }

    int find = find_neogit_dir(cwd);

    if (find == 1)
    {
        printf("neogit repository has already initialized\n");
        return 0;
    }

    if (find == -1)
    {
        strcat(cwd, "/.neogit");

        if (mkdir(cwd, 0755) != 0)
        {
            return 0;
        }
        printf("create neogit\n");
        return 1;
    }

    printf("bug!\n");
    return 0;
}