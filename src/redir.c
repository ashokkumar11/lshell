#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <redir.h>

int rd_pos(char *argv[], int *flag)
{

	int loop;

	for (loop = 0; argv[loop] != NULL; loop++) {
		if (strcmp(argv[loop], "<") == 0) {
			*flag = RD_STDIN;
			return loop;
		}
		if (strcmp(argv[loop], ">") == 0) {
			*flag = RD_STDOUT;
			return loop;
		}
		if (strcmp(argv[loop], ">>") == 0) {
			*flag = RD_APPEND;
			return loop;
		}
	}
	return -1;
}

int rd_stdin(char *argv[], int sign_pos)
{
	pid_t pid;
	int status;
	int fd;
	char *cmd[SIZE];
	int loop;

	for (loop = 0; loop < sign_pos; loop++)
		cmd[loop] = argv[loop];
	cmd[loop] = NULL;

	pid = fork();
	switch (pid) {
	case 0:
		fd = open(argv[2], O_RDONLY);
		if (fd == -1)
			error(1, errno, "File cannot be opened");
		dup2(fd, STDIN_FILENO);

		if (execvp(cmd[0], cmd) == -1)
			return -1;
		close(fd);
		break;
	case -1:
		printf("error");
		break;

	default:
		if (wait(&status) == -1)
			return -1;
		break;
	}
	return 0;

}

int rd_stdout(char *argv[], int sign_pos)
{
	pid_t pid;
	int status;
	int fd;
	char *cmd[SIZE];
	int loop;

	for (loop = 0; loop < sign_pos; loop++)
		cmd[loop] = argv[loop];
	cmd[loop] = NULL;

	pid = fork();
	switch (pid) {
	case 0:
		fd = open(argv[sign_pos+1], O_WRONLY|O_CREAT, 0666);
		dup2(fd, STDOUT_FILENO);
		if (execvp(cmd[0], cmd) == -1)
			return -1;
		close(fd);
		break;
	case -1:
		printf("error");
		break;
	default:
		if (wait(&status) == -1)
			return -1;
		break;
	}
	return 0;
}

int rd_append(char *argv[], int sign_pos)
{
	pid_t pid;
	int status;
	int fd;
	char *cmd[SIZE];
	int loop;

	for (loop = 0; loop < sign_pos; loop++)
		cmd[loop] = argv[loop];
	cmd[loop] = NULL;

	pid = fork();
	switch (pid) {
	case 0:
		fd = open(argv[sign_pos+1], O_WRONLY|O_CREAT|O_APPEND, 0666);
		dup2(fd, STDOUT_FILENO);
		if (execvp(cmd[0], cmd) == -1)
			return -1;
		close(fd);
		break;
	case -1:
		printf("error");
		break;
	default:
		if (wait(&status) == -1)
			return -1;
		break;
	}
	return 0;

}

int redirection(char *argv[])
{
	int flag;
	int sign_pos;


	sign_pos = rd_pos(argv, &flag);

	switch (flag) {
	case RD_STDIN:
		rd_stdin(argv, sign_pos);
		break;
	case RD_STDOUT:
		rd_stdout(argv, sign_pos);
		break;
	case RD_APPEND:
		rd_append(argv, sign_pos);
		break;
	}
	return 0;
}
