#include "utility.h"

// this was a typo, however it is so funny to me that I'm keeping it
#define MAX_BUBBER 1024
#define MAX_ARGS 64
#define SEPARATORS " \t\n"


// char cwd[PATH_MAX];
char* shellShort = "etsh";
void commandParser(char *args[64], int* retFlag);
int fileProcessor(char* fileName);


// TODO
// [X] External Commands
// [ ] Background Execution
// [ ] I/O Redirect


int main(int argc, char *argv[]){
  char buf[MAX_BUBBER];
  char * args[MAX_ARGS];
  char ** arg;
  char * prompt = "\x1b[42m↳\x1b[0m\x1b[32m\x1b[1m֍ ⇝ \x1b[0m";
  extern char cwd[PATH_MAX];

  // sets currently working dir var for use later
  getcwd(cwd, sizeof(cwd));

  // sets batch mode
  if (argv[1] != NULL) {
      // tries to run a batch file
      if(fileProcessor(argv[1]) == -1) {
            printf("Invalid Script File");
            // if it doesn't work return the program with an errored state
            return -1;
      } else {
            return 0;
      }
  }

  // change shell envVar
  setEnvironShell(argv[0]);


  // execute a .etshrc if one exists in the homedir
  char* home = getenv("HOME");

  char* homeRC = calloc(strlen(home) + strlen("/.etshrc") + 2, sizeof(char));
  sprintf(homeRC, "%s/.etshrc", home);

  fileProcessor(homeRC);
  free(homeRC);

  
  while (!feof(stdin)) {

    printf("\x1b[37m\x1b[42m %s \x1b[0m\n", cwd);
    fputs(prompt, stdout);

    // get arguments and tokenise
    if (fgets(buf, MAX_BUBBER, stdin)) {
      arg = args;
      *arg++ = strtok(buf, SEPARATORS); 
    }

    while ((*arg++ = strtok(NULL,SEPARATORS)));

    // if anything inputted

    // have a way to continue when needed
    int retFlag;

    // process the command
    commandParser(args, &retFlag);

    // check if there was a continue flag set
    if (retFlag == 3)
          continue; 
  }

  return 0;
}

/// @brief Takes in arguments as an array of strings and executes commands based on the input
/// @param args an array of string arguments, with the first being the command name
/// @param retFlag flag that allows continue to be run after a command is executed
void commandParser(char *args[64], int* retFlag){
      *retFlag = 1;
      if (args[0])
      {

            char *catArgs = serialiseArgument(args);

            /* COMMANDS */

            // clear
            if (!strcmp(args[0], "clr"))
            {
                  clr();
                  {
                        *retFlag = 3;
                        return;
                  };
            }

            // exit
            if (!strcmp(args[0], "exit"))
            {
                  exit(0);
            }

            // dir
            if (!strcmp(args[0], "dir"))
            {
                  dir(catArgs);
                  free(catArgs);
                  {
                        *retFlag = 3;
                        return;
                  };
            }

            // echo
            if (!strcmp(args[0], "echo"))
            {
                  echo(catArgs);
                  free(catArgs);
                  {
                        *retFlag = 3;
                        return;
                  };
            }

            // cd
            if (!strcmp(args[0], "cd"))
            {
                  cd(catArgs);
                  getcwd(cwd, sizeof(cwd));
                  free(catArgs);
                  {
                        *retFlag = 3;
                        return;
                  };
            }

            // environ
            if (!strcmp(args[0], "environ"))
            {
                  environGet();
                  {
                        *retFlag = 3;
                        return;
                  };
            }

            // script
            if (!strcmp(args[0], "script"))
            {
                  int res = fileProcessor(args[1]);
                  free(catArgs);
                  if (res == -1) {printf("Invalid Script\n");}
                  {
                        *retFlag = 3; 
                        return;
                  };
            }

            // pause
            if (!strcmp(args[0], "pause"))
            {
                  sysPause();
                  {
                        *retFlag = 3;
                        return;
                  };
            }

            // help
            if (!strcmp(args[0], "help"))
            {
                  help(catArgs);
                  free(catArgs);
                  {
                        *retFlag = 3;
                        return;
                  }
            }

            // if not explicitly stated

            // fork an instance of this shell, then execute the command
            int forkRes = forkExec(args);

            if (forkRes)
            printf("%s: %s, Command Not Found.\n", shellShort, args[0]);

            /* END OF COMMANDS */
      }
}

/// @brief Executes commands from a given file 
/// @param fileName file which is to be run
/// @return -1 if the file is invalid
int fileProcessor(char *fileName){

    // open the file
    FILE* fptr = fopen(fileName, "r");

    // declare the same things we do in main()
    char buf[MAX_BUBBER];
    char* args[MAX_ARGS];
    char** arg;

    // check if file opened successfully
    if (fptr == NULL) {return -1;}

    // read in each line and tokenise
    while(fgets(buf, MAX_BUBBER, fptr)) {
      
      arg = args;
      *arg++ = strtok(buf, SEPARATORS); 

      while ((*arg++ = strtok(NULL,SEPARATORS)));

      // execute the command
      int retFlag;
      commandParser(args, &retFlag);
      if (retFlag == 3) {continue;}
    }
    
    // close the file
    fclose(fptr);

    return 0;
}
