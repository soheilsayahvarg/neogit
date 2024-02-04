#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_revert(int argc, char *argv[])
{

    char neogit_dir_address[MAX_ADDRESS_LENGTH];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }

    char user_is_in_HEAD_address[MAX_ADDRESS_LENGTH];
    strcpy(user_is_in_HEAD_address, neogit_dir_address);
    strcat(user_is_in_HEAD_address, "user is in HEAD");
    FILE *user_is_in_HEAD_file;
    int user_is_in_HEAD = ((user_is_in_HEAD_file = fopen(user_is_in_HEAD_address, "r")) != NULL);
    user_is_in_HEAD_file = fopen(user_is_in_HEAD_address, "w");

    int last_commit_id = 0;
    char last_commit_id_address[MAX_ADDRESS_LENGTH];
    strcpy(last_commit_id_address, neogit_dir_address);
    strcat(last_commit_id_address, "last_commit_id");
    FILE *last_commit_id_file = fopen(last_commit_id_address, "r");
    fscanf(last_commit_id_file, "%d", &last_commit_id);
    fclose(last_commit_id_file);

    if (argc == 2 || argc > 5)
    {
        printf("invalid input\n");
        return 0;
    }

    if (!strcmp(argv[2], "-n"))
    {
        if (argc == 3)
        {
            remove(user_is_in_HEAD_address);
            checkout_to_commit(last_commit_id);
            user_is_in_HEAD_file = fopen(user_is_in_HEAD_address, "w");
            fclose(user_is_in_HEAD_file);
            return 1;
        }
        if (argc == 4)
        {
            if (!strncmp(argv[3], "commit ", strlen("commit ")))
            {
                int commit_number = 0;
                sscanf(argv[3], "commit %d", &commit_number);
                if (commit_number <= 0 || commit_number > last_commit_id)
                {
                    printf("invalid commit number\n");
                    return 0;
                }
                remove(user_is_in_HEAD_address);

                checkout_to_commit(commit_number);

                user_is_in_HEAD_file = fopen(user_is_in_HEAD_address, "w");
                fclose(user_is_in_HEAD_file);
                return 1;
            }
        }

        printf("invalid input\n");
        return 0;
    }

    if (argc == 3)
    {
        remove(user_is_in_HEAD_address);
        int commit_number = run_checkout(argc, argv);
        if (commit_number == 0)
        {
            printf("can't checkout\n");
            return 0;
        }
        // read message
        char commit_data_address[MAX_ADDRESS_LENGTH];
        strcpy(commit_data_address, neogit_dir_address);
        strcat(commit_data_address, "commits_data/commit ");
        char commit_number_string[MAX_NUMBERS_DIGITS];
        sprintf(commit_number_string, "%d", commit_number);
        strcat(commit_data_address, commit_number_string);

        FILE *commit_data_file = fopen(commit_data_address, "r");
        char line_in_commit_data[MAX_LINE_IN_FILES_LENGTH];
        fgets(line_in_commit_data, sizeof(line_in_commit_data), commit_data_file);
        fgets(line_in_commit_data, sizeof(line_in_commit_data), commit_data_file);
        fgets(line_in_commit_data, sizeof(line_in_commit_data), commit_data_file);
        if (line_in_commit_data[strlen(line_in_commit_data) - 1] == '\n')
        {
            line_in_commit_data[strlen(line_in_commit_data) - 1] = '\0';
        }
        memmove(line_in_commit_data, line_in_commit_data + strlen("message : "), strlen(line_in_commit_data) - strlen("message : ") + 1);

        // add and commit files
        user_is_in_HEAD_file = fopen(user_is_in_HEAD_address, "w");
        fclose(user_is_in_HEAD_file);

        system("neogit add *");

        // TODO: don't copy old commit in new commit
        return creat_commit(line_in_commit_data);
    }

    if (argc == 5 && strcmp(argv[2], "-m") == 0)
    {
        if (strlen(argv[3]) > MAX_COMMIT_MESSAGE_LENGTH)
        {
            printf("max commit message is %d characters\n", MAX_COMMIT_MESSAGE_LENGTH);
            return 0;
        }
        char message[MAX_COMMIT_MESSAGE_LENGTH + 1];
        strcpy(message, argv[3]);

        remove(user_is_in_HEAD_address);
        strcpy(argv[2], argv[4]);
        if (run_checkout(argc - 2, argv) == 0)
        {
            printf("can't checkout\n");
            if (user_is_in_HEAD)
            {
                user_is_in_HEAD_file = fopen(user_is_in_HEAD_address, "w");
                fclose(user_is_in_HEAD_file);
            }
            return 0;
        }
        user_is_in_HEAD_file = fopen(user_is_in_HEAD_address, "w");
        fclose(user_is_in_HEAD_file);

        system("neogit add *");

        // TODO: don't copy old commit in new commit
        return creat_commit(message);
    }

    printf("invalid input\n");
    return 0;
}
