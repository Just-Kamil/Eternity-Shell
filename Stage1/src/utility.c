#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utility.h"
extern char **environ;




// COMMANDS //

int clr() {
  system("clear");
  // returning 0 here to show that the program completed successfully
  return 0;
}

int dir(char* arguments) {
  // build a command skeleton

  // allocate memory for the basic ls command
  char* command = calloc(4, sizeof(char));

  // check if user provided own ls arguments
  if(arguments[1] == '-') {
    strcat(command, "ls ");
  } else {
    command = realloc(command, sizeof(char) * 8);
    strcat(command, "ls -la ");
  }

  // expand the memory based on the size of the gven command
  command = realloc(command, sizeof(char) * 3 +  sizeof(char) * strlen(arguments));

  // join the arguments together
  strcat(command, arguments);

  // call time
  system(command);
  free(command);
  return 0;
}

int getEnviron() {
  return 0;
}

// END OF COMMANDS //


// SIMPLESHELL.C UTILS //


// takes in all arguments after a given command
// merges them and adds spaces
// returns the resulting string
char* serialiseArgument(char** args) {

        // reset pointer
       char** arg = args;

        // adjust for command
        *arg++;

        // aggregate all the arguments
        char* catArgs = calloc(strlen(*arg), sizeof(char));


        while(*arg != NULL) {
          catArgs = realloc(catArgs, sizeof(char) * strlen(catArgs) + sizeof(char) * strlen(*arg));
          // strcat(catArgs, *arg++);
          sprintf(catArgs,"%s %s", catArgs, *arg++);
        }
      
      return catArgs;

}
