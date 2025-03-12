#ifndef COMMANDS_H
#define COMMANDS_H

typedef struct {
    const char* name;         
    const char* description;  
    void (*function)(const char* args); 
} command_t;

void register_commands();
int execute_command_by_name(const char* name, const char* args);
void list_commands();

void cmd_hello(const char* args);
void cmd_echo(const char* args);
void cmd_poweroff(const char* args);
void cmd_help(const char* args);

#endif 