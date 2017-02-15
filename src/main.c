/* main program */

#include "function.h"
#include "navigation.h"
#include "environment.h"
#include "parse.h"
#include "exec.h"


// defining the value of BUFFER_SIZE
#define BUFFER_SIZE 1024

//profile error message

const char *profile_error = "No profile file into home directory\n";



/**
 * main program.
 *
 *
 * @param numer of string parse int argv
 * @param string array poiter argv
 *
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char **argv) {
	// getting the profile file stored into user home dir
	if (get_profile() != 0) {
		write(STDIN_FILENO, profile_error , strlen(profile_error)+1);
		// exit failure if no profile file found into user home dir
		return EXIT_FAILURE;

	}
	//initialising list of aliases
	
	alias_t *alias = NULL;
	alias = malloc(sizeof (alias_t));
    alias->alias_value = NULL;
    alias->alias_name = NULL;
    alias->next_alias = NULL;

	char buffer[BUFFER_SIZE];
	
	//the command line parsed into while loop
	char *cmd;
	

	string_t *s;

	write(STDIN_FILENO, strcat(cmd_pwd() , "$ ") , strlen(cmd_pwd())+2);
	
	while (read(STDIN_FILENO, buffer, BUFFER_SIZE)>0) {

		s = malloc(sizeof(string_t));
		s->symbol.i = 0;
		s->in.exe = NULL;
		s->in.param = NULL;
		s->out.exe = NULL;
		s->out.param = NULL;

		// getting the string parsed into shell
		cmd = read_stdin(buffer,BUFFER_SIZE);

		// finding alias using cmd
		
			parser_cmd(cmd, s, alias);

		write(STDIN_FILENO, strcat(cmd_pwd() , "$ ") , strlen(cmd_pwd())+2);
	}
	return EXIT_SUCCESS;
}


/*


	alias_tmp = find_alias(cmd, alias);

		// first check. test if the command is an alias
		if(alias_tmp != NULL)
			if (alias_tmp->alias_name != NULL && alias_tmp->alias_value != NULL) {
				//retrieving the value of the alias
				cmd = alias_tmp->alias_value;
				//storing a copy of the alias_value
				alias_value = malloc(sizeof(char) * strlen(alias_tmp->alias_value));
				alias_value = strcpy(alias_value, alias_tmp->alias_value);
				get_alias_value = 1;
			}


		// reattributing  alias value to alias
		if (get_alias_value) {
			alias_tmp->alias_value = alias_value;
			alias_value = NULL;
		}

*/ 
