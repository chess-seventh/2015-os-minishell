h/**
 * @file parse.h
 *
 * Functions that are used to execute a command.
 *
 * @author Frederick Ney
 * @author Chess Seventh
 */

#include <stdio.h>
#include <string.h>
 #include <stdbool.h>
#include "function.h"

#ifndef __PARSE_H_INCLUDE__
#define __PARSE_H_INCLUDE__

/**
 * Parser to see there is a symbol pipe sup or inf in the command
 *
 * @param cmd pointer to the command 
 *
 * @return 0 if we found it in the cmd.
 * @return 1 if we did not find it.
 */
int symbol_find(string_t *s, char *cmd);

/**
 * Function that looks at the position of where the symbol is 
 *
 * @param cmd pointer to the command 
 *
 * @return the position of the symbol
 *
 */
int get_position(char *cmd);


/**
 * Function used only when there is no symbol, executes the part of the command found in inner_parser()
 *
 * @param argv pointer to the command 
 * @param string_t pointer to the structure of the command
 * @param direction if where we are looking at. 0 == left & 1 == right
 *
 * @return 0 if all good 1 if error
 *
 */
int basic_parser(char *argv, string_t *cmd, alias_t *alias) ;



/**
 * Function to determine what command we are looking at. 
 *
 * @param argv pointer to the command 
  *
 * @return 1 if pwd
 * @return 2 if cd
 * @return 3 if echo
 * @return 4 if alias
 * @return 0 if error or not found
 *
 */
int inner_parser(char *cmd) ;


/**
 * Function to store the correct information in the right place in the structure exe_t. 
 *
 * @param arg command to store 
 * @param exe_t pointer to structure that contains the values  
 *
 * @return 0 all good 
 * 
 */
int exe_parser(char *arg, string_t *e, int d, alias_t *alias);


/**
 * Function that parses the block before or after the redirection  ( > < |) . 
 *
 * @param cmd that is the string to analyse
 * @param direction to know if we are treating the left (0) or right side (1)
 * @param string_t s the structure where all will be
 * @param alias_t to treat if needed. 
 *
 * @return 0 all good 
 * 
 */
int parse_block(char *cmd, int direction, string_t *s, alias_t *alias);


/**
 * Function that is executed from the MAIN. 
 *
 * @param argv the string to analyse
 * @param string_t s the structure to fill,
 * @param alias_t to treat if needed. 
 *
 * @return 0 all good 
 * 
 */
int parser_cmd(char *argv, string_t *s, alias_t *alias);


#endif //  __PARSE_H_INCLUDE__