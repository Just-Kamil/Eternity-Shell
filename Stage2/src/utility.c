#include <termios.h>
#include "utility.h"
#include <bits/waitflags.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
extern char **environ;
extern FILE* curOut;

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
  printf( "%s\n", arguments);
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

/// @brief Displays help information from a file using the 'more' command
/// @param arguments a topic that the user wants more info on 
/// @return 0 when completed
int help(char* arguments) {

  // ok so, I decided that the manual is going to be copied and placed into whatever dir the script will be running from
  // this is reflected in the make file
  // I did this so that the program can be compiled and somewhat moved around.

  char progPath[PATH_MAX];

  // have to clear the string to remove garbage data that may interfere with setting the path
  memset(progPath, 0, sizeof(progPath));

  // get the path of the shell
  readlink("/proc/self/exe", progPath, sizeof(progPath));

  // the position of the last slash
  char* lastSlash = strrchr(progPath, '/');

  // add an end of string char after the slash
  *lastSlash++ = '\0';

  // if help is just run on its own
  if (!strlen(arguments)) {
    // PATH/man/readme.txt
    char* command = calloc(strlen("more /man/readme.txt") + strlen(progPath) + 2, sizeof(char));
    sprintf(command, "more %s/man/readme.txt", progPath);

    system(command);
    free(command);
    return 0;
  }

  // specific about

  // PATH/man/{arguments}.txt

  // move arguments by one to adj for whitespace
  *arguments++;

  // the string that will be used to check if the help article exists
  char* file = calloc(strlen("/man/.txt") + strlen(progPath) + strlen(arguments) + 3, sizeof(char));
  sprintf(file,"%s/man/%s.txt", progPath, arguments);
  
  if (access(file, F_OK) == 0) {
  // the command that will be run with system
  char* command = calloc(strlen("more ") + strlen(file) + 2, sizeof(char));
  sprintf(command, "more %s", file);

  system(command);

  free(command);
  free(file);

  } else {
    printf("no help article found for %s\n", arguments);
    free(file);
  }

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

/// @brief forks an instance of simpleshell/eternity shell and then replaces it with the given process and passes arguments to it
/// @param arguments An array of strings terminating with a NULL
/// @return exit status, 0 if successful
int forkExec(char* arguments[]) {

  int status;
  int bg_flag;

  // find the last arg in arguments
  char* lastArg = lastString(arguments);

  // change the flag if the last arg is an ampersand
 if (lastArg[0] == '&') {bg_flag = 1;} else {bg_flag = 0;}

  int argc = stringArrayLength(arguments);

  int forkRes = fork();

  switch (forkRes)
  {
  case -1:
    // something went wrong
    return -1;
    break;
  
  case 0:
    // execute whatever it was we wanted to
    // remove the last argument so that '&' does not get passed to the function


    if (bg_flag) {

      // find the length of arguments
      int argc = stringArrayLength(arguments);

      // create a new array to store the commands
      char* newArgs[64];

      // copy items up until &
      memcpy(newArgs, arguments, (argc - 1) * sizeof(char*));

      // set the last arg to NULL so that execvp doesn't throw a fit
      newArgs[argc - 1] = NULL;

      // this basically sends stdout to the ether, done so apps in the bg don't show in the terminal
      int file_null = open("/dev/null", O_WRONLY | O_APPEND);

      // 1 is stdout, so we replace it with /dev/null
      dup2(file_null, 1);

      // execute the command
      execvp(newArgs[0], newArgs);

      close(file_null);

      // if this executes *something* went wrong, write the error
      // stderr is not replaced so the user will be able to see it
      fprintf(stderr, "Command was unable to run, errno: %d \n", errno);
      exit(errno);

    } else {

      // we don't reed to actually handle the redirects because serialiseArguments already does that
      // check if there are any redirect symbols
      char** redirectPos = checkRedirect(arguments);

      // there is a redirect, truncate the inputs to before the symbol
      if (redirectPos != NULL) {
        // get the length of arguments
        int argc = stringArrayLength(arguments);

        // find the length of the array from the returned pointer
        int neoArgc = stringArrayLength(redirectPos);

        // the position that we want to copy until is the difference between the arg counts
        int symPos = argc - neoArgc;


        // variable to store new args
        char* newArgs[64];

        // copy the args over up until the symbol
        memcpy(newArgs, arguments, symPos * sizeof(char*));

        // add a null at the end to keep execvp happy
        newArgs[symPos + 1] = NULL;


        // execute the comand
        execvp(newArgs[0], newArgs);

        // something wrong, send error
        fprintf(stderr, "Command was unable to run, errno: %d \n", errno);
        //exit(errno);

      } else {
      // arguments needs to be an array of strings, it also MUST terminate with NULL
      execvp(arguments[0], arguments); 
      fprintf(stderr, "Command was unable to run, errno: %d \n", errno);

      // exit so that we don't stay in the fork
      exit(errno);
      }

    }

    // if something gets executed here, problem!
    // most likely the command doesn't exist
    return -1;
    break;
  
  default:
  if (!bg_flag)
    waitpid(forkRes, &status, WUNTRACED);
  }

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

  // check for redirection
  char** redirSym = checkRedirect(args);

  // if there are redirect arguments, handle them and handle any errors that could occur
  if (redirSym != NULL) if(handleRedirect(args) == -1) {fprintf(stderr, "etsh: Unable to preform redirection, errno: %d\n", errno); return NULL;}

  // aggregate all the arguments
  char* catArgs = calloc(strlen(*arg), sizeof(char));

  // reallocate memory and concatenate the string until the args run out
  while(*arg != NULL) {

    // check if we need to stop at redirSym
    if (redirSym != NULL && *redirSym == *arg) {break;}

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

// END OF SIMPLESHELL UTILS //

// OTHER UTILS //

/// @brief returns a pointer to the first instance of a redirect symbol
/// @param args an array of strings that consists of the arguments given
/// @return pointer to the first instance of a redirect symbol or NULL
char** checkRedirect(char** args) {
  // move through all of the arguments, find the first instance of either > < or >> then return a pointer to there

  // set the moving pointer
  char** arg = args;

  while (*arg != NULL) {
    // don't need to check for >> because no actual redirection is happening here
    if (*arg[0] == '>' || *arg[0] == '<') {return arg;}

    // move to next item
    *arg++;
  }

  // if there is no redirect, return NULL
  return NULL;
}


/// @brief replaces stdout with given file 
/// @param filePath file to replace stdout with
/// @param mode when set to 0, truncates. when set to 1 appends
/// @return retuns 0 upon success -1 on fail
int replaceOut(char* filePath, int mode, int create) {

  // default is set to truncate and write
  int oflags = O_WRONLY | O_TRUNC;

  // if the mode is 1 or higher, append
  if (mode) oflags = O_WRONLY | O_APPEND;

  // if the create flag is set, OR the flags with the create flag
  if (create)oflags =  oflags | O_CREAT;

  // open the file we will replace stdout with
  // when creating a file set the perms to -rw-r--r-- (0644) which is the default for files I think
  int file_redir = open(filePath, oflags, 0644);

  // if open fails, return -1
  if (file_redir == -1) return -1;

  // replace stdout with the file
  int status = dup2(file_redir, 1);

  // if dup2 fails, return -1
  if (status == -1) return -1;

  // everything worked
  return 0;

}

/// @brief replaces stdin with given file
/// @param filePath file to replace stdin with
/// @return returns 0 on success and -1 on failure
int replaceIn(char* filePath) {

  // open file
  int file_redir = open(filePath, O_RDONLY);

  // if open fails, return -1
  if (file_redir == -1) return -1;

  // replace stdin with file
  int status = dup2(file_redir, 0);

  // if dup2 fails, return -1
  if (status == -1) return -1;

  // if we got here everything is aok
  return 0;
}

int handleRedirect(char** args) {
  // check for all the symbols

  // I don't like doing it this way, and there probably is a better way. but ;)
  char** arg = args;

  // variables for the symbols
  char** gtSym = NULL;
  char** dgtSym = NULL;
  char** ltSym = NULL;

  // check for > and >>

  // traverse array looking for >
  while(*arg != NULL && *arg[0] != '>') {*arg++;}
  
  // if something was found
  if (*arg != NULL) {
    // check for >>

    // had to add an extra variable because pointers weren't working
    char* foundString = *arg;
    if (strlen(*arg) == 2 && foundString[1] == '>') dgtSym = arg;
    else gtSym = arg;
  }

  // check for <
  // reset pointer
  arg = args;
  while(*arg != NULL && *arg[0] != '<') {*arg++;}
  if (*arg != NULL) ltSym = arg;

  // change stdout as needed

  // >
  if (gtSym != NULL) {

    // set set the flag to create
    int createFile = 0;

    // the filename will be *after* the symbol
    char* filePath = *(gtSym  + 1);

    // check if we can write to file, if not make one
    if(access(filePath, W_OK) != 0) createFile = 1;

    // replace stdout with the file
    int status = replaceOut(filePath, 0, createFile);

    // if something goes wrong return -1
    if (status == -1) return -1;

  }

  // >>
  if (dgtSym != NULL) {
        // set set the flag to create
    int createFile = 0;

    // the filename will be *after* the symbol
    char* filePath = *(dgtSym  + 1);

    // check if we can write to file, if not; make one
    if(access(filePath, W_OK) != 0) createFile = 1;

    // replace stdout with the file
    int status = replaceOut(filePath, 1, createFile);

    // if something goes wrong return -1
    if (status == -1) return -1;
  }

  // change stdin as needed

  // <
  if (ltSym != NULL) {

    // the file argument
    char* filePath = *(ltSym + 1);

    // we need to be able to read the file
    if(access(filePath, R_OK) != 0) return -1;

    // replace stdin with our file
    int status = replaceIn(filePath);
    
    // if something goes wrong return -1
    if (status == -1) return -1;
    
  }

  // if we get here, then everything should have worked
  return 0;
}

/// @brief returns the last string in an array
/// @param stringArray an array of strings
/// @return pointer to the last string in the array
char* lastString(char* stringArray[]) {
  // start the pointer at the begining of the array
  char** string = stringArray;

  // traverse the array until the next item is NULL
  while (*(string + 1)  != NULL) {*string++;}

  // retrun the last string
  return *string;
}

/// @brief returns the length of a string array
/// @param stringArray and array of strings
/// @return the length of the array
int stringArrayLength(char* stringArray[]) {
  
  // var to store the length
  // set to 1 to adj offset
  int count = 1;

  // start the pointer at the begining of the array
  char** string = stringArray;

  // traverse list counting how many items there are
  while (*(string + 1) != NULL) {count++; *string++;}

  // return the count
  return count;
}