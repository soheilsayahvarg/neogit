#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_add(int argc, char *argv[])
{

    if (argc == 2)
    {
        printf("please specify a file\n");
        return 0;
    }

    if (!strcmp(argv[2], "-redo"))
    {
        if (argc != 3)
        {
            printf("invalid input\n");
            return 0;
        }

        return add_redo();
    }

    if (!strcmp(argv[2], "-f"))
    {

        for (int i = 3; i < argc; i++)
        {
            add_to_stage(argv[i]);
        }
        return 1;
    }

    if (!strcmp(argv[2], "-n"))
    {
        if (argc != 4)
        {
            printf("invalid input\n");
            return 0;
        }

        int depth = 0;
        sscanf(argv[3], "%d", &depth);

        return add_n(depth, 0);
    }

    for (int i = 2; i < argc; i++)
    {
        add_to_stage(argv[i]);
    }

    return 1;
}

int add_to_stage(char file_name[])
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
            // copy file
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

            char all_stage_address[MAX_ADDRESS_LENGHT];
            strcpy(all_stage_address, neogit_dir_address);
            strcat(all_stage_address, "all_stage");

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
                strcpy(command, "cp -r \"");
            }
            else
            {
                strcpy(command, "cp \"");
            }
            strcat(command, file_address);
            strcat(command, "\" \"");
            strcat(command, neogit_dir_address);

            if (entry->d_type == 4)
            {
                while (command[strlen(command) - 1] != '/')
                {
                    command[strlen(command) - 1] = '\0';
                }
                command[strlen(command) - 1] = '\0';
            }

            strcat(command, "\"");

            if (!system(command))
            {
                printf("add %s to stage\n", file_name);

                FILE *all_stage_file;
                all_stage_file = fopen(all_stage_address, "a");
                fprintf(all_stage_file, "%s\n", command);

                return 1;
            }
            return 0;
        }
    }
    printf("not found \"%s\"\n", file_name);
    return 0;
}

int add_redo()
{
    char neogit_dir_address[MAX_ADDRESS_LENGHT];
    if (find_neogit_dir(neogit_dir_address) == 1)
    {
        char all_stage_address[MAX_ADDRESS_LENGHT];
        strcpy(all_stage_address, neogit_dir_address);
        strcat(all_stage_address, "all_stage");

        FILE *all_stage_file;
        if ((all_stage_file = fopen(all_stage_address, "r")) == NULL)
        {
            printf("nothing add to stage\n");
            return 0;
        }

        char command[MAX_BASH_COMMAND];
        while (fgets(command, sizeof(command), all_stage_file))
        {
            if (command[strlen(command) - 1] == '\n')
            {
                command[strlen(command) - 1] == '\0';
            }
            system(command);
        }
        printf("redo all file\n");
        return 1;
    }

    printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
    return 0;
}

int add_n(int depth, int number_of_tab)
{
    struct dirent *entry;
    char first_cwd[MAX_ADDRESS_LENGHT];
    if (getcwd(first_cwd, MAX_ADDRESS_LENGHT) == NULL)
    {
        printf("error getcwd\n");
        return 0;
    }

    if (depth <= 0)
    {
        printf("invalid input\n");
        chdir(first_cwd);
        return 0;
    }

    DIR *dir = opendir(".");
    if (dir == NULL)
    {
        printf("error opening current directory\n");
        chdir(first_cwd);
        return 0;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".neogit") == 0)
        {
            continue;
        }

        char file_address[MAX_ADDRESS_LENGHT];
        getcwd(file_address, sizeof(file_address));
        strcat(file_address, "/");
        strcat(file_address, entry->d_name);

        char neogit_dir_address[MAX_ADDRESS_LENGHT];
        if (find_neogit_dir(neogit_dir_address) != 1)
        {
            printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
            chdir(first_cwd);
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
        for (int i = 0; i < number_of_tab; i++)
        {
            printf("\t");
        }
        if (entry->d_type == 8)
        {
            FILE *file;

            printf("\"%s\" : ", entry->d_name);
            if ((file = fopen(neogit_dir_address, "r")) == NULL)
            {
                printf("-\n");
            }
            else
            {
                printf("+\n");
            }
        }
        else
        {
            if (depth == 1)
            {
                DIR *dir;
                printf("\"%s\" : ", entry->d_name);
                if ((dir = opendir(neogit_dir_address)) == NULL)
                {
                    printf("-\n");
                }
                else
                {
                    printf("+\n");
                }
            }
            else
            {
                printf("\"%s\":\n", entry->d_name);
                chdir(entry->d_name);
                add_n(depth - 1, number_of_tab + 1);
                chdir(first_cwd);
            }
        }
        chdir(first_cwd);
    }
}
