#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include "builtins.h"

/*
 * This file contains all bultin functions. Except, maybe, ls.
 * Built-in functions include cd, pwd, exit.
 */

int shcd(char **args) {
  chdir(args[1]);
  return 1;
}

int shpwd() {
  char *cdir = (char*)malloc(PATH_MAX * sizeof(char*));
  getcwd(cdir, PATH_MAX);

  printf("%s ", cdir);
  return 0;
}

int shexit() {
  return 0;
}
