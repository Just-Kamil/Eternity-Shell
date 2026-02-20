#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "utility.h"

// this was a typo, however it is so funny to me that I'm keeping it
#define MAX_BUBBER 1024
#define MAX_ARGS 64
#define SEPARATORS " \t\n"

int main(int argc, char *argv[])
{
  char buf[MAX_BUBBER];
  char * args[MAX_ARGS];
  char ** arg;
  char * prompt = "֍ ⇝ ";
  
  while (!feof(stdin)) {

    fputs(prompt, stdout);

    // get arguments and tokenise
    if (fgets(buf, MAX_BUBBER, stdin)) {
      arg = args;
      *arg++ = strtok(buf, SEPARATORS); 
    }

    while ((*arg++ = strtok(NULL,SEPARATORS)));


    //if anything inputed
    if (args[0]) {
      
      // clear
      if (!strcmp(args[0], "clr")) {clr(); continue;}

      if (!strcmp(args[0], "exit")) {exit(0);}

      if (!strcmp(args[0], "dir")) {
        // reset pointer
        arg = args;

        // adjust for command
        *arg++;

        // aggregate all the arguments
        char* catArgs = calloc(strlen(*arg), sizeof(char));


        while(*arg != NULL) {
          catArgs = realloc(catArgs, sizeof(char) * strlen(catArgs) + sizeof(char) * strlen(*arg));
          // strcat(catArgs, *arg++);
          sprintf(catArgs,"%s %s", catArgs, *arg++);
        }
        dir(catArgs);
        free(catArgs);
        continue;

      }

      arg = args;
      while (*arg) {
        fprintf(stdout, "%s ", *arg++);
        fputs("\n", stdout);
      }

    }

  }


  return 0;
}
