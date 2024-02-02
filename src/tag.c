#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_tag(int argc, char *argv[])
{
    FILE *tag_file;

    char neogit_dir_address[MAX_ADDRESS_LENGHT];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }

    // show all tags
    // TODO
    if (argc == 2)
    {
        return 1;
    }

    char tag_address[MAX_ADDRESS_LENGHT];
    strcpy(tag_address, neogit_dir_address);
    strcat(tag_address, "/tags/");
    strcat(tag_address, argv[3]);

    // show tags data
    if (argc == 4 && strcmp(argv[2], "show") == 0)
    {
        if ((tag_file = fopen(tag_address, "r")) == NULL)
        {
            printf("not found tag name\n");
            return 0;
        }
        char line_in_tag_file[MAX_LINE_IN_FILES_LENGTH];
        while (fgets(line_in_tag_file, sizeof(line_in_tag_file), tag_file) != NULL)
        {
            printf("%s", line_in_tag_file);
        }
        return 1;
    }

    // ger username useremail
    char username[MAX_USERNAME_LENGHT], useremail[MAX_USEREMAIL_LENGTH];
    if (read_user_config(username, useremail) != 1)
    {
        printf("first set username and useremail with neogit config user.mame or user.email\n");
        return 0;
    }
    if (argc == 3 || argc > 9)
    {
        printf("invalid input\n");
        return 0;
    }
    if (((argc & 1) == 1) && (strcmp(argv[argc - 1], "-f") != 0))
    {
        printf("invalid input\n");
        return 0;
    }

    if (strcmp(argv[2], "-a"))
    {
        printf("invalid input\n");
        return 0;
    }

    if (((tag_file = fopen(tag_address, "r")) != NULL) && (strcmp(argv[argc - 1], "-f") != 0))
    {
        printf("tag does already exist\n");
        return 0;
    }

    int tag_commit_id = 0;
    char last_commit_id_address[MAX_ADDRESS_LENGHT];
    strcpy(last_commit_id_address, neogit_dir_address);
    strcat(last_commit_id_address, "last_commit_id");
    FILE *last_commit_id_file = fopen(last_commit_id_address, "r");
    fscanf(last_commit_id_file, "%d", &tag_commit_id);

    if (strcmp(argv[argc - 3], "-c") == 0)
    {
        int commit_id = 0;
        sscanf(argv[argc - 2], "%d", &commit_id);
        if (commit_id > tag_commit_id || commit_id <= 0)
        {
            printf("invalid commit\n");
            return 0;
        }
        tag_commit_id = commit_id;
    }
    if (strcmp(argv[argc - 2], "-c") == 0)
    {
        int commit_id = 0;
        sscanf(argv[argc - 1], "%d", &commit_id);
        if (commit_id > tag_commit_id || commit_id <= 0)
        {
            printf("invalid commit\n");
            return 0;
        }
        tag_commit_id = commit_id;
    }

    tag_file = fopen(tag_address, "w");
    fprintf(tag_file, "tag %s\n", argv[3]);
    fprintf(tag_file, "commit %d\n", tag_commit_id);
    fprintf(tag_file, "Author: %s <%s>\n", username, useremail);
    time_t now = time(NULL);
    fprintf(tag_file, "Date : %s\n", asctime(localtime(&now)));

    if (!strcmp(argv[4], "-m"))
    {
        fprintf(tag_file, "Message: %s\n", argv[5]);
    }
    printf("create tag %s\n", argv[3]);
    return 1;
}
