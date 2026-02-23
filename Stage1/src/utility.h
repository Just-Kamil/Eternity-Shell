
#ifndef UTILITY_H_
#define UTILITY_H_


int clr();
int dir(char* args);
int environGet();
int echo(char* arguments);

char* serialiseArgument(char** args);
int setEnvironShell(char* programArg);


#endif // UTILITY_H_
