#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/signal.h>

#include "util.h"
#include "disp.h"
#include "operation.h"

// Declaring all the functions to be used in the file
int main();
int loop();


/*
 * Setup part of the code, that is executed first and compiles various parts of
 * the shell together.
 */
int main() {

	// Load from the configuration file?

	// Run the program loop
	loop();

	// EXIT, Somehow I guess

}

/*
 * The REPL part of the shell. Large overview by definition, minor details
 * must be handled by various modules as constructed
 */

int loop() {

	size_t bsize = 0;
	char *line = (char*)malloc(bsize);
	char *command = (char*)malloc(bsize);
	char **commands;
	char **argv;
	int argc;
	int status, i, num;

	// signal(SIGINT, SIG_IGN);
	do {

		//			FLOW: Display prompt and get input
		prompt();
		getline(&line, &bsize, stdin);

		// deal with blank input
		if (*line == (char)10)
			continue;

		//			FLOW: Split lines into multiple commands by ';' etc
		//			and count them into num (passed as a pointer)
		commands = splitlines(line, &num);

		//			FLOW: Process each command extracted
		for(i = 0; i < num; i++) {
			command = commands[i];

			// blank input
			if (*command == (char)10) {
				// perror("Please input something");
				continue;
			}

			//			FLOW: Get all tokens in the each command
			//			and store the count in argc
			argv = parseline(command, &argc);

			//			FLOW: Execute the command - get the exit status
			status = run(argv, argc);
			if(status == 0)
				break;
		}

	} while(status != 0);

	return 0;
}
