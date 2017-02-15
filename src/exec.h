/**
 * @file exec_path.c
 *
 * All shared functions between all parts of TP.
 *
 * @author Frederick Ney
 * @author Chess Seventh
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "function.h"


#ifndef __EXEC_H_INCLUDE__
#define __EXEC_H_INCLUDE__

#define WRITE_PIPE 1
#define READ_PIPE 0
#define BUF_SIZE 1024

/**
 * delim_cout
 *
 * @param input args for counting how many args there's
 * @param input string delimiter between args
 * @return int wich is the numbers of args
 */
const int delim_count(const char* param, const char* c);
/**
 * string_array.
 *
 * @param input arg of the prg
 * @param input binary execution program
 * @return array pointer of string.
 */
char **string_array(char *arg, char *prg);
/**
 * exec_path.
 *
 * @param input string_t for pipes redirection and simplest execution
 * @param input alias_t  list of alias
 */
void exec_path (string_t *cmd, alias_t *alias);
/**
 * execute.
 *
 * @param input exe_t for executing the program
 * @param input array pointer to string for prg args
 * @param input path directory
 */
void execute(exe_t prg, char **param, char *path);
/**
 * execute.
 *
 * @param input string_t for pipes redirection and simplest execution
 * @param input array pointer to string for prg args
 * @param input path directory
 */
void execute_left_diple(string_t *cmd, char **param, char *path);

/**
 * execute.
 *
 * @param input string_t for pipes redirection and simplest execution
 * @param input array pointer to string for prg args
 * @param input path directory
 */
void execute_right_diple(string_t *cmd, char **param, char *path);

/**
 * execute.
 *
 * @param input string_t for pipes redirection and simplest execution
 * @param input array pointer to string for th first prg args
 * @param input array pointer to string for the second prg args
 * @param input path directory
 */
void pipe_execution(string_t *cmd, char **param1, char **param2, char *path);

#endif //__EXEC_H_INCLUDE__
