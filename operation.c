#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>

#include "operation.h"
#include "builtins.h"
#include "ls.h"
#include "pinfo.h"
#include "util.h"
#include "redirection.h"

/*
 * Principal exec function.
 * Splits commands into builtins and otherwise, and processes according
 *
 * Returns: Exit status
 */
int run(char **argv, int argc) {

  // For pipes, redirection is done
  for(int i = 0; i < argc; i++) {
    if (argv[i][0] == '<' || argv[i][0] == '>') {
      return redirect(argv, argc);
    }
  }

  return exe(argv, argc);
}

int exe(char **argv, int argc) {
  // Flag, check if builtin was invoked and keep track of & existence
  int t = 0;

  // Hack to pass requirement of no '&' in builtins
  if(argv[argc-1][0] == '&') {
    argc -= 1;
    t = 1;
  }

  // Running builting functions
  if (strcmp(argv[0],"cd") == 0) {
    return shcd(argv, argc);
  }
  else if (strcmp(argv[0],"quit") == 0) {
    return shexit();
  }
  else if (strcmp(argv[0],"pwd") == 0) {
    return shpwd();
  }
  else if (strcmp(argv[0],"ls") == 0) {
    return shls(argv, argc);
  }
  else if (strcmp(argv[0],"pinfo") == 0) {
    return pinfo(argv, argc);
  }
  else if (strcmp(argv[0],"remindme") == 0) {
    return remind(argv, argc);
  }

  // If & exists in command end
  if(t == 1) {
    argc += 1;
  }

  return startProc(argv, argc);
}

int startProc(char **argv, int argc) {

  int pid, wpid, status;

  // Starting a new process in Unix
  pid = fork();

  //Child Process. Loads new operation into this process with execvp
  if (pid == 0) {
    execvp(argv[0], argv);
    perror("Could not execute command");
    exit(EXIT_FAILURE);
  }
  // Parent Process
  else if (pid > 0){

    // Make the parent process wait a bit. Or not
    if(argv[argc-1][0] == '&') {
    }
    else {
      do {
        wpid = waitpid(pid, &status, WUNTRACED);
      } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

  }
  // Error
  else {
    perror("Error forking");
  }


  return 1;
}
