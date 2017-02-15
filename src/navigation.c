/**
 * @file navigation.h
 *
 * Functions for navigation in Shell.
 *
 * @author Frederick Ney
 * @author Chess Seventh
 */

#include "navigation.h"
#include "function.h"

/*/
       int chdir(const char *path);
       int fchdir(int fd);
//*/


 /**
 * Command to go to the mentioned directory.
 *
 */
int cmd_cd(char *path) {
	if (path == NULL) {
		return 1; // Failed
	}
	return (chdir(path));
}



 /**
 * Command to print the current directory.
 * uses getcwd sys call. 
 * 
*/

char *cmd_pwd() {
	char dirname[1024];
	return (getcwd(dirname, 1024));
}



 /**
 * Find an ALIAS .
 *
 * @param input name of alias to find
 * @param structure of current alias
 * @return pointer on structure that contains the alias.
 */
alias_t *find_alias(char *alias_name, alias_t *alias) {
	alias_t *tmp_alias = alias;
	if (alias->alias_value != NULL && alias->alias_name != NULL) {
		if (strcmp(alias_name, tmp_alias->alias_name) == 0) {
			return tmp_alias;
		}
		if (tmp_alias->next_alias != NULL) {
			tmp_alias = tmp_alias->next_alias;
			while (tmp_alias != NULL) {
				if (strcmp(alias_name, tmp_alias->alias_name) == 0) {
					return tmp_alias;
					break;
				}
				tmp_alias = tmp_alias->next_alias;
                if (tmp_alias == NULL)
                    return NULL;
			}
			return tmp_alias;
		}
		else
			return NULL;
	}
 	else
		return NULL;
}

/**
 * List all alias.
 * 
 * @param the structure of all aliases in order to list them
 *
 * @return void because all will be printed
 *
 */
void list_alias(alias_t *alias) {
	alias_t *tmp_alias = alias;
	if (alias->alias_name != NULL && alias->alias_value != NULL) {
		write(STDIN_FILENO, tmp_alias->alias_name, strlen(tmp_alias->alias_name));
		write(STDIN_FILENO, "\n", 1);
		while (tmp_alias->next_alias != NULL) {
			write(STDIN_FILENO, tmp_alias->next_alias->alias_name, strlen(tmp_alias->next_alias->alias_name));
			write(STDIN_FILENO, "\n", 1);
			tmp_alias = tmp_alias->next_alias;
		}
	}
}

 /**
 * Find an ALIAS Value.
 *
 * @param structure of current alias.
 * @return the value of the alias.
 */
char *get_alias_value(alias_t *current) {
	return (current->alias_value);
}



 /**
 * Add an ALIAS .
 *
 * @param input alias name
 * @param input alias value
 * @param structure of current alias
 * @return int for sucess or failure.
 */
int add_alias(char *alias_name, char *alias_value, alias_t *current) {
	alias_t	*tmp_alias;
	tmp_alias = malloc(sizeof(alias_t) * 1);
	tmp_alias->alias_name = alias_name;
	tmp_alias->alias_value = alias_value;
	tmp_alias->next_alias = NULL;

	alias_t *tmp_alias_current;
	tmp_alias_current = current;
	if(current->alias_name == NULL && current->alias_value == NULL) {
		current->alias_name = tmp_alias->alias_name;
		current->alias_value = tmp_alias->alias_value;
		return 0 ; // sucess
	} 
	else if(current->alias_name != NULL) {
		while(tmp_alias_current->next_alias != NULL) {
			tmp_alias_current = tmp_alias_current->next_alias;
		}
		tmp_alias_current->next_alias = tmp_alias;
		return 0;
	}
	else
		return 1 ; // fail
}

 /**
 * Delete an ALIAS .
 *
 * @param input alias name
 * @param structure of current alias
 * @return int to define error.
 */
int delete_alias(char *alias_name, alias_t *current) {
	alias_t *tmp_alias = current;
	alias_t *alias_to_delete;
	if(strcmp(tmp_alias->alias_name, alias_name) == 0) {
		if (current->next_alias != NULL) {
		current->alias_value = current->next_alias->alias_value;
		current->alias_name = current->next_alias->alias_name;
		tmp_alias = current->next_alias;
		current->next_alias = current->next_alias->next_alias;
		}
		tmp_alias->alias_name = NULL;
		tmp_alias->alias_value = NULL;
		tmp_alias->alias_value = NULL;
		free(tmp_alias);
 		tmp_alias = NULL;
		if (current->alias_name == NULL && current->alias_value == NULL) {
			current = malloc(sizeof(alias_t));
			current->alias_value = NULL;
			current->alias_name = NULL;
			current->next_alias = NULL;
		}
		return 0;
 	}
	while(tmp_alias->next_alias != NULL) {
 		if(strcmp(tmp_alias->next_alias->alias_name, alias_name) == 0) {
 			alias_to_delete = tmp_alias->next_alias;
 			tmp_alias->next_alias = tmp_alias->next_alias->next_alias;
			free(alias_to_delete);
			alias_to_delete = NULL;
			return 0;
			break;
 		}
 		else {
 			tmp_alias = tmp_alias->next_alias;

 		}
 	}
	return 1;
}


 /**
 * Parse the command ALIAS .
 *
 * @param char* input command from user
 * @param current structure of aliases
 *
 * @return int 0 is sucess
 *
 */
int parse_cmdALIAS(exe_t *e, alias_t *alias_current) {
	int to_do = 0; // 0 = error
	char *tmp_aliasname = NULL;
	char *tmp_value = NULL;
	char *cmd_alias;
    cmd_alias = malloc (sizeof(char) * strlen(e->param));
    cmd_alias = strcpy(cmd_alias, e->param);

	cmd_alias = strtok(cmd_alias, " ");
	if (cmd_alias != NULL) {
		if (strcmp(cmd_alias, "-h") == 0 || strcmp(cmd_alias, "--help") == 0) {
			usageAlias();
			return 0;
		}
		if (strcmp(cmd_alias, "-a") == 0) {
			to_do = 1; // add
			cmd_alias = strtok(NULL, " ");
			if (cmd_alias != NULL) {
				if (strcmp(cmd_alias, "-n") == 0) {
					cmd_alias = strtok(NULL, " ");
					if (cmd_alias != NULL) {
						if (cmd_alias != NULL && strcmp(cmd_alias, "-v") != 0) {
							tmp_aliasname = cmd_alias;
							cmd_alias = strtok(NULL, " ");
							if (cmd_alias != NULL) {
								if (strcmp(cmd_alias, "-v") == 0) {
									cmd_alias = strtok(NULL, "\"");
                                    cmd_alias = strtok(cmd_alias, "!");
									if (cmd_alias != NULL) {
										tmp_value = cmd_alias;
									}
									else if (cmd_alias == NULL) {
										// fail at -v argument
										usageAlias();
										return 3;
									}
								}
								else {
									// fail
									usageAlias();
									return 2;
								}
							}
							else {
								// fail
								usageAlias();
								return 2;
							}
						}
						else {
							usageAlias();
							return 2; // fail at  the -v
						}
					}
					else {
						usageAlias();
						return 2; // fail at  the -v
					}
				}

				else {
					usageAlias();
					return 2; // fail at  the -v
				}
			}
			else {
				usageAlias();
				return 2; // fail at  the -v
			}
		}
		else if (strcmp(cmd_alias, "-d") == 0) {
			cmd_alias = strtok(NULL, " \0\n");
			if (cmd_alias != NULL) {
				to_do = 2; // to delete
				tmp_aliasname = cmd_alias;
			}
			else {
				usageAlias();
				return 2; // fail at  the -v
			}
		}
		else if (strcmp(cmd_alias, "-l") == 0) {
			to_do = 3; // list all
		}
		else if (strcmp(cmd_alias, "-v") == 0) {
			tmp_aliasname = cmd_alias;
			cmd_alias = strtok(NULL, " \0\n");
			if (cmd_alias != NULL) {
				if (cmd_alias != NULL && strcmp(cmd_alias, "-n") != 0 && strcmp(cmd_alias, "-l") != 0 && strcmp(cmd_alias, "-d") != 0 && strcmp(cmd_alias, "-v") != 0 && strcmp(cmd_alias, "-a") != 0) {
					tmp_aliasname = cmd_alias;
					alias_t *tmp = find_alias(tmp_aliasname, alias_current);
					if (tmp != NULL) {
						write(STDIN_FILENO, tmp->alias_value, strlen(tmp->alias_value));
						write(STDIN_FILENO, "\n", 1);
					}
				}
				else {
					usageAlias();
					return 2; // fail at  the -v
				}
			}
			else {
				usageAlias();
				return 2; // fail at  the -v
			}
		}
		else {
			//fail no initial parameters
			usageAlias();
			return 1;
		}
	}
	else {
		//fail no initial parameters
		usageAlias();
		return 1;
	}

	switch(to_do) {
		case 1: // ADD
			add_alias(tmp_aliasname, tmp_value, alias_current);
			return 0;
			break;

		case 2: // DELETE
			delete_alias(tmp_aliasname, alias_current);
			return 0;
			break;

		case 3: // LIST ALL
			list_alias(alias_current);
			return 0;
			break;
	}
	return 1;
}


 /**
  * Parse the command LS .
  *
  * @param path_name pointer to a directory [optional].
  * default value is the current directory.
  */
 void cmd_ls (char *path_name) {
	 DIR *directory;
	 struct dirent *dir;
	 struct stat stats;
	 char *path_dir = path_name;
	 if (path_dir == NULL)
		 path_dir = "./";
	 char buffer[1024];
	 char num_to_string[1024];
	 char *print;
	 directory = opendir(path_dir);
	 if(directory == NULL )
		 perror("ls: cannot access source");
	 else {
		 while ((dir = readdir(directory)) != NULL) {
			 sprintf(buffer, "%s/%s", path_dir, dir->d_name);
			 sprintf(num_to_string,"%lu", stats.st_size);
			 stat(buffer, &stats);
			 sprintf(num_to_string,"%lu", stats.st_size);
			 print = num_to_string;
			 print = strcat(print, " ");
			 print = strcat(print, dir->d_name);
			 print = strcat(print, "\n");
			 write(STDIN_FILENO, print, strlen(print));
		 }
		 closedir(directory);
	 }
 }


 /**
  * Display usage of the Alias command. .
  *
  */
void usageAlias() {
	char *usage = "usage: alias [-a -n [NAME] -v [VALUE] ] or [-d [NAME]] or [-l] or [-v [NAME]] or [-h] or [--help]\n\n\toptions:\n\n\t-a: is to add an Alias and it's value:\n\t\tto name the alias:\n\t\t-n [NAME]: name is the name of your alias.\n\t\t-v [VALUE]: value is the command of the alias.\n\n\t-d [NAME]: delete an Alias.\n\n\t-l: List all Aliases. \n\n\t-v [NAME]: get alias value.\n\n\t-h --help : displays this help message.\n\n\n";
	 write(STDIN_FILENO, usage , strlen(usage));
}