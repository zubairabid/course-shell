#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "ls.h"


int shls(char **argv, int argc) {
  struct dirent **namelist;
  struct stat fdetails;
  int n, i, opt, flags;

  char *dir = (char*)malloc(PATH_MAX*sizeof(char)), *filename, *filepath, *perm;
  perm = (char*)malloc(10*sizeof(char));



  // while ((opt = getopt(argc, argv, "la")) != -1) {
  //   printf("%c\n", opt);
  //   switch(opt) {
  //     case 'l':
  //       printf("l got\n");
  //       break;
  //     case 'a':
  //       printf("a got\n");
  //       break;
  //     case '?':
  //       printf("%c got \n", opt);
  //       break;
  //     default:
  //       printf("What got\n");
  //   }
  // }


  if (argc >= 2) {
    if (argv[1][0] == '-')
      if (argc == 3)
        dir = argv[2];
      else
        dir = ".";
    else
      dir = argv[1];
  }
  else {
    dir = ".";
  }
  printf("%s\n", dir);

  // Gets a list of all files in the namelist as a dirent struct
  n = scandir(dir, &namelist, NULL, alphasort);



  if (n < 0) {
    // Error
  }
  else {
    // Printing the details
    for (i = 0; i < n; i++) {

      filename = namelist[i]->d_name;

      filepath = (char*)malloc((strlen(dir) + strlen(filename) + 2)*sizeof(char));
      strcpy(filepath, dir);
      strcat(filepath, "/");
      strcat(filepath, filename);

      // Get file data
      lstat(filepath, &fdetails);

      // Setting up permissions printing et al
      strcpy(perm, "----------");
      if(S_ISDIR(fdetails.st_mode))
        perm[0] = 'd';
      if(fdetails.st_mode & S_IRUSR)
        perm[1] = 'r';
      if(fdetails.st_mode & S_IWUSR)
        perm[2] = 'w';
      if(fdetails.st_mode & S_IXUSR)
        perm[3] = 'x';
      if(fdetails.st_mode & S_IRGRP)
        perm[4] = 'r';
      if(fdetails.st_mode & S_IWGRP)
        perm[5] = 'w';
      if(fdetails.st_mode & S_IXGRP)
        perm[6] = 'x';
      if(fdetails.st_mode & S_IROTH)
        perm[7] = 'r';
      if(fdetails.st_mode & S_IWOTH)
        perm[8] = 'w';
      if(fdetails.st_mode & S_IXOTH)
        perm[9] = 'x';

      printf("%s\t%d\t%d\t%ld\t%s\n", perm, fdetails.st_uid, fdetails.st_gid, fdetails.st_size, namelist[i]->d_name);

      free(namelist[i]);
      free(filepath);
    }
    free(namelist);
  }

  return 1;
}
