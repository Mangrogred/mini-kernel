#include "../include/shell.h"
#include "../include/screen.h"
#include "../include/io.h"
#include "../include/keyboard.h"
#include "../include/commands.h"
#include <stddef.h>

int strcmp(const char* str1, const char* str2) {
    int i = 0;
    
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return str1[i] - str2[i];
        }
        i++;
    }
    
    return str1[i] - str2[i];
}

void strcpy(char* dest, const char* src) {
    int i = 0;
    
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    
    dest[i] = '\0';
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

void parse_command(char* input, char* command, char* args) {
    int i = 0;
    int j = 0;
    
    while (input[i] == ' ' && input[i] != '\0') {
        i++;
    }
    
    while (input[i] != ' ' && input[i] != '\0') {
        command[j] = input[i];
        j++;
        i++;
    }
    command[j] = '\0';
    
    while (input[i] == ' ' && input[i] != '\0') {
        i++;
    }
    
    j = 0;
    while (input[i] != '\0') {
        args[j] = input[i];
        j++;
        i++;
    }
    args[j] = '\0';
}

void poweroff() {
    outb(0xf4, 0x00);
    outw(0xB004, 0x2000);
    kprint("Failed to power off. It is now safe to turn off your computer.\n");
    for(;;);
}

void execute_command(const char* input) {
    char command[64];
    char args[192];
    
    int i;
    for (i = 0; i < 64; i++) {
        command[i] = 0;
    }
    for (i = 0; i < 192; i++) {
        args[i] = 0;
    }
    
    char input_copy[256];
    for (i = 0; i < 256; i++) {
        input_copy[i] = 0;
    }
    
    for (i = 0; i < 255 && input[i] != '\0'; i++) {
        input_copy[i] = input[i];
    }
    input_copy[i] = '\0';
    
    parse_command(input_copy, command, args);

    if (command[0] == '\0') {
        return;
    }
    
    if (!execute_command_by_name(command, args)) {
        kprint("Unknown command: ");
        kprint(command);
        kprint("\n");
    }
} 