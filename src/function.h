/**
 * @file function.h
 *
 * All shared functions between all parts of TP.
 *
 * @author Frederick Ney
 * @author Chess Seventh
 */

 #include <stdio.h>
 #include <string.h>
 #include <stdlib.h>
 #include <unistd.h>
 #include <stdlib.h>
 #include <dirent.h>
 #include <sys/stat.h>
 #include <sys/types.h>



/*
 #include "navigation.h"
 #include "environment.h"
 #include "exec.h"
 #include "parse.h"
*/
// from exec.h


 #ifndef __FUNCTION_H_INCLUDE__
 #define __FUNCTION_H_INCLUDE__


// char* retrieve_env(env_name) if env_name == token then no env
// list alias
 typedef struct alias {
	/// name of the alias.
	char *alias_name;

	/// structure of all command known.
	char *alias_value;

	/// next alias
	struct alias *next_alias;

} alias_t;


typedef struct {
	char *exe;
	char *param;
} exe_t;

typedef struct {
	int i; // 0 no symbol 1 == true
	char sym;
} symbol_t;

 typedef struct {
 	exe_t in;
 	symbol_t symbol;
 	exe_t out;
 } string_t;


/**
 * Parser to see what the user is asking to do.
 *  
 *
 * @param cmd pointer to the command 
 *
 * @return 0 if affectation went good, 1: failure.
 */
int parser(char *cmd);

/**
 * Read specific file.
 *  
 *
 * @param FILE buffer of a opened file
 *
 * @return char array of environment value.
 */
char **read_file(FILE *file);

/**
 * Function that reads buffer in parsing.
 *
 * @param char * buffer. The command line input by the user.
 * @param int, the size of the buffer of the user input.
 *
 * @return 0 if all went good, 1: if failure.
 */
char *read_stdin(char *buffer, int buff_size);

/**
 * fopen_file.
 *
 *
 * - opening file.
 *
 * @param path to file.
 * @return file buffer.
 */
FILE *fopen_file(char *path);


/**
 * fclose_file.
 *
 * - closing file.
 *
 * @param file descriptor.
 * @return int errno.
 */
int fclose_file(FILE *file);

/**
 * Function that retrieves the environment variable.
 *
 *
 * @param char * of the the envirnoment name.
 *
 * @return the char * of the value of the environment we looked for.
 */
char *retrive_env(char * env_name);
/**
 * cd_parser
 *
 * @param string_t used for interpreting the command
 */
void cd_parser(string_t *argv);
/**
 * alias_parser
 *
 * @param string_t used for interpreting the command
 */
void alias_parser(string_t *argv, alias_t *a);
/**
 * pwd_parser
 *
 * @param string_t used for interpreting the command
 */
void pwd_parser();
/**
 * echo_parser
 *
 * @param string_t used for interpreting the command
 */
void echo_parser(string_t *argv);
/**
 * cd
 *
 * @param string argv for interpreting the command
 */
void cd(char *argv);
/**
 * echo
 *
 * @param string argv for interpreting the command
 */
void echo(char *argv);

#endif // __FUNCTION_H_INCLUDE__