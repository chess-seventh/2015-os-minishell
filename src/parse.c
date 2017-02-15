/**
 * @file parse.c
 *
 * Functions that are used to execute a command.
 *
 * @author Frederick Ney
 * @author Chess Seventh
 */

#include "parse.h"
#include "exec.h"
#include "navigation.h"

/**
 * Parser to see there is a symbol pipe sup or inf in the command
 *
 * @param cmd pointer to the command 
 *
 * @return 0 if we found it in the cmd.
 * @return 1 if we did not find it.
 */
int symbol_find(string_t *s, char *cmd) {
	const char tocheck[] = {'>' , '<', '|'} ;
	const int l = 3;
	int i, j;
	char *tmp_cmd;
	tmp_cmd = malloc(sizeof(char) * strlen(cmd));
	tmp_cmd = strcpy(tmp_cmd, cmd);
	s->symbol.i = 1;
	for (i = 0; i < strlen(tmp_cmd); i++) {
		for (j = 0; j < l; j++) {
			if(tmp_cmd[i] == tocheck[j]) {
				s->symbol.sym = tocheck[j];
				s->symbol.i = 0; // 0 is true, meaning that there is symbol in it.
				return 0;
			}
		}
	}
	return 1;
}


/**
 * Function that looks at the position of where the symbol is 
 *
 * @param cmd pointer to the command 
 *
 * @return the position of the symbol
 *
 */
int get_position(char *arg) {
	int i, nb;
	nb = -1;
	bool found = false;
	char *cmd ;
	cmd = malloc(sizeof(char) * strlen(arg));
	cmd = strcpy(cmd, arg);
	//cmd[strlen(arg)] = malloc(sizeof(char) * strlen(arg));
	
	for (i = 0 ; i < strlen(cmd) ; i++) {
		if( cmd[i] == '>' || cmd[i] == '<' || cmd[i] == '|') {
				found = true;
				nb = i;
		}
	}
	if(found == false) {
		return -1; // error not found
	}
	if(found == true) {
		return nb;
	}
	return nb;
}



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
int basic_parser(char *argv, string_t *cmd, alias_t *alias) {
		char *comd;
		char *ex;
		bool success = false;
		comd = malloc(sizeof(char) * strlen(argv));

		if ( cmd->symbol.i) {
		comd = strcpy(comd, argv);
	
		ex = strtok(comd, " ");

		switch(inner_parser(ex)){

			case 0:
				// printf("command not found");
				success = false;
				break;

			case 1:
				pwd_parser();
				success = true;
				break;

			case 2:
				cd_parser(cmd);
				success = true;
				break;

			case 3:
				echo_parser(cmd);
				success = true;
				break;

			case 4:
				if (cmd->in.param == NULL) {
					usageAlias();
				}
				else {	
					alias_parser(cmd, alias);
					success = true;
				}
				break;

			case 5:
				exit(0);
				break;

			default:
			exec_path(cmd, alias);
			success = true;
			break;
		}
	} else {
		success = false;
	} 

	if (success == true) {
		return 0;
	} else if (success == false) {
		return 1;
	}
	return -1;
}


/**
 * Function to determine what command we are looking at. 
 *
 * @param argv pointer to the command 
  *
 * @return 1 if pwd
 * @return 2 if cd
 * @return 3 if echo
 * @return 4 if alias
 * @return 5 if exit or quit
 * @return 0 if error or not found
 *
 */
int inner_parser(char *cmd) {

	char *tmp;
	tmp = malloc(sizeof(char) * strlen(cmd));
	tmp = strcpy(tmp, cmd);
	int ret;

	if (strcmp(tmp, "pwd") == 0) {
		ret = 1;
	}
	 else if (strcmp(tmp, "cd") == 0) {
			ret = 2;
		}
	 else if (strcmp(tmp, "echo") == 0){
		ret = 3;	
	}	
	 else if( strcmp (tmp, "alias") == 0){
		ret = 4;
	}		
	 else if( strcmp(tmp, "exit") == 0) {
			ret = 5;
	}
	 else if( strcmp(tmp, "quit") == 0) {
			ret = 5;
	}
	else if (strcmp(tmp, "\0"))
		return 6;
		else {
		ret = 0;
	}
	return ret;
}

/**
 * Function to store the correct information in the right place in the structure exe_t. 
 *
 * @param arg command to store 
 * @param exe_t pointer to structure that contains the values  
 *
 * @return 0 all good 
 * 
 */
int exe_parser(char *arg, string_t *e, int d, alias_t *alias) {
	char *tmp;
	char *cmd_left ;
	char *cmd_right ;
	char *cmd_tmp;

	tmp = malloc(sizeof(char) * strlen(arg));
	tmp = strcpy(tmp, arg);

	switch (d) {
		case 0:
			
			cmd_left = malloc(sizeof(char)* 1024);
			cmd_right = malloc(sizeof(char)* 1024);
			cmd_tmp = strtok(arg, &e->symbol.sym);
			cmd_left = strcpy(cmd_left, cmd_tmp);
			cmd_tmp = strtok(NULL, &e->symbol.sym);
			cmd_right = strcpy(cmd_right, cmd_tmp);
			
			cmd_tmp = strtok(cmd_left, " ");
			e->in.exe = cmd_tmp;
			cmd_tmp = strtok(NULL, "\"");
			e->in.param = cmd_tmp;

			cmd_tmp = strtok(cmd_right, " ");
			e->out.exe = cmd_tmp;
			cmd_tmp = strtok(NULL, "\"");
			e->out.param = cmd_tmp;

			exec_path(e, alias);


			break;
		
		case -1:
			tmp = strtok(tmp, " ");
			e->in.exe = tmp;
			tmp = strtok(NULL, "\"");
			e->in.param = tmp;
			break;
	}
	return 0 ; // all good
}


/**
 * Function that parses the block before or after the redirection  ( > < |) . 
 *
 * @param cmd that is the string to analyse
 * @param direction to know if we are treating the left (0) or right side (1)
 * @param string_t s the structure where all will be
 * @param alias_t to treat if needed. 
 *C
 * @return 0 all good 
 * 
 */
int parse_block(char *cmd, int direction, string_t *s, alias_t *alias) {
	// if direction  == 0 then we are looking at the left side of the array
	// inf direction  == 1 then we are looking at the right side of the array

	bool success = false;
	int i=0;
	switch(direction) {
		case 0: // left parsing 
			if(cmd != NULL) {
				
				i = exe_parser(cmd, s, direction, alias);
				if(i == 1) {
					success = false;
				}
			} else if (cmd == NULL) {
				success = false;
			}
			success = true;
			break;

		case -1:
			i = exe_parser(cmd, s, direction, alias);
			if( (basic_parser(cmd, s, alias) == 0) ) { // all went well.
					if (i == 1) {
						success = false;
					}else if (i == 0) {
						success = true;	
					}
			}
			break;

		default:
			if( (basic_parser(cmd, s, alias) == 0) ) { // all went well.
				success = true;
			}
			break;
	}
	if(success == true || i == 0) {
		return 0;
	} else if(success == false || i == 1) {
		return 1;
	}
	return -1; // something went wrong.
}

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
int parser_cmd(char *argv, string_t *s, alias_t *alias) {

	char* cmd;
	cmd = malloc(sizeof(char));

	int i = 0;
	char *cmd_left ;
	cmd_left = malloc(sizeof(char) * strlen(argv));

	cmd = strcpy(cmd, argv); // variable tempo pour traitement

	

		if(symbol_find(s, cmd) == 0) {				
				parse_block(cmd, 0, s, alias); // 0 == left from symbol 	

		} else if(symbol_find(s, cmd) == 1) { // did not find a symbol so to treat normally.
			for (i = 0 ; i < strlen(cmd) ; i++) {
				cmd_left[i] = cmd[i]; // 3
			}
			parse_block(cmd_left, -1, s, alias);
		}

return 0; // all good

} 
