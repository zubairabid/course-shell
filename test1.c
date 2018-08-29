#include <stdio.h>
#include <sys/utsname.h>
#include <string.h>
#include <unistd.h>

int main() {

	struct utsname buf;
	char *hostname;

	uname(&buf);
	// HOST_NAME_MAX is not working, so I've used 255 instead
	gethostname(hostname, 255);

	printf("%s\n", buf.sysname);
	printf("%s\n", buf.nodename);
	printf("%s\n", buf.release);
	printf("%s\n", buf.version);
	printf("%s\n", buf.machine);
	printf("%s\n", hostname);

	

	return 0;
}
