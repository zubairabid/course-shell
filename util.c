#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

#define DELIM_TOKEN " \t\r\f\a\n"

char **parseline(char *line) {

  int bsize = 128, count = 0;

  char *token;
  char **tokens = (char**)malloc(bsize * sizeof(char*));

  token = strtok(line, DELIM_TOKEN);

  do {

    tokens[count++] = token;
    token = strtok(NULL, DELIM_TOKEN);

  } while(token != NULL);

  return tokens;

}
