#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_config(int argc, char *argv[])
{

    if (strcmp(argv[2], "-global") == 0 && argc == 5)
    {
        char global_config_address[strlen(NEOGIT_GLOBAL_ADDRESS) + strlen("config") + 1];
        strcpy(global_config_address, NEOGIT_GLOBAL_ADDRESS);
        strcat(global_config_address, "config");
        char global_new_config_address[strlen(NEOGIT_GLOBAL_ADDRESS) + strlen("new_config") + 1];
        strcpy(global_new_config_address, NEOGIT_GLOBAL_ADDRESS);
        strcat(global_new_config_address, "new_config");

        if (strcmp(argv[3], "user.name") == 0 || strcmp(argv[3], "user.email") == 0)
        {
            return creat_config(global_config_address, global_new_config_address, argv[3], argv[4]);
        }
    }
    if (argc == 4)
    {

        if (strcmp(argv[2], "user.name") == 0 || strcmp(argv[2], "user.email") == 0)
        {
            return creat_config("edit this", "edit this", argv[2], argv[3]);
        }
        return 0;
    }
    printf("Invalid inputs.\n");
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
            printf("Sorry, your username cannot be \"nothing\"\n");
            return 0;
        }

        if (strlen(input1) >= MAX_USERNAME_LENGHT)
        {
            printf("Sorry, your username cannot be longer than %d characters.\n", MAX_USERNAME_LENGHT - 1);
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
            printf("Sorry, your useremail cannot be \"nothing\"\n");
            return 0;
        }

        if (strlen(input2) >= MAX_USEREMAIL_LENGTH)
        {
            printf("Sorry, your useremail cannot be longer than %d characters.\n", MAX_USEREMAIL_LENGTH - 1);
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

    printf("Invalid inputs.\n");
    return 0;
}