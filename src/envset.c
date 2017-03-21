#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <envset.h>

int cwd(void)
{
	char cwd[PATH_MAX];
  
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		error(1, errno, "Error getting current working directory");
	if (setenv("PWD", cwd, 1))
		error(1, errno, "Error setting PWD variable");
	return SUCCESS;
}

int login_usr(void)
{
	char login[LOGIN_MAX];

	if (getlogin_r(login, sizeof(login)))
		error(1, errno, "Error getting user info");	
	if (setenv("USERNAME", login, 1))
		error(1, errno, "Error setting USERNAME variable");
	return SUCCESS;
}

int get_uinfo(void)
{
	uid_t uid;
	struct passwd *passwd = NULL;

	uid = geteuid();
	passwd = getpwuid(uid);

	if (setenv("USER", passwd->pw_name, 1))
		error(1, errno, "Error setting USER variable");
	if (setenv("LOGNAME", passwd->pw_name, 1))
		error(1, errno, "Error setting LOGNAME variable");	
	if (setenv("HOME", passwd->pw_dir, 1))
		error(1, errno, "Error setting HOME variable");
	return SUCCESS;
}

int set_env(char *name, char *value)
{
	if (setenv(name, value, 1))
		error(1, errno, "Error setting %s variable", name);
	return SUCCESS;
}

void seperate(char *str, char *val[])
{
	char *token;
	int argc = 0;

	str[strlen(str) -1] = '\0';
	token = strtok(str, "=");
	while(token != NULL) {
		val[argc] = token;
		token = strtok(NULL, "=");
		argc += 1;
	}	
}

void init(void)
{
	FILE *fp;
	char *val[ENV_MAX];
	char str[STR_MAX];
  
	get_uinfo();   
	login_usr();
	cwd();
  
	if (strcpy(str, getenv("HOME")) == NULL)
		error(1, errno, "Error strcpy");
	if (strcat(str, "/.shellrc") == NULL)
		error(1, errno, "Error strcat");
  
	fp = fopen(str, "r");
	if (fp == NULL)
		error(1, errno, "Error opening config file");
	while (fgets(str, sizeof(str), fp) != NULL) {
		seperate(str, val);
		set_env(val[0], val[1]);
	}
}
