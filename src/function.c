/**
 * @file function.c
 *
 * All shared functions between all parts of TP.
 *
 * @author Frederick Ney
 * @author Chess Seventh
 */

 #include "function.h"
 #include "environment.h"
 #include "exec.h"
 #include "parse.h"
 #include "navigation.h"
/**
 * Parser to see what the user is asking to do.
 *  
 *
 * @param cmd pointer to the command 
 *
 * @return 0 if affectation went good, 1: failure.
 */
int parser(char *cmd) {
	if(strtok(cmd,"=")) {
		return 0;
	}
	else {
		return 1;
	}
}

/**
 * Read specific file.
 *  
 *
 * @param FILE buffer of a opened file
 *
 * @return char array of environment value.
 */
char **read_file(FILE *file) {
	char **array_env;
	array_env = malloc(sizeof(char *)*BUF_SIZE);
	for (int k = 0; k < BUF_SIZE; k++) {
		array_env[k] = malloc(sizeof(char) * BUF_SIZE);
	}
	int i = 0;
	char buffer[BUF_SIZE];
	char string[BUF_SIZE];
	while(fgets(buffer, BUF_SIZE, file)) {
		for (int j = 0; j < BUF_SIZE; j++) {
			string[j]= '\0';
		}
		for(int j = 0; j < BUF_SIZE; j++) {
			if (buffer[j] != '\n') {
				string[j] = buffer[j];
			}
			else {
				string[j] = '\0';
				break;
			}
		}
		strcpy(array_env[i], string);
		i++;
	}
	return array_env;
}

/**
 * Function that reads buffer in parsing.
 *
 * @param char * buffer. The command line input by the user.
 * @param int, the size of the buffer of the user input.
 *
 * @return 0 if all went good, 1: if failure.
 */
 char *read_stdin(char *buffer, int buff_size) {
 	/// 
 	char *character;
 	character = malloc(sizeof(char) * buff_size);
 	for(int i = 0; i < buff_size; i++)
			if (buffer[i] != '\n')
				character[i] = buffer[i];
			else {
				character[i]= '\0';
				break;
			}
		return (character);
 }

/**
 * fopen_file.
 *
 *
 * - opening file.
 *
 * @param path to file.
 * @return file buffer.
 */
FILE *fopen_file(char *path) {
	return fopen(path, "r+");
}

/**
 * fclose_file.
 *
 * - closing file.
 *
 * @param file descriptor.
 * @return int errno.
 */
int fclose_file(FILE *file) {
	return fclose(file);
}

/**
 * Function that retrieves the environment variable.
 *
 *
 * @param char * of the the envirnoment name.
 *
 * @return the char * of the value of the environment we looked for.
 */
char *retrive_env(char * env_name) {
	char *data = NULL;
	data = get_env(env_name);
	data = strtok(data, "\n");
	if (data == NULL)
		data = env_name;
	return data;
}
/**
 * cd_parser
 *
 * @param string_t used for interpreting the command
 */
void cd_parser(string_t *argv) {
    if (argv->in.param == '\0') {
        cd(retrive_env("HOME"));
    }
    else
	    cd(argv->in.param);
}
/**
 * alias_parser
 *
 * @param string_t used for interpreting the command
 */
void alias_parser(string_t *argv, alias_t *a) {
	parse_cmdALIAS(&argv->in, a);
}
/**
 * pwd_parser
 *
 * @param string_t used for interpreting the command
 */
void pwd_parser() {
		write(STDIN_FILENO,cmd_pwd(), strlen(cmd_pwd()));
        write(STDIN_FILENO,"\n", 1);
}
/**
 * echo_parser
 *
 * @param string_t used for interpreting the command
 */
void echo_parser(string_t *argv) {
	echo(argv->in.param);
}
/**
 * cd
 *
 * @param string argv for interpreting the command
 */
void cd(char *argv) {
	char *env;
	char *path = malloc(sizeof(char) * strlen(argv));
    env = malloc(sizeof(char) * strlen(argv));
    env = strtok(argv, "$");
    env = strtok(env, "\0");
    path = retrive_env(env);
    if (cmd_cd(path) != 0) {
        perror("cd");
    }
}
/**
 * echo
 *
 * @param string argv for interpreting the command
 */
void echo(char *argv) {
	char *data = NULL;
	char *error;
	error="environment: No such environment variable called: ";
	char *env;
	if (argv != NULL) {
		env = malloc(sizeof(char) * strlen(argv));
		env = strcpy(env, argv);
		if (env[0] != '$') {
			// executing echo with something else instead of environment variable
			data = retrive_env(env);
			write(STDIN_FILENO, data , strlen(data));
			write(STDIN_FILENO, "\n" , 1);
		}
		else {
			// executing echo with environment variable
			env = strtok(env, "$");
			data = retrive_env(env);
			// getting error
			if (strcmp(data, env) == 0) {
				write(STDIN_FILENO, error, strlen(error));
				write(STDIN_FILENO, data, strlen(data));
				write(STDIN_FILENO, "\n", 1);
			}
			else {
				write(STDIN_FILENO, data, strlen(data));
				write(STDIN_FILENO, "\n", 1);
			}
		}
	}
}