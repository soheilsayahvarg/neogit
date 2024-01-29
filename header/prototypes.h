#ifndef _PROTOTYPES_H
#define _PROTOTYPES_H

int about_neogit();

int run_config(int argc, char *argv[]);
int creat_config(char config_address[], char new_config_address[], char input1[], char input2[]);

int run_init(int argc, char *argv[]);

int run_add(int argc, char *argv[]);

int run_reset(int argc, char *argv[]);

int run_commit(int argc, char *argv[]);

int run_checkout(int argc, char *argv[]);

#endif