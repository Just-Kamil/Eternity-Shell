
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
int cd(char* arguments);
int dir(char* arguments);
int echo(char* arguments);

extern char cwd[PATH_MAX];
char* serialiseArgument(char** args);
int setEnvironShell(char* programArg);


#endif // UTILITY_H_
