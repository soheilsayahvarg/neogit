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
    if (!strcmp(argv[1], "config"))
    {
        return run_config(argc, argv);
    }
    if (!strcmp(argv[1], "init"))
    {
        return run_init(argc, argv);
    }
    if (!strcmp(argv[1], "add"))
    {
        return run_add(argc, argv);
    }
    if (!strcmp(argv[1], "reset"))
    {
        return run_reset(argc, argv);
    }
    if (!strcmp(argv[1], "commit"))
    {
        return run_commit(argc, argv);
    }
    if (!strcmp(argv[1], "checkout"))
    {
        return run_checkout(argc, argv);
    }

    if (argc == 2)
    {
        return read_alias(argv[1]);
    }

    printf("invalid command.\n");
    return 0;
}