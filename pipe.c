#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "pipe.h"
#include "operation.h"
#include "util.h"

// char*[][] pipe_split(char **argv, int argc) {
//   char *args[128][128];
//
//   int m = 0, n = 0;
//   for (int i = 0; i < argc; i++) {
//     if (strcmp(argv[i],"|") != 0) {
//       args[m][n++] = argv[i];
//     }
//     else {
//       args[m][n] = NULL;
//       m++;
//       n = 0;
//     }
//   }
//   args[++m][0] = NULL;
//
//   // printf("Split pipes, now printing\n");
//   //
//   // for (int i = 0; i <= m; i++) {
//   //   for (int j = 0; args[i][j] != NULL; j++)
//   //     printf("%s ", args[i][j]);
//   //   printf("\n");
//   // }
//
//   // fargs = (char***)args;
//   return args;
// }
