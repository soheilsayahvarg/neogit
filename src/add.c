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

        return 1;
    }

    if (argc == 3)
    {
        return add_to_stage(argv[2]);
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

        return 1;
    }
}

int add_to_stage(char filepath[])
{
    /* it's not over yet.
    FILE *file = fopen(".neogit/staging", "r");
    if (file == NULL)
        return 0;
    char line[MAX_LINE_IN_FILES_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        int length = strlen(line);

        if (length > 0 && line[length - 1] == '\n')
        {
            line[length - 1] = '\0';
        }

        if (strcmp(filepath, line) == 0)
            return 0;
    }
    fclose(file);

    file = fopen(".neogit/staging", "a");
    if (file == NULL)
        return 0;

    fprintf(file, "%s\n", filepath);
    fclose(file);
    */
    return 1;
}
