#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_config(int argc, char *argv[])
{

    if (strcmp(argv[2], "-global") == 0 && argc == 5)
    {

        if (strcmp(argv[3], "user.name") == 0 || strcmp(argv[3], "user.email") == 0)
        {
            char global_config_address[MAX_ADDRESS_LENGHT];
            strcpy(global_config_address, NEOGIT_GLOBAL_ADDRESS);
            strcat(global_config_address, "config");
            char global_new_config_address[MAX_ADDRESS_LENGHT];
            strcpy(global_new_config_address, NEOGIT_GLOBAL_ADDRESS);
            strcat(global_new_config_address, "new_config");
            return creat_config(global_config_address, global_new_config_address, argv[3], argv[4]);
        }

        if (!strncmp(argv[3], "alias.", 6))
        {
            char global_alias_address[MAX_ADDRESS_LENGHT] = NEOGIT_GLOBAL_ADDRESS;
            strcat(global_alias_address, "alias/");
            return creat_alias(global_alias_address, argv[3], argv[4]);
        }

        printf("invalid input\n");
        return 0;
    }
    if (argc == 4)
    {

        if (strcmp(argv[2], "user.name") == 0 || strcmp(argv[2], "user.email") == 0)
        {
            char neogit_dir_address[MAX_ADDRESS_LENGHT];
            if (find_neogit_dir(neogit_dir_address) == 1)
            {
                char config_address[MAX_ADDRESS_LENGHT];
                strcpy(config_address, neogit_dir_address);
                strcat(config_address, "config");
                char new_config_address[MAX_ADDRESS_LENGHT];
                strcpy(new_config_address, neogit_dir_address);
                strcat(new_config_address, "new_config");

                return creat_config(config_address, new_config_address, argv[2], argv[3]);
            }
            printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
            return 0;
        }

        if (!strncmp(argv[2], "alias.", 6))
        {
            char neogit_dir_address[MAX_ADDRESS_LENGHT];
            if (find_neogit_dir(neogit_dir_address) == 1)
            {
                char alias_address[MAX_ADDRESS_LENGHT];
                strcpy(alias_address, neogit_dir_address);
                strcat(alias_address, "alias/");
                return creat_alias(alias_address, argv[2], argv[3]);
            }
            printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
            return 0;
        }

        printf("invalid inputs.\n");
        return 0;
    }

    printf("invalid inputs.\n");
    return 0;
}

int creat_config(char config_address[], char new_config_address[], char input1[], char input2[])
{
    FILE *config, *new_config;
    char old_username[MAX_USERNAME_LENGHT], old_useremail[MAX_USEREMAIL_LENGTH];
    char line_in_config[MAX_LINE_IN_FILES_LENGTH];

    if (!strcmp(input1, "user.name"))
    {
        if (!strcmp(input2, "nothing"))
        {
            printf("sorry, your username cannot be \"nothing\"\n");
            return 0;
        }

        if (strlen(input1) >= MAX_USERNAME_LENGHT)
        {
            printf("sorry, your username cannot be longer than %d characters.\n", MAX_USERNAME_LENGHT - 1);
            return 0;
        }

        config = fopen(config_address, "r");
        if (config == NULL)
        {
            config = fopen(config_address, "w");
            fprintf(config, "username : %s\nuseremail : nothing", input2);
            fclose(config);
            printf("global username set : \"%s\"\n", input2);
            return 1;
        }

        new_config = fopen(new_config_address, "w");
        fgets(line_in_config, sizeof(line_in_config), config);
        fgets(line_in_config, sizeof(line_in_config), config);

        sscanf(line_in_config, "useremail : %s", old_useremail);

        fprintf(new_config, "username : %s\nuseremail : %s", input2, old_useremail);

        fclose(config);
        fclose(new_config);

        remove(config_address);
        rename(new_config_address, config_address);

        printf("global username set : \"%s\"\n", input2);
        return 1;
    }

    if (!strcmp(input1, "user.email"))
    {
        if (!strcmp(input2, "nothing"))
        {
            printf("sorry, your useremail cannot be \"nothing\"\n");
            return 0;
        }

        if (strlen(input2) >= MAX_USEREMAIL_LENGTH)
        {
            printf("sorry, your useremail cannot be longer than %d characters.\n", MAX_USEREMAIL_LENGTH - 1);
            return 0;
        }

        config = fopen(config_address, "r");
        if (config == NULL)
        {
            config = fopen(config_address, "w");
            fprintf(config, "username : nothing\nuseremail : %s", input2);
            fclose(config);
            printf("global useremail set : \"%s\"\n", input2);
            return 1;
        }

        new_config = fopen(new_config_address, "w");
        fgets(line_in_config, sizeof(line_in_config), config);

        sscanf(line_in_config, "username : %s", old_username);

        fprintf(new_config, "username : %s\nuseremail : %s", old_username, input2);

        fclose(config);
        fclose(new_config);

        remove(config_address);
        rename(new_config_address, config_address);

        printf("global useremail set : \"%s\"\n", input2);
        return 1;
    }

    printf("invalid inputs.\n");
    return 0;
}

int creat_alias(char alias_address[], char input1[], char input2[])
{

    char alias_name[strlen(input1) - strlen("alias.") + 1];
    sscanf(input1 + strlen("alias."), "%s", alias_name);
    strcat(alias_address, alias_name);

    FILE *alias_file;

    alias_file = fopen(alias_address, "w");

    int pointer = 0;
    char string[MAX_COMMAND_LENGHT];

    while (pointer < strlen(input2))
    {
        while (input2[pointer] == ' ')
        {
            pointer++;
        }

        sscanf(input2 + pointer, "%s", string);
        pointer += strlen(string);
        fprintf(alias_file, "%s\n", string);
    }

    fclose(alias_file);
    printf("set \"%s\" command to \"%s\"\n", alias_name, input2);
    return 1;
}

// TODO
int read_alias(char command[])
{
    FILE *alias_file;

    // check global alias
    char global_alias_address[MAX_ADDRESS_LENGHT] = NEOGIT_GLOBAL_ADDRESS;
    strcat(global_alias_address, "alias/");
    strcat(global_alias_address, command);

    alias_file = fopen(global_alias_address, "r");
    if (alias_file != NULL)
    {
        printf("found command in global alias\n");

        char string[MAX_COMMAND_LENGHT];

        int argc = 1;
        char argv[MAX_NUMBER_OF_COMMNAD][MAX_COMMAND_LENGHT];

        while (fgets(string, sizeof(string), alias_file) != NULL)
        {
            if (string[strlen(string) - 1] == '\n')
            {
                string[strlen(string) - 1] = '\0';
            }
            strcpy(argv[argc], string);
            argc++;
        }
        return 1;
    }

    // check alias
    char neogit_dir_address[MAX_ADDRESS_LENGHT];
    if (find_neogit_dir(neogit_dir_address) == 1)
    {
        char alias_address[MAX_ADDRESS_LENGHT];
        strcpy(alias_address, neogit_dir_address);
        strcat(alias_address, "alias/");
        strcat(alias_address, command);

        alias_file = fopen(alias_address, "r");
        if (alias_file != NULL)
        {
            printf("found command in alias\n");

            char string[MAX_COMMAND_LENGHT];

            int argc = 1;
            char argv[MAX_NUMBER_OF_COMMNAD][MAX_COMMAND_LENGHT];

            while (fgets(string, sizeof(string), alias_file) != NULL)
            {
                if (string[strlen(string) - 1] == '\n')
                {
                    string[strlen(string) - 1] = '\0';
                }
                strcpy(argv[argc], string);
                argc++;
            }
            return 1;
        }

        printf("invalid command\n");
        return 0;
    }
    printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
    return 0;
}