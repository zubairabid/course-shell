#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>

int prompt() {
	struct utsname buf;
	char *user = (char*)malloc(HOST_NAME_MAX);
	char *cdir = (char*)malloc(PATH_MAX);

	// Getting system details
	uname(&buf);
	// hostname, path (might want error checking on this lad)
	user = getenv("USER");
	getcwd(cdir, PATH_MAX);

	printf("%s@%s %s $ ", user, buf.nodename, cdir);
}
