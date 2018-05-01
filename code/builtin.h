#ifndef BUILTIN_H
#define BUILTIN_H

#include "defs.h"

extern char promt[PRMTLEN];

int cd(char* cmd);

int exit_shell(char* cmd);

int pwd(char* cmd);

#define PATH_LENGHT 1024 

#endif // BUILTIN_H
