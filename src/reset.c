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
            char file_address[MAX_ADDRESS_LENGTH];
            getcwd(file_address, sizeof(file_address));
            strcat(file_address, "/");
            strcat(file_address, file_name);

            char neogit_dir_address[MAX_ADDRESS_LENGTH];
            if (find_neogit_dir(neogit_dir_address) != 1)
            {
                printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
                return 0;
            }

            int len = strlen(neogit_dir_address) - strlen(".neogit/");
            strcat(neogit_dir_address, "stage/");

            for (int i = len; i < strlen(file_address); i++)
            {
                int LENGTH = strlen(neogit_dir_address);
                neogit_dir_address[LENGTH] = file_address[i];
                neogit_dir_address[LENGTH + 1] = '\0';
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

int reset_undo()
{
    char neogit_dir_address[MAX_ADDRESS_LENGTH];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }

    char last_add_address[MAX_ADDRESS_LENGTH];
    strcpy(last_add_address, neogit_dir_address);
    strcat(last_add_address, "last_add");
    FILE *last_add_file = fopen(last_add_address, "r");
    int add_number = 0;
    fscanf(last_add_file, "%d", &add_number);
    if (add_number <= 0)
    {
        printf("cannot reset undo\n");
        return 0;
    }
    fclose(last_add_file);
    last_add_file = fopen(last_add_address, "w");
    fprintf(last_add_file, "%d\n", add_number - 1);

    char add_address[MAX_ADDRESS_LENGTH];
    strcpy(add_address, neogit_dir_address);
    strcat(add_address, "add/add ");
    char add_number_string[MAX_NUMBERS_DIGITS];
    sprintf(add_number_string, "%d", add_number);
    strcat(add_address, add_number_string);

    FILE *add_file = fopen(add_address, "r");

    char command[MAX_BASH_COMMAND];
    fgets(command, sizeof(command), add_file);
    fclose(add_file);
    remove(add_address);
    printf("reset undo\n");
    system(command);
    return 1;
}
