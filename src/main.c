#include "includes.h"
#include "prototypes.h"
#include "defines.h"

int main(int argc, char *argv[])
{
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
    printf("Invalid command.");
    return 0;
}