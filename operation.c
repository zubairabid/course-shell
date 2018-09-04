#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "operation.h"
#include "builtins.h"
#include "ls.h"


int run(char **argv, int argc) {

  int t = 0;

  // Hack to pass requirement
  if(argv[argc-1][0] == '&') {
    argc -= 1;
    t = 1;
  }

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
  else if (strcmp(argv[0],"ls") == 0) {
    return shls(argv, argc);
  }

  if(t == 1) {
    argc += 1;
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
    // Make the parent process wait a bit. Or not
    if(argv[argc-1][0] == '&') {
      // do {
      //   wpid = waitpid(pid, &status, WNOHANG);
      // } while(!WIFEXITED(status) && !WIFSIGNALED(status));
      // printf("exited %d\n", status);
    }
    else {
      do {
        wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
      // printf("%d exited %d\n", wpid, status);
    }

    // printf("%d exited %d\n", wpid, status);
  }
  // Error
  else {

  }

  return 1;
}
