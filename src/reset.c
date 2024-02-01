#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_reset(int argc, char *argv[])
{
    if (argc == 2)
    {
        printf("please specify a file\n");
        return 0;
    }

    if (!strcmp(argv[2], "-undo"))
    {
        if (argc != 3)
        {
            printf("invalid input\n");
            return 0;
        }

        return reset_undo();
    }

    if (!strcmp(argv[2], "-f"))
    {

        for (int i = 3; i < argc; i++)
        {
            reset_in_stage(argv[i]);
        }
        return 1;
    }

    for (int i = 2; i < argc; i++)
    {
        reset_in_stage(argv[i]);
    }

    return 1;
}

int reset_in_stage(char file_name[])
{
    struct dirent *entry;
    DIR *dir = opendir(".");
    if (dir == NULL)
    {
        printf("error opening current directory\n");
        return 0;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (!strcmp(entry->d_name, file_name))
        {
            // delete file
            char file_address[MAX_ADDRESS_LENGHT];
            getcwd(file_address, sizeof(file_address));
            strcat(file_address, "/");
            strcat(file_address, file_name);

            char neogit_dir_address[MAX_ADDRESS_LENGHT];

            if (find_neogit_dir(neogit_dir_address) != 1)
            {
                printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
                return 0;
            }

            int len = strlen(neogit_dir_address) - strlen(".neogit/");
            strcat(neogit_dir_address, "stage/");

            for (int i = len; i < strlen(file_address); i++)
            {
                int lenght = strlen(neogit_dir_address);
                neogit_dir_address[lenght] = file_address[i];
                neogit_dir_address[lenght + 1] = '\0';
            }

            char command[MAX_BASH_COMMAND];

            if (entry->d_type == 4)
            {
                strcpy(command, "rm -rf \"");
            }
            else
            {
                strcpy(command, "rm -f \"");
            }
            strcat(command, neogit_dir_address);
            strcat(command, "\"");

            if (!system(command))
            {
                printf("reset %s in stage\n", file_name);
                return 1;
            }
            return 0;
        }
    }
    printf("not found \"%s\"\n", file_name);
    return 0;
}
// TODO
int reset_undo()
{
    return 1;
}