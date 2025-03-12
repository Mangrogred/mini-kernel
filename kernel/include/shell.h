#ifndef SHELL_H
#define SHELL_H

#include <stddef.h> 

void execute_command(const char* command);
void parse_command(char* input, char* command, char* args);

int strcmp(const char* str1, const char* str2);
void strcpy(char* dest, const char* src);
size_t strlen(const char* str);

void poweroff();

#endif 