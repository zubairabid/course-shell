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
#include "pipe.h"

/*
 * Principal exec function to split command into pipeline
 * Splits command into pipeline and runs the pipeline
 *
 * Returns: Exit status
 */
int run(char **argv, int argc) {

  // Flow: check for all pipes, split commands and parent/child processes
  // accordingly. Then for each of these, process with redirection etc as
  // per normal
  // printf("About to split pipes\n");
  char ***all_com = NULL;
  all_com = pipe_split(argv, argc, all_com);
  for (char **i = *all_com; i != NULL; i = i + 1) {
    printf("%s\n", *i);
    for (char *j = *i; j != NULL; j = j + 1) {
      printf("I lived agains, bih\n");
      printf("%s ", j);
    }
    printf("\n");
    printf("%s\n", *i);
  }

  int fdes[2], ret;
  pid_t pid;
  int fd_in = 0;

  while(*all_com != NULL) {
    pipe(fdes);
    pid = fork();
    if (pid == -1) {
      perror("fork");
      exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
      dup2(fd_in, 0);
      if (*(all_com + 1) != NULL)
        dup2(fdes[1], 1);
      close(fdes[0]);
      ret = prex(*all_com);
    }
    else {
      wait(NULL);
      close(fdes[1]);
      fd_in = fdes[0];
      all_com++;
    }
  }

  return 1;
}

/*
 * Gets argument count in the value passed by pipe and sets up IO redirect
 * Continues normal execution if nothing else happens
 */
int prex(char **argv) {
  int argc = 1; // Length of the string to process is in this
  while(*argv != NULL)
    argc++;

  for(int i = 0; i < argc; i++) {
    if (argv[i][0] == '<' || argv[i][0] == '>') {
      return redirect(argv, argc);
    }
  }

  return exe(argv, argc);
}

/*
 * Primary execution function. Separates builtins out, passes others
 * to primary execvp executor function
 */
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
