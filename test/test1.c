#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
	pid_t pid1, pid2;
	int pipefd[2];

	char *argv1[] = {"cat", "test3", NULL};
	char *argv2[] = {"wc", "-c", NULL};

	pipe(pipefd);

	pid1 = fork();

	if (pid1 == -1) {
		perror("fork");
	}
	if (pid1 == 0) {
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		execvp(argv1[0], argv1);
		perror("exec");
		return 1;
	}


	else {
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[1]);
		execvp(argv2[0], argv2);
		perror("exec");
		return 1;
	}

	close(pipefd[0]);
	close(pipefd[1]);
	// Wait for everything to finish and exit.
	waitpid(pid1);

	return 0;
}
