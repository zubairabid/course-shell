char **parseline(char *line, int *argc);
char **splitlines(char *line, int *num);
void sig_child(int signum);
void sig_stop(int signum);
void sig_int(int signum);
void sig_tstp(int signum);
