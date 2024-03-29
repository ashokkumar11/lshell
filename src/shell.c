#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <unistd.h>
#include <envset.h>
#include <builtin.h>
#include <shell.h>
#include <redir.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pipe.h>
#include <colors.h>

static char buf[BUF_MAX];
static char *argv[ARGV_MAX];
static int argc;

void command_seperate(void)
{
	char *token;

	buf[strlen(buf) - 1] = '\0';
	token = strtok(buf, " ");
	while(token != NULL) {
		argv[argc] = strdup(token);
		token = strtok(NULL, " ");
		argc += 1;
	}
	argv[argc] = NULL;
	replace_shellvar();
}

void shell_prompt(void)
{
	argc = 0;
	if (strcmp(getenv("USER"), "root") == 0)
		printf(BLD WHT"%s"RED"@"LIM"%s:"BLU"%s# " RESET, getenv("USER"), getenv("USERNAME"), getenv("PWD"));
	else
		printf(BLD WHT"%s"RED"@"LIM"%s:"BLU"%s$ " RESET, getenv("USER"), getenv("USERNAME"), getenv("PWD"));
}

int command_execute(void)
{
	pid_t pid;
	int status;

	if (builtin_cmds(argv) == 0)
		return SUCCESS;
	switch (pid = fork()) {
	case -1:
		error(1, errno, "Process Error");
		break;
	case 0:    
		if (execvp(argv[0], &argv[0]) == -1)       
			error(1, errno, "execvp error");
		break;
	default:
		if (waitpid(pid, &status, 0) < 0)
			error(1, errno, "waitpid");
	}
	return SUCCESS;
}

void replace_shellvar(void)
{
	int i;
	
	for (i = 0; i < argc; i++) 
		if (argv[i][0] == '$')
			argv[i] = getenv(&argv[i][1]);	   
}

int check(void)
{
	int i;

	for (i = 0; buf[i]; i++) {
		if (buf[i] == '=')
			return 0;
		if (buf[i] == '<' || buf[i] == '>')
			return 1;
		if (buf[i] == '|')
			return 2;
	}
	return FAILURE;
}

int main()
{	
	init();
	shell_prompt();
	while (1) {
		fgets(buf, sizeof(buf), stdin);
		if (buf[0] == '\n') {
			shell_prompt();
		} else {
			if (check() == 0) {
				seperate(buf, argv);
				set_env(argv[0], argv[1]);
			} else if (check() == 1) {
				command_seperate();
				redirection(argv);
			} else if (check() == 2) {
				command_seperate();
				pipe_command(argc, argv);
			} else {
				command_seperate();
				command_execute();
			}
			shell_prompt();
		}
	}
}
