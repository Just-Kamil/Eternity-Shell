#include <termios.h>
#include "utility.h"
extern char **environ;

char cwd[PATH_MAX];


// COMMANDS //

/// @brief calls the system clear command
/// @return 0 when completed
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
  command = realloc(command, sizeof(char) * 3 +  sizeof(char) * strlen(arguments) + 1);

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

/// @brief prints line by line each argument in environ
/// @return 0 when completed
int environGet() {
  char** envHead = environ;
  while (*envHead != NULL) {printf("%s\n", *envHead); *envHead++;}
  return 0;
}

/// @brief Changes directory to path given, otherwise prints current directory
/// @param arguments path to a directory
/// @return 0 if no errors, otherwise returns error code
int cd(char* arguments) {

  // cwd is set from setting the env variable earlier
  // if the argument is empty, print current directory
  if (!strlen(arguments)) {printf("%s\n", cwd); return 0;}

  // have to move one to ignore the leading empty char
  arguments++;

  int res = chdir(arguments);


  if (res == 0) {

    // set env var of PWD to new dir
    getcwd(cwd, sizeof(cwd));

    // set the env var to the now current dir
    setenv("PWD", cwd, 1);

    return 0;
  } else { // some error occurred
    switch (errno)
    {
    case EACCES:
      // no perms 
      printf("cd: You do not have the permission to change into this dir\n");
      return EACCES;
    case EFAULT:
      // something im sure
      // https://linux.die.net/man/2/chdir
      printf("cd: path points outside your accessible address space.\n");
      return EFAULT;
    case EIO:
      // I/O error somehow
      printf("cd: your input was invalid\n");
      return EIO;

    case ELOOP:
      // too many symlinks
      printf("cd: too many symlinks when trying to resolve the path\n");
      return ELOOP;

    case ENAMETOOLONG:
      // the path name was too long
      printf("cd: the path name was too long, somehow\n");
      return ENAMETOOLONG;
    
    case ENOENT:
      // file (dir) ain't real
      printf("cd: directory does not exist\n");
      return ENOENT;
    
    case ENOTDIR:
      // tried to cd into a file
      printf("cd: path to file, not a directory\n");
      return ENOTDIR;

    }
  }
  // im sure there is a chance someone somehow ends up here
  return 0;
}


int help(char* arguments, char* runDir) {
  if (!strlen(arguments)) {system("more ../manual/help.txt"); return 0;}
  return 0;
}

/// @brief Pauses the program until ENTER is inputted
/// @return 0 upon completion
int sysPause() {
  // disabling buffer from here
  // https://shtrom.ssji.net/skb/getc.html


  // declare two variables
  // one to store the old terminal settings
  // one to set the new ones
  struct termios old_tio, new_tio;

  // get the current terminal settings into the addr of old_tio
  tcgetattr(STDIN_FILENO, &old_tio);

  // copy all the settings to the new variable
  new_tio = old_tio;

  // basically flip the bit flags that determine if the terminal has ICANON and ECHO
  // the two flags that are responsible for printing typed text onto the terminal
  new_tio.c_lflag &=(~ICANON & ~ECHO);

  // apply changes to the terminal
  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

  // for for enter to be inputted
  while (getchar() != 10);

  // restore old settings
  tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
  return 0;
}

// END OF COMMANDS //


// SIMPLESHELL.C UTILS //

/// @brief Returns a concatenated version of arguments to be passed to commands
/// @param args an array of strings
/// @return a string of all passed arguments with spaces separating them
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

        // reallocate memory and concatenate the string until the args run out
        while(*arg != NULL) {
          // needed to add 2 to the end of realloc,
          // to accommodate the fact the strlen() doesn't count the terminating char
          // which would cause the program to write memory where it shouldn't and it would make fclose() sad :(
          catArgs = realloc(catArgs, sizeof(char) * strlen(catArgs) + sizeof(char) * strlen(*arg) + 2);
          sprintf(catArgs,"%s %s", catArgs, *arg++);
        }
      
      return catArgs;

}


/// @brief Sets the shell env var to this shell
/// @return 0 upon success
int setEnvironShell(char* programArg) {

  // this function used to be really long, then I discovered /proc/self/exe
  // now it's 4 lines of code
  // so, shoutouts to /proc/self/exe

  // create a var to store the path from readlink
  char buff[PATH_MAX];

  // /proc/self/exe is a symlink to whatever application is running
  // so we take the resolve from that and put it into buff
  readlink("/proc/self/exe", buff, sizeof(buff));

  // set the env var
  setenv("SHELL", buff, 1);

  return 0;
}
