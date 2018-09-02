#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
	char **argv;
	int argc;
	int status;

	do {
		prompt();
		getline(&line, &bsize, stdin);
		if (*line == (char)10)
			continue;
		argv = parseline(line, &argc);

		status = run(argv, argc);
	} while(status != 0);

	return 0;
}
