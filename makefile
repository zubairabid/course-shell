IDIR = include

CC=gcc
CFLAGS=-I$(IDIR) -Wall

FILES = smain.c disp.c util.c operation.c builtins.c ls.c pinfo.c redirection.c pipe.c global.c

install:
	$(CC) $(CFLAGS) $(FILES) -o shell


.PHONY: clean

clean:
	rm shell
