#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "operation.h"
#include "builtins.h"
//
// int exit() {
//   return 0;
// }

int run(char **args) {

  if (strcmp(args[0],"cd") == 0) {
    return shcd(args);
  }
  else if (strcmp(args[0],"exit") == 0) {
    return shexit();
  }

  startProc(args);
  return 1;
}

int startProc(char **args) {
  int pid, wpid, status;

  // Starting a new process in Unix
  pid = fork();

  //Child Process. Loads new operation into this process with execvp
  if (pid == 0) {
    execvp(args[0], args);
    exit(EXIT_FAILURE);
  }
  // Parent Process
  else if (pid > 0){
    // Make the parent process wait a bit, or shit does not get done
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  // Error
  else {

  }

  return 1;
}
