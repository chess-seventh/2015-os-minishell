/**
 * @file environment.h
 *
 * The definition of environment variables.
 *
 * @author Frederick Ney
 * @author Chess Seventh
 */

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "function.h"


/**
 * set_std_env.
 *
 * - Initialise standard environment variables into environment file.
 *
 * @param file buffer of .profile.
 * @return int, 0: success, 1: failure.
 */
int set_std_env(FILE *profile);

/**
 * add_env.
 *
 * - Adding environment variables into environment struct.
 *
 * @param name of environment variable
 * @param value of environment variable.
 * @return int, 0: success, 1: failure.
 */
int add_env(char *env_name, char *env_value);

/**
 * get_env.
 *
 * - getting environment variables into environment file.
 *
 * @param name of environment variable.
 * @return string of environment value.
 */
char *get_env(char *env_name);

/**
 * get_profile.
 *
 * - loading profile file.
 *
 * @return int of environment value.
 */
int get_profile();

#endif // ENVIRONMENT_H