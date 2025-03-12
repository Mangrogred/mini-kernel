#include "../include/commands.h"
#include "../include/screen.h"
#include "../include/io.h"
#include "../include/shell.h"

extern int strcmp(const char* str1, const char* str2);

#define MAX_COMMANDS 10
static command_t commands[MAX_COMMANDS];
static int command_count = 0;

void add_command(const char* name, const char* description, void (*function)(const char* args)) {
    if (command_count < MAX_COMMANDS) {
        commands[command_count].name = name;
        commands[command_count].description = description;
        commands[command_count].function = function;
        command_count++;
    }
}

void register_commands() {
    command_count = 0;
    
    add_command("hello", "Displays a greeting", cmd_hello);
    add_command("echo", "Displays the arguments", cmd_echo);
    add_command("poweroff", "Shuts down the computer", cmd_poweroff);
    add_command("help", "Displays available commands", cmd_help);
}

int execute_command_by_name(const char* name, const char* args) {
    int i;
    for (i = 0; i < command_count; i++) {
        if (strcmp(commands[i].name, name) == 0) {
            commands[i].function(args);
            return 1; 
        }
    }
    return 0; 
}

void list_commands() {
    kprint("Available commands:\n");
    
    int i;
    for (i = 0; i < command_count; i++) {
        kprint("  ");
        kprint(commands[i].name);
        kprint(" - ");
        kprint(commands[i].description);
        kprint("\n");
    }
}

void cmd_hello(const char* args __attribute__((unused))) {
    kprint("Hello, World!\n");
}

void cmd_echo(const char* args) {
    kprint(args);
    kprint("\n");
}

void cmd_poweroff(const char* args __attribute__((unused))) {
    kprint("Shutting down...\n");
    outb(0xf4, 0x00);
    outw(0xB004, 0x2000);
    kprint("Failed to power off. It is now safe to turn off your computer.\n");
    for(;;);
}

void cmd_help(const char* args __attribute__((unused))) {
    list_commands();
} 