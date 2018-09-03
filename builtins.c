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

  chdir(location);
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
