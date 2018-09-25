#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
	pid_t pid;
	int fdes[2];
	fdes[0]=0;
	fdes[1]=0;
  int fd_in = 0;
	int c = 0;

	int orig[2];

	char *args[128][128] = {
		{"ls",  NULL},
		{"grep", "e", NULL},
		{"sort", NULL},
		{NULL}
	};

	int i = 0;

	printf("Starting args\n");
	while (*args[c] != NULL) {

		// printf("Inside loop: args[c] is \n");
		char *command[128];
		for (i = 0; args[c][i] != NULL; i++) {
			command[i] = args[c][i];
			// printf("%s ", args[c][i]);
		}
		command[i] = NULL;
		// printf("\n");


		pipe(fdes);
		pid = fork();
		if (pid == -1) {
			perror("fork");
			exit(EXIT_FAILURE);
		}
    else if (pid == 0) {
			// printf("In child, args[%d] = %s\n", c, args[c]);
      dup2(fd_in, 0);

			printf("Closed fdes[0] in child\n");
			close(fdes[0]);

			printf("Executing\n");
			for(i = 0; command[i] != NULL; i++) {
				printf("%s ", command[i]);
			}
			printf("\n");

      if (*args[c+1] != NULL) {
				printf("Setting STDOUT to fdes[1]\n");
				printf("\n");
				dup2(fdes[1], 1);
			}
      execvp(command[0], command);
			exit(EXIT_FAILURE);
    }
    else {
      wait(NULL);
      close(fdes[1]);
      fd_in = fdes[0];
			c++;
    }
		int status;
		waitpid(pid, &status, WUNTRACED);
	}
	return 0;
}
