/**
 * @file exec_path.c
 *
 * All shared functions between all parts of TP.
 *
 * @author Frederick Ney
 * @author Chess Seventh
 */

#include "exec.h"
#include "function.h"
#include "navigation.h"
#include "parse.h"
/**
 * delim_count
 *
 * @param input args for counting how many args there's
 * @param input string delimiter between args
 * @return int wich is the numbers of args
 */
const int delim_count(const char* param, const char* c) {
    if (param != NULL) {
        int counter = 0;
        size_t len = strlen(param);

        for (int i = 0; i < len; i++)
            if (param[i] == *c)
                counter++;
        counter++;
        return counter;
    }
    else
        return 0;
}
/**
 * string_array.
 *
 * @param input arg of the prg
 * @param input binary execution program
 * @return array pointer of string.
 */
char **string_array(char *arg, char *prg) {
    if (prg != NULL) {
        int size = delim_count(arg, " ") + 2;
        char **param = malloc(sizeof(char *) * size);
        char *token;
        int i = 0;
        param[i] = malloc(sizeof(char) * sizeof(prg));
        param[i] = strcpy(param[i], prg);
        i++;
        if (arg != NULL)
            while ((token = strsep(&arg, " ")) != NULL) {
                param[i] = malloc(sizeof(char) * sizeof(token));
                param[i] = strcpy(param[i], token);
                i++;
            }
        param[size - 1] = NULL;
        return param;
    } else
        return NULL;
}
/**
 * exec_path.
 *
 * @param input string_t for pipes redirection and simplest execution
 * @param input alias_t  list of alias
 */
void exec_path (string_t *cmd, alias_t *alias) {
    alias_t *tmp = alias;
    char *al_cmd;
    char *argv;
    char *tmp_str;
    tmp_str = malloc(sizeof(char)* strlen(cmd->in.exe));
    tmp_str = strcpy(tmp_str,cmd->in.exe );
    tmp_str = strtok(tmp_str, " ");
    if (find_alias(tmp_str, tmp) != NULL){
        tmp = find_alias(tmp_str, tmp);
        al_cmd = get_alias_value(tmp);
        argv = malloc(sizeof(char) * strlen(al_cmd));
        argv = strcpy(argv, al_cmd);
        al_cmd = strtok(al_cmd, " ");
        cmd->in.exe = al_cmd;
        al_cmd = strtok(NULL, "\"");
        cmd->in.param = al_cmd;
        basic_parser(argv, cmd, alias);

    }
    else {
        char *path = retrive_env("PATH");
        char **param1 = string_array(cmd->in.param, cmd->in.exe);
        char **param2 = string_array(cmd->out.param, cmd->out.exe);
        pid_t pid = fork();
        if (pid != 0) {
            waitpid(pid, NULL, 0);
        }
        else if (pid == 0) {
            if (cmd->symbol.sym) {
                switch (cmd->symbol.sym) {
                    case '>':
                        execute_right_diple(cmd, param1, path);
                        break;
                    case '<':
                        execute_left_diple(cmd, param1, path);
                        break;
                    case '|':
                        pipe_execution(cmd, param1, param2, path);
                        break;
                }
            }
            else {
                execute(cmd->in, param1, path);
                exit(0);
            }
        }
    }
}
/**
 * execute.
 *
 * @param input exe_t for executing the program
 * @param input array pointer to string for prg args
 * @param input path directory
 */
void execute(exe_t prg, char **param, char *path) {
    char *exec;
    char *token;
    while((token = strsep(&path, ":")) != NULL) {
        exec = malloc(sizeof(char) * BUF_SIZE);
        strcat(exec, token);
        strcat(exec, "/");
        strcat(exec, prg.exe);
        execvp(exec, param);
    }
    perror("execvp");
    free(exec);
}
/**
 * execute.
 *
 * @param input string_t for pipes redirection and simplest execution
 * @param input array pointer to string for prg args
 * @param input path directory
 */
void execute_left_diple(string_t *cmd, char **param , char *path) {
    int pipe_d[2];
    pipe(pipe_d);
    pid_t pid = fork();
    if(pid == 0) {
        char buffer[BUF_SIZE];
        memset(buffer, 0, sizeof(buffer));
        int file_descriptor;
        file_descriptor = open(cmd->out.exe, O_CREAT | O_RDWR | O_APPEND, 0755);
        while(read(file_descriptor, buffer, BUF_SIZE) != 0) {
            write(pipe_d[WRITE_PIPE], buffer, strlen(buffer));
            memset(buffer, 0, sizeof(buffer));
        }
        close(WRITE_PIPE);
        exit(0);
    } else {
        waitpid(pid, NULL, 0);
        close(0);
        int b = dup(pipe_d[READ_PIPE]);
        dup2(b, 0);
        close(pipe_d[READ_PIPE]);
        execute(cmd->in, param, path);
        exit(0);
    }
}
/**
 * execute.
 *
 * @param input string_t for pipes redirection and simplest execution
 * @param input array pointer to string for prg args
 * @param input path directory
 */
void execute_right_diple(string_t *cmd, char **param, char *path) {
    int pipe_d[2];
    pipe(pipe_d);
    pid_t pid = fork();
    if(pid == 0) {
        int status;
        wait(&status);
        char* buffer[BUF_SIZE];
        memset(buffer, 0, sizeof(buffer));
        int file_descriptor;
        file_descriptor = open(cmd->out.exe, O_CREAT | O_RDWR | O_APPEND, 0775);
        while(read(pipe_d[READ_PIPE], buffer, BUF_SIZE) != 0) {
            write(file_descriptor, buffer, BUF_SIZE);
            perror("write");
            memset(buffer, 0, sizeof(buffer));
        }
        close(pipe_d[READ_PIPE]);
        exit(0);
    } else {
        close(1);
        int b = dup(pipe_d[WRITE_PIPE]);
        dup2(b, 1);
        close(pipe_d[WRITE_PIPE]);
        execute(cmd->in, param, path);
        exit(0);
    }
}


/**
 * execute.
 *
 * @param input string_t for pipes redirection and simplest execution
 * @param input array pointer to string for th first prg args
 * @param input array pointer to string for the second prg args
 * @param input path directory
 */
void pipe_execution(string_t *cmd, char **param1, char **param2, char *path) {
    int pipe_d[2];
    pipe(pipe_d);
    pid_t pid = fork();
    if (pid == 0) {
        close(0);
        int b = dup(pipe_d[READ_PIPE]);
        dup2(b, 0);
        close(pipe_d[READ_PIPE]);
        execute(cmd->out, param2, path);
    } else {
        close(1);
        int b = dup(pipe_d[WRITE_PIPE]);
        dup2(b, 1);
        close(pipe_d[WRITE_PIPE]);
        execute(cmd->in, param1, path);
    }
}