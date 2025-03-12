#include "include/kernel.h"
#include "include/screen.h"
#include "include/keyboard.h"
#include "include/shell.h"
#include "include/commands.h"

void main() {
    clear_screen();
    
    kprint("Kernel started\n");
    kprint("Type 'help' for a list of commands.\n\n");
    init_keyboard();
    register_commands();
    char input_buffer[256];
    int i;
    while(1) {
        for (i = 0; i < 256; i++) {
            input_buffer[i] = 0;
        }
        
        kprint("> ");
        keyboard_read_line(input_buffer, 256);
        
        if (input_buffer[0] != '\0') {
            execute_command(input_buffer);
        }
    }
}