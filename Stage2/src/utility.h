
#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <linux/limits.h>

int clr();
int environGet();
int sysPause();
int help(char* arguments);
int cd(char* arguments);
int dir(char* arguments);
int echo(char* arguments);
int forkExec(char* arguments[]);

extern char cwd[PATH_MAX];
char* serialiseArgument(char** args);
int setEnvironShell(char* programArg);

int handleRedirect(char** args);
int replaceIn(char* filePath);
int replaceOut(char* filePath, int mode, int create);
char** checkRedirect(char** args);
char* lastString(char* stringArray[]);
int stringArrayLength(char* stringArray[]);


#endif // UTILITY_H_
