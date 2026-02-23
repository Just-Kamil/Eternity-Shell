#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>

#include "utility.h"
extern char **environ;




// COMMANDS //

/// calls the system clear command
int clr() {
  system("clear");
  // returning 0 here to show that the program completed successfully
  return 0;
}

/// @brief Lists files in a directory using ls
/// @param arguments parameters passed down to ls, "-la" by default if no ls args provided
/// @return 0 if program completes successfully
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

  // expand the memory based on the size of the given command
  command = realloc(command, sizeof(char) * 3 +  sizeof(char) * strlen(arguments));

  // join the arguments together
  strcat(command, arguments);

  // call time
  system(command);
  free(command);
  return 0;
}

/// @brief Prints a message into the terminal
/// @param arguments message to print
/// @return 0 if program exits successfully
int echo(char* arguments) {
  printf("%s\n", arguments);
  return 0;
}


int environGet() {
  char** envHead = environ;
  while (*envHead != NULL) {printf("%s\n", *envHead); *envHead++;}
  return 0;
}

int cd(char* arguments) {
  return 0;
}

int help() {
  return 0;
}

int pause() {
  return 0;
}

// END OF COMMANDS //


// SIMPLESHELL.C UTILS //


// you can tell I'm taking this seriously because I looked up the doxygen style guide
/*
* Returns a concatenated version of arguments for use in passing to commands
*
* @param args an array of strings
* 
* @return a string of all passed strings with spaces separating them
*/
char* serialiseArgument(char** args) {

        // reset pointer
       char** arg = args;

        // adjust for command
        *arg++;

        // check if there are arguments at all
        // if not, return *some* kind of pointer so that free can be used later
        if (*arg == NULL) {return calloc(0, sizeof(char));}

        // aggregate all the arguments
        char* catArgs = calloc(strlen(*arg), sizeof(char));

        while(*arg != NULL) {
          catArgs = realloc(catArgs, sizeof(char) * strlen(catArgs) + sizeof(char) * strlen(*arg));
          // strcat(catArgs, *arg++);
          sprintf(catArgs,"%s %s", catArgs, *arg++);
        }
      
      return catArgs;

}


/// @brief Sets the shell env var to this shell
/// @return 0 upon success
int setEnvironShell(char* programArg) {

  // build the string

  // the current directory as a string
  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));

  // prefix for putenv
  char* expArg = "SHELL=";

  // i hate this
  // WHY IS IT AN INT????? WHY? THERE IS LITERALLY A CHAR TYPE IN C, JUST USE THAT
  
  // name of the shell (fr this time)
  char* progName = strrchr(programArg, 47);

  // allocate enough memory for the final string
  char* finalExport = calloc(strlen(cwd) + strlen(expArg) + strlen(progName), sizeof(char));

  // concatenate the strings
  finalExport = strcat(finalExport, expArg);
  finalExport = strcat(finalExport, cwd);
  finalExport = strcat(finalExport, progName);

  // set the env var
  putenv(finalExport);

  // DO NOT free memory taken up by finalExport, environ uses that var
  // free(finalExport);

  return 0;
}
