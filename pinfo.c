#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pinfo.h"
#include "util.h"

int pinfo(char **argv, int argc) {

  char *procpid = (char*)malloc(256*sizeof(char));
  if (argc == 1) {
    sprintf(procpid, "%d", getpid());
  }
  else {
    procpid = argv[1];
  }
  char *path = (char*)malloc(512*sizeof(char));
  strcpy(path, "/proc/");
  strcat(path, procpid);
  strcat(path, "/stat");

  int temp;
  ssize_t read;
  size_t len = 0;
  char *line = NULL;
  char **linep = NULL;
  FILE * file;
  file = fopen(path , "r");

  while ((read = getline(&line, &len, file)) != -1) {
    linep = parseline(line, &temp);
    printf("pid -- %s\n", linep[0]);
    printf("Process Status -- %s\n", linep[2]);
    printf("{Virtual Memory} -- %s\n", linep[22]);
  }
  fclose(file);

  strcpy(path, "/proc/");
  strcat(path, procpid);
  strcat(path, "/exe");
  readlink(path, line, 4096);
  line[4095] = '\0';
  printf("Executable path - %s\n",line);

  return 1;
}
