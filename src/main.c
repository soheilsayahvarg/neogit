#include "includes.h"
#include "prototypes.h"
#include "defines.h"

#define TES
#ifdef TEST
int main()
{
    int argc = 1;
    char *argv[] = {"neogit"};

#else
int main(int argc, char *argv[])
{
#endif

    for (int i = 0; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("\n");

    if (argc == 1)
    {
        return about_neogit();
    }

    // config
    if (!strcmp(argv[1], "config"))
    {
        return run_config(argc, argv);
    }

    // init
    if (!strcmp(argv[1], "init"))
    {
        return run_init(argc, argv);
    }

    // add
    if (!strcmp(argv[1], "add"))
    {
        return run_add(argc, argv);
    }

    // reset
    if (!strcmp(argv[1], "reset"))
    {
        return run_reset(argc, argv);
    }

    // status
    if (!strcmp(argv[1], "status"))
    {
        return run_status(argc, argv);
    }

    // commit
    if (!strcmp(argv[1], "commit"))
    {
        return run_commit(argc, argv);
    }
    if (!strcmp(argv[1], "set"))
    {
        return run_set_message(argc, argv);
    }
    if (!strcmp(argv[1], "replace"))
    {
        return run_replace_message(argc, argv);
    }
    if (!strcmp(argv[1], "remove"))
    {
        return run_remove_message(argc, argv);
    }

    // log
    if (!strcmp(argv[1], "log"))
    {
        return run_log(argc, argv);
    }

    // branch
    if (!strcmp(argv[1], "branch"))
    {
        return run_branch(argc, argv);
    }

    // checkout
    if (!strcmp(argv[1], "checkout"))
    {
        return run_checkout(argc, argv);
    }

    // revert
    if (!strcmp(argv[1], "revert"))
    {
        return run_revert(argc, argv);
    }

    // tag
    if (!strcmp(argv[1], "tag"))
    {
        return run_tag(argc, argv);
    }

    // grep
    if (!strcmp(argv[1], "grep"))
    {
        return run_grep(argc, argv);
    }

    // alias
    if (argc == 2)
    {
        return read_alias(argv[1]);
    }

    // invalid command
    printf("invalid command.\n");
    return 0;
}
