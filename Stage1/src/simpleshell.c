#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

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
  char* shellShort = "etsh";


  // change shell envVar
  setEnvironShell(argv[0]);
  
  while (!feof(stdin)) {

    fputs(prompt, stdout);

    // get arguments and tokenise
    if (fgets(buf, MAX_BUBBER, stdin)) {
      arg = args;
      *arg++ = strtok(buf, SEPARATORS); 
    }

    while ((*arg++ = strtok(NULL,SEPARATORS)));


    //if anything inputted
    if (args[0]) {
      
      char* catArgs = serialiseArgument(args);

      /* COMMANDS */

      // clear
      if (!strcmp(args[0], "clr")) {clr(); continue;}

      // exit
      if (!strcmp(args[0], "exit")) {exit(0);}

      // dir
      if (!strcmp(args[0], "dir")) {dir(catArgs); free(catArgs); continue;}

      // echo
      if (!strcmp(args[0], "echo")) {echo(catArgs); free(catArgs); continue;}

      // cd
      if (!strcmp(args[0], "cd")) {cd(catArgs); free(catArgs); continue;}

      // environ
      if (!strcmp(args[0], "environ")) {environGet(); continue;}


      // if not explicitly stated
      printf("%s: %s, Command Not Found.\n", shellShort, args[0]);

      /* END OF COMMANDS */

    } // END OF COMMAND CHECK

  }


  return 0;
}
