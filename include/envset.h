#define LOGIN_MAX 32
#define SUCCESS 0
#define FAILURE -1
#define ENV_MAX 2
#define STR_MAX 256

int cwd(void);
int login_usr(void);
int get_uinfo(void);
int set_env(char *name, char *value);
void seperate(char *str, char *val[]);
void init(void);
