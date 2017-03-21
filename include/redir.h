#define SIZE 50

enum {
	RD_STDIN = 0,
	RD_STDOUT,
	RD_APPEND
};

int rd_pos(char *argv[], int *flag);
int rd_stdin(char *argv[], int sign_pos);
int rd_stdout(char *argv[], int sign_pos);
int rd_append(char *argv[], int sign_pos);
void redirection(char *argv[]);
