#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "redirection.h"
#include "operation.h"
#include "util.h"

int redirect(char **argv, int argc) {
  // There are 5 cases here:
  // 1. Single '>'. Replace STDOUT with file
  // 2. Single '>>'. Same as 1, use append
  // 3. Single '<'. Replace STDIN with file
  // 4. Single '<' followed by '>' at index+2. Replace STDIN, STDOUT
  // 5. Single '<' followed by '>>' at index+2. Imitate (2,1) for (5,4)

  int temp, type, stdout_s, stdin_s; // Type: 1, 2, 3, 4, 5
  char **command, *ifile, *ofile, *sym;
  int out, in;
  command = (char**)malloc(128*sizeof(char));

  // Process arguments
  // Extract command
  for (int i = 0; i < argc; i++) {
    if (argv[i][0] != '<' && argv[i][0] != '>') {
      command[i] = argv[i];
      strcat(command[i], "\0");
    }
    else {
      temp = i;
      break;
    }
  }

  printf("\nIn redirect: executed command\n");
  for(int i = 0; i < temp; i++) {
    printf("%s ", command[i]);
  }
  printf("\n\n");

  // Invalid syntax
  if(temp == argc - 1) {
    return 0;
  }

  sym = argv[temp];
  if (sym[0] == '>') {
    ofile = argv[temp+1];
    strcat(ofile, "\0");
    if (strlen(sym) == 1) {
      type = 1;
    }
    else {
      type = 2;
    }

  }
  else {
    ifile = argv[temp+1];
    strcat(ifile, "\0");
    type = 3;

    if(temp+1 != argc-1) { // More
      if(temp+2 == argc-1) { // Invalid syntax
        return 0;
      }
      if(argv[temp+2][0] != '>') { // Invalid syntax
        return 0;
      }
      ofile = argv[temp+3];

      if (strlen(argv[temp+2]) == 1) {
        type = 4;
      }
      else {
        type = 5;
      }
    }
  }

  stdin_s = dup(0);
  stdout_s = dup(1);

  if (type != 3) { // Open ofile
    if (type == 1 || type == 4)
      out = open(ofile, O_WRONLY | O_CREAT, 0644);
    else
      out = open(ofile, O_WRONLY | O_APPEND | O_CREAT, 0644);
    dup2(out, 1);
    close(out);
  }
  if(type >= 3) { // Open ifile
    in = open(ifile, O_RDONLY);
    dup2(in, 0);
    close(in);
  }

  int ret = exe(command, temp);

  dup2(stdin_s, 0);
  dup2(stdout_s, 1);

  return ret;
}
