#ifndef UTILS_H
#define UTILS_H

#include "defs.h"
#include "stdbool.h"
char* split_line(char* buf, char splitter);

int block_contains(char* buf, char c);

char** str_split(char* a_str, const char a_delim);

bool setEnviromentVariables(char *cmd);

void free_char_array(char **wl);
#endif // UTILS_H
