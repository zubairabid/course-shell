#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include "util.h"
#include "disp.h"

#define DELIM_TOKEN " \t\r\f\a\n"
#define COMMAND_DELIM_TOKEN ";"
#define BUF_DEFAULT 128


/*
 * Splits each line into tokens by DELIM_TOKEN
 * Stores number of tokens into pointer variable 'argc'
 *
 * Returns: Array of tokens **tokens
 */
char **parseline(char *line, int *argc) {

  // bsize Defines expected size of array. Default BUF_DEFAULT
  // count is used as a counter variable to assign values to array
  int bsize = BUF_DEFAULT, count = 0;

  char *token;
  char **tokens = (char**)malloc(bsize * sizeof(char*));

  for(int i = 0; i < bsize; i++) {
    tokens[i] = NULL;
  }

  // Tokenizing the string: initial token extracted
  token = strtok(line, DELIM_TOKEN);
  do {

    // Add token value from previous iteration into array of tokens
    tokens[count++] = token;
    strcat(tokens[count-1], "\0"); // null terminating strings for bugfix

    // In case of overflow of array, reassign memory
    if(count >= bsize) {
      bsize += BUF_DEFAULT;
      tokens = realloc(tokens, bsize * sizeof(char*));
    }

    // extract the next token
    token = strtok(NULL, DELIM_TOKEN);
  } while(token != NULL);
  for(int i = count + 1; i < bsize; i++) {
    tokens[i] = NULL;
  }
  // tokens[count++] = "\0";
  // Saves the number of tokens extracted into pointer 'argc'
  *argc = count;

  // Return the array of tokens
  return tokens;

}


/*
 * Splits the command into separate commands based on COMMAND_DELIM_TOKEN
 * Stores number of commands into pointer variable 'num'
 *
 * Returns: Array of commands **commands
 */
char **splitlines(char *line, int *num) {

  // bsize Defines expected size of array. Default BUF_DEFAULT
  // count is used as a counter variable to assign values to array
  int bsize = BUF_DEFAULT, count = 0;

  char *command;
  char **commands = (char**)malloc(bsize * sizeof(char*));

  // Tokenizing the string: initial token extracted
  command = strtok(line, COMMAND_DELIM_TOKEN);
  do {

    // Add token value from previous iteration into array of tokens
    commands[count++] = command;
    strcat(commands[count-1], "\0"); // null terminating strings for bugfix

    // In case of overflow of array, reassign memory
    if(count >= bsize) {
      bsize += BUF_DEFAULT;
      commands = realloc(commands, bsize * sizeof(char*));
    }

    // extract the next token
    command = strtok(NULL, ";");
  } while(command != NULL);
  // commands[count++] = "\0";
  // Saves the number of tokens extracted into pointer 'num'
  *num = count;

  return commands;

}


// SIGNAL HANDLERS

void sig_int(int signum) {

}

void sig_stop(int signum) {

}

void sig_tstp(int signum) {

}

void sig_child(int signum) {
  // perror("\nProcess terminated");
  return;
}
