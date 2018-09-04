#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "builtins.h"

/*
 * This file contains all bultin functions. Except, maybe, ls.
 * Built-in functions include cd, pwd, exit.
 */

int shcd(char **argv, int argc) {

  char *location;
  if (argc == 1) {
    location = "~";
  }
  else {
    location = argv[1];
  }

  if (strcmp(location, "~") == 0) {
    location = getenv("HOME");
  }

  if(chdir(location) == -1) {
    perror("Could not cd into specified directory");
  }
  return 1;
}

int shpwd() {
  char *cdir = (char*)malloc(PATH_MAX * sizeof(char*));
  getcwd(cdir, PATH_MAX);

  printf("%s ", cdir);
  return 1;
}

int shexit() {
  return 0;
}

// Basic echo. No support for double quoted shit
int shecho(char **argv, int argc) {
  int i;
  // char *temp;
  for(i = 1; i < argc; i++) {
    // temp = argv[i];
    printf("%s", argv[i]);
  }
  printf("\n");
  return 1;
}
