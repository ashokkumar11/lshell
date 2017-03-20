#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <envset.h>
#include <builtin.h>

int shell_cd(char *path)
{
	if (path == NULL) {
		if (chdir(getenv("HOME")))
			return FAILURE;
		cwd();
	} else {
		if (chdir(path))
			return FAILURE;
		cwd();
	}
	return SUCCESS;
}

void shell_exit(void)
{
	exit(0);
}

int builtin_cmds(char *argv[])
{
	if (strcmp("history", argv[0]) == 0)
		//if (tsh_history())
		return SUCCESS;
	if (strcmp("exit", argv[0]) == 0) {
		shell_exit();
		return SUCCESS;
	}
	if (strcmp("cd", argv[0]) == 0)
		if (shell_cd(argv[1]) == 0)
			return SUCCESS;
	if (strcmp("jobs", argv[0]) == 0)
		return SUCCESS;
	if (strcmp("bg", argv[0]) == 0)
		return SUCCESS;
	if (strcmp("fg", argv[0]) == 0)
		return SUCCESS;
	return FAILURE;
}
