
#ifndef UTILITY_H_
#define UTILITY_H_


int clr();
int environGet();
int cd(char* arguments);
int dir(char* arguments);
int echo(char* arguments);


char* serialiseArgument(char** args);
int setEnvironShell(char* programArg);


#endif // UTILITY_H_
