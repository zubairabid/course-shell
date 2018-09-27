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
  // Builtin non fork functions
  if (strcmp(argv[0],"cd") == 0) {
    return shcd(argv, argc);
  }
  else if (strcmp(argv[0],"quit") == 0) {
    return shexit();
  }
  else if (strcmp(argv[0], "setenv") == 0) {
    return shsetenv(argv, argc);
  }
  else if (strcmp(argv[0], "unsetenv") == 0) {
    return shunsetenv(argv, argc);
  }


  // prex(argv);
  // return 1;

  // Flow: check for all pipes, split commands and parent/child processes
  // accordingly. Then for each of these, process with redirection etc as
  // per normal



  // Stores all comss
  char *args[128][128];
  for(int i=0;i<128;i++)
  {
    for(int j=0;j<128;j++)
      args[i][j] = NULL;
  }

  int m = 0, n = 0;
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i],"|") != 0) {
      args[m][n++] = argv[i];
      strcat(args[m][n], "\0");
    }
    else {
      args[m][n] = NULL;
      m++;
      n = 0;
    }
  }
  args[++m][0] = NULL;


  // printf("THIS IS A TEST\n");
  // for(int i = 0; *args[i] != NULL; i++) {
  //   for(int j = 0; args[i][j] != NULL; i++) {
  //     printf("%s", args[i][j]);
  //   }
  //   printf("\n");
  // }


  int fdes[2] = {0, 0}, ret;
  pid_t pid;
  int fd_in = 0;
  int c = 0, i;



  while(*args[c] != NULL) {

    char *command[128];
    for (i = 0; args[c][i] != NULL; i++) {
			command[i] = args[c][i];
		}
    command[i] = NULL;
    pipe(fdes);

    pid = fork();
    if (pid == -1) {
      perror("fork");
      exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
      dup2(fd_in, 0);

			close(fdes[0]);

      if (*args[c+1] != NULL) {
				dup2(fdes[1], 1);
			}
      c++;
      ret = prex(command);
      exit(0);
    }
    else {
      wait(NULL);
      close(fdes[1]);
      fd_in = fdes[0];
      c++;
    }
  }
  int status;
  waitpid(pid, &status, WUNTRACED);
  return 1;
}

/*
 * Gets argument count in the value passed by pipe and sets up IO redirect
 * Continues normal execution if nothing else happens
 */
int prex(char **argv) {
  // printf("Got to pre-executor\n");
  int argc = 0; // Length of the string to process is in this
  while(argv[argc] != NULL)
    argc++;

  // printf("Printing command that's gonna be executed\n");
  // for(int i = 0; i < argc; i++) {
  //   printf("%s ", argv[i]);
  // }
  // printf("\n\n");
  //
  // for(int i = 0; i < argc; i++) {
  //   for(int j = 0; j < strlen(argv[i]); j++)
  //     printf("%d ", (int)argv[i][j]);
  //   printf("\t");
  // }
  // printf("\n\n");

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

  // printf("Inside process: thing running is\n");
  // for(int i = 0; i < argc; i++) {
  //   printf("%s ", argv[i]);
  // }
  // printf("\n\n");

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
