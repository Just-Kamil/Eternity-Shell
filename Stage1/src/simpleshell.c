#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/limits.h>

#include "utility.h"

// this was a typo, however it is so funny to me that I'm keeping it
#define MAX_BUBBER 1024
#define MAX_ARGS 64
#define SEPARATORS " \t\n"


// TODO
// [ ] make the while loop check whether or not it's in batch mode
// [ ] make manual
// [ ] make batch mode
// [ ] implement help
// [ ] implement pause
// [x] add the cwd to the prompt

// *SPECIAL* TODO
//  [ ] make .etshrc functionality
//  [x] funny colours


int main(int argc, char *argv[])
{
  char buf[MAX_BUBBER];
  char * args[MAX_ARGS];
  char ** arg;
  char * prompt = "\x1b[42m↳\x1b[0m\x1b[32m\x1b[1m֍ ⇝ \x1b[0m";
  char* shellShort = "etsh";

  char cwd[PATH_MAX];
  getcwd(cwd, sizeof(cwd));


  // change shell envVar
  setEnvironShell(argv[0]);
  
  while (!feof(stdin)) {

    printf("\x1b[37m\x1b[42m%s\x1b[0m\n", cwd);
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
      if (!strcmp(args[0], "cd")) {cd(catArgs); getcwd(cwd, sizeof(cwd)); free(catArgs); continue;}

      // environ
      if (!strcmp(args[0], "environ")) {environGet(); continue;}


      // if not explicitly stated
      printf("%s: %s, Command Not Found.\n", shellShort, args[0]);

      /* END OF COMMANDS */

    } // END OF COMMAND CHECK

  }


  return 0;
}
