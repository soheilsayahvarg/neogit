#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int find_neogit_dir(char cwd[])
{
    int exists = 0;

    if (getcwd(cwd, MAX_ADDRESS_LENGHT) == NULL)
    {
        printf("Error getcwd\n");
        return 0;
    }

    char tmp_cwd[MAX_ADDRESS_LENGHT];
    struct dirent *entry;
    do
    {
        DIR *dir = opendir(".");
        if (dir == NULL)
        {
            printf("Error opening current directory\n");
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
            printf("Error getcwd\n");
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
    return 1;
}