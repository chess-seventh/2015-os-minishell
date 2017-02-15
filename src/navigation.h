/**
 * @file navigation.h
 *
 * Functions for navigation in Shell.
 *
 * @author Frederick Ney
 * @author Chess Seventh
 */

#include "function.h"

#ifndef __NAVIGATION_H_INCLUDE__
#define __NAVIGATION_H_INCLUDE__



 /**
 * Command to go to directory "CD".
 *
 * @return 0 if we successfully changed the directory, 1 if error.
 */
int cmd_cd(char *path);


 /**
 * Command to print the current directory.
 *
 */
char *cmd_pwd();

 /**
 * Find an ALIAS .
 *
 * @param input name of alias to find
 * @param structure of current alias
 * @return pointer on structure that contains the alias.
 */
alias_t *find_alias(char *alias_name, alias_t *alias);



 /**
 * Find an ALIAS .
 *
 * @param input name of alias to find
 * @param structure of current alias
 * @return the structure that contains the alias.
 */
char *get_alias_value(alias_t *current);



 /**
 * Add an ALIAS .
 *
 * @param input alias name
 * @param input alias value
 * @param structure of current alias
 * @return the structure that contains the alias.
 */
int add_alias(char *alias_name, char *alias_value, alias_t *current);

 /**
 * Delete an ALIAS .
 *
 * @param input alias name
 * @param structure of current alias
 * @return int to define error.
 */
int delete_alias(char *alias_name, alias_t *current);

/**
 * Parse the command ALIAS .
 *
 * @param char* input command from user
 * @param current structure of aliases
 *
 * @return int 0 is sucess
 *
 */
int parse_cmdALIAS(exe_t *e, alias_t *alias_current);

 /**
  * Parse the command LS .
  *
  * @param path_name pointer to a directory [optional].
  * default value is the current directory.
  */
 void cmd_ls (char *path_name);


 /**
  * Display usage of the Alias command. .
  *
  */
void usageAlias();

/**
 * List all alias.
 * 
 *
 * @param alias_t structure to output all aliases in memory.
 *
 * @return void - nothing it prints all values directly in shell.
 */
void list_alias(alias_t *alias);


#endif //  __NAVIGATION_H_INCLUDE__