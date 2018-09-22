#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

int main() {

	struct utsname buf;
	char *cdir = (char*)malloc(PATH_MAX);
	char *user = (char*)malloc(HOST_NAME_MAX);

	user = getenv("USER");
	uname(&buf);
	// HOST_NAME_MAX is not working, so I've used 255 instead
	//gethostname(hostname, HOST_NAME_MAX);

	// Getting pwd output
	if (getcwd(cdir, PATH_MAX) != NULL) {
		printf("No error finding path\n");
	}

	// printf("%s\n", buf.sysname);
	printf("%s\n", buf.nodename);
	// printf("%s\n", buf.release);
	// printf("%s\n", buf.version);
	// printf("%s\n", buf.machine);
	printf("%s\n", user);

	printf("%s\n", cdir);
	

	return 0;
}
