#ifndef _PROTOTYPES_H
#define _PROTOTYPES_H

int main(int argc, char *argv[]);

int about_neogit();

int find_neogit_dir(char cwd[]);

// time
int comprator_time(char date1_string[], char date2_string[]);

// config
int run_config(int argc, char *argv[]);
int creat_config(char config_address[], char new_config_address[], char input1[], char input2[]);
int read_user_config(char username[], char useremail[]);

// init
int run_init(int argc, char *argv[]);
int make_neogit_dir(char neogit_dir_address[]);

// add
int run_add(int argc, char *argv[]);
int add_to_stage(char file_name[]);
int add_redo();
int add_n(int depth, int number_of_tab);
int save_add_command(int argc, char *argv[]);

// reset
int run_reset(int argc, char *argv[]);
int reset_in_stage(char file_name[]);
int reset_undo();

// status
int run_status(int argc, char *argv[]);
int check_status(char repository_address[], char stage_address[], char commit_address[]);
int compare_file(char file_path_1[], char file_path_2[]);

// commit
int run_commit(int argc, char *argv[]);
int creat_commit(char message[]);
int run_set_message(int argc, char *argv[]);
int run_replace_message(int argc, char *argv[]);
int run_remove_message(int argc, char *argv[]);

// log
int run_log(int argc, char *argv[]);
int show_commit_data(int number_of_commit);

// branch
int run_branch(int argc, char *argv[]);
int add_branch(char branch_name[], char neogit_dir_address[]);
int read_branch(char branch_name[]);

// checkout
int run_checkout(int argc, char *argv[]);
int checkout_to_commit(int commit_number);

// tag
int run_tag(int argc, char *argv[]);

// alias
int creat_alias(char alias_address[], char input1[], char input2[]);
int read_alias(char command[]);

#endif