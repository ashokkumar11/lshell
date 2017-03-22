#include <unistd.h>
#include <fcntl.h>  
#include <stdio.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int shell_pipe(int argc, char *argv[], int count)
{
	int fd[2];
	int i;
	int j;
	int k = 0;
	int fd_in = 0;	
	int pid;
	
	for (i = 0, j = 0; i < count +1; i++, j = k) {
		for (; argv[k] != NULL; k++) {
		}		
		k++;		
		pipe(fd);
		if ((pid = fork()) == -1) {
			exit(EXIT_FAILURE);
		} else if (pid == 0) {
			dup2(fd_in, 0);
			if (i != count)
				dup2(fd[1], 1);
			close(fd[0]);
			execvp(argv[j], &argv[j]);
			exit(EXIT_FAILURE);
		} else {
								
			wait(NULL);
			close(fd[1]);
			fd_in = fd[0];
		}
	}
}

int pipe_command(int argc, char *argv[])
{
	int count = 0;
	int i;
	
	for (i = 0; i < argc; i++) {
		if (strcmp(argv[i],"|") == 0) {
			argv[i] = NULL;
			count++;
		} 
	}
	shell_pipe(argc, argv, count);
	return 0;
}
