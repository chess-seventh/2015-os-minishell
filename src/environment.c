/**
 * set_std_env.
 *
 * - Initialise standard environment variables into environment file.
 *
 * @param file buffer of .profile.
 * @return int, 0: success, 1: failure.
 */

#include "environment.h"

int set_std_env(FILE *profile) {
	struct passwd *env;
	char **array_env = NULL;
	char *env_name = NULL;
	char *env_value = NULL;
	char *token;
	int i = 0;
	if((env = getpwuid(getuid()))) {
			array_env = read_file(profile);
			while (strcmp(array_env[i],"\0") != 0 && array_env[i] != NULL) {
				token = strtok(array_env[i], "=");
				for (int j = 0; j < 2; ++j) {
					if (j == 0 && token != NULL) {
						env_name = malloc(sizeof(char) * strlen(token));
						strcpy(env_name, token);
					}
					else {
						if (token != NULL) {
							env_value = malloc(sizeof(char) * strlen(token));
							strcpy(env_value, token);
						}
						else
							env_name = env_value = NULL;
					}
					token = strtok(NULL, "=");
				}
				if (env_name != NULL && env_value != NULL) {
					add_env(env_name, env_value);
				}
				else
					return 2;
				i++;
			}
		if(add_env("HOME", env->pw_dir))
			return 2;
		else
			return 0;
	}
	else
		return 1;
}

/**
 * add_env.
 *
 * - Adding environment variables into environment file.
 *
 * @param name of environment variable
 * @param value of environment variable.
 * @return int, 0: success, 1: failure.
 */
int add_env(char *env_name, char *env_value) {
	return setenv(env_name,env_value, 1);
}

/**
 * get_env.
 *
 * - getting environment variables into environment file.
 *
 * @param name of environment variable.
 * @return string of environment value.
 */
char *get_env(char *env_name) {
	return getenv(env_name);
}

/**
 * get_profile.
 *
 * - loading profile file.
 *
 * @return int of environment value.
 */
int get_profile() {
	static const char *profile = "/profile";
	FILE *profile_file;
	struct passwd *env;
	if((env = getpwuid(getuid()))) {
		profile_file = fopen_file(strcat(env->pw_dir, profile));
		if (profile_file != NULL)
			return set_std_env(profile_file);
		else
			return 2;
	}
	else
		return 1;
}
