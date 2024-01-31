#ifndef _PROTOTYPES_H
#define _PROTOTYPES_H

int main(int argc, char *argv[]);

int about_neogit();

int find_neogit_dir(char cwd[]);

int run_config(int argc, char *argv[]);
int creat_config(char config_address[], char new_config_address[], char input1[], char input2[]);
int creat_alias(char alias_address[], char input1[], char input2[]);
int read_alias(char command[]);

int run_init(int argc, char *argv[]);
int make_neogit_dir(char neogit_dir_address[]);

int run_add(int argc, char *argv[]);
int add_to_stage(char filepath[]);

int run_reset(int argc, char *argv[]);

int run_commit(int argc, char *argv[]);

int run_checkout(int argc, char *argv[]);

#endif