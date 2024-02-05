#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int run_grep(int argc, char *argv[])
{

    char neogit_dir_address[MAX_ADDRESS_LENGTH];
    if (find_neogit_dir(neogit_dir_address) != 1)
    {
        printf("not found neogit dir, first make a neogit dir with \"neogit init\"\n");
        return 0;
    }

    char file_address[MAX_ADDRESS_LENGTH];
    getcwd(file_address, sizeof(file_address));

    if (argc < 6)
    {
        printf("invalid input\n");
        return 0;
    }
    if (strcmp(argv[2], "-f") != 0 || strcmp(argv[4], "-p") != 0)
    {
        printf("invalid input\n");
        return 0;
    }
    strcat(file_address, "/");
    strcat(file_address, argv[3]);

    int show_number_of_line = 0;
    if (strcmp(argv[argc - 1], "-n") == 0)
    {
        show_number_of_line = 1;
    }

    int last_commit_id = 0;
    char last_commit_id_address[MAX_ADDRESS_LENGTH];
    strcpy(last_commit_id_address, neogit_dir_address);
    strcat(last_commit_id_address, "last_commit_id");
    FILE *last_commit_id_file = fopen(last_commit_id_address, "r");
    fscanf(last_commit_id_file, "%d", &last_commit_id);

    if (strcmp(argv[argc - 2 - show_number_of_line], "-c") == 0)
    {
        int commit_id = 0;
        sscanf(argv[argc - 1 - show_number_of_line], "commit %d", &commit_id);
        if (commit_id <= 0 || commit_id > last_commit_id)
        {
            printf("invalid commit id\n");
            return 0;
        }
        char file_address_in_commit[MAX_ADDRESS_LENGTH];
        strcpy(file_address_in_commit, neogit_dir_address);
        strcat(file_address_in_commit, "commits_files/commit ");
        char commit_number_string[MAX_NUMBERS_DIGITS];
        sprintf(commit_number_string, "%d", commit_id);
        strcat(file_address_in_commit, commit_number_string);
        strcat(file_address_in_commit, "/");

        strcat(file_address_in_commit, file_address + strlen(file_address_in_commit) - strlen(".neogit/commits_files/commit / ") - strlen(commit_number_string));
        strcpy(file_address, file_address_in_commit);
    }

    FILE *file = fopen(file_address, "r");
    if (file == NULL)
    {
        printf("not found file\n");
        return 0;
    }

    char line[MAX_LINE_IN_FILES_LENGTH];
    char word[MAX_WORD_IN_GREP_LENGTH];
    strcpy(word, argv[5]);

    int line_number = 1;
    while (fgets(line, sizeof(line), file))
    {
        strcpy(word, argv[5]);
        if (show_number_of_line)
        {
            printf("%d - ", line_number);
        }
        line_number++;
        find_word_and_mark(line, word);
        printf("%s", line);
    }
}

void find_and_replace(char string[], char find[], char replace[])
{
    int pointer = 0;
    while (strstr(string + pointer, find) != NULL)
    {
        pointer = strstr(string + pointer, find) - string;
        char tmp[strlen(string) + 1];
        strcpy(tmp, string + pointer + strlen(find));
        string[pointer] = '\0';
        strcat(string, replace);
        strcat(string, tmp);
        pointer += strlen(replace);
    }
}

void find_word_and_mark(char string[], char word[])
{
    char new_word[strlen(word) + strlen(RED) + strlen(RESET) + 2];

    strcat(word, " ");
    char character_end_of_word[] = {' ', '.', ',', ';', ':', '\n', '!', '?', '(', ')', '{', '}', '[', ']', '+', '-', '='};

    for (int i = 0; i < strlen(character_end_of_word); i++)
    {
        word[strlen(word) - 1] = character_end_of_word[i];
        strcpy(new_word, RED);
        strcat(new_word, word);
        strcat(new_word, RESET);
        find_and_replace(string, word, new_word);
    }
}
