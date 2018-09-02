#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "operation.h"
#include "builtins.h"
#include "ls.h"


int run(char **argv, int argc) {

  // Running builting functions
  if (strcmp(argv[0],"cd") == 0) {
    return shcd(argv, argc);
  }
  else if (strcmp(argv[0],"exit") == 0) {
    return shexit();
  }
  else if (strcmp(argv[0],"pwd") == 0) {
    return shpwd();
  }
  else if (strcmp(argv[0],"shls") == 0) {
    return shls(argv, argc);
  }


  startProc(argv, argc);
  return 1;
}

int startProc(char **argv, int argc) {

  int pid, wpid, status;

  // Starting a new process in Unix
  pid = fork();

  //Child Process. Loads new operation into this process with execvp
  if (pid == 0) {
    execvp(argv[0], argv);
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
