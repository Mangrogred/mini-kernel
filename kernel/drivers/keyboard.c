#include "../include/keyboard.h"
#include "../include/io.h"
#include "../include/screen.h"

#define KEYBOARD_BUFFER_SIZE 256
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static int buffer_pos = 0;

static int shift_pressed = 0;
static int caps_lock = 0;

void init_keyboard() {
    int i;
    for (i = 0; i < KEYBOARD_BUFFER_SIZE; i++) {
        keyboard_buffer[i] = 0;
    }
    buffer_pos = 0;
    
    kprint("Keyboard initialized.\n");
}

unsigned char get_input_keycode() {
    unsigned char keycode = 0;
    
    while((inb(KEYBOARD_STATUS_PORT) & 1) == 0);
    
    keycode = inb(KEYBOARD_DATA_PORT);
    
    if(keycode & 0x80) {
        if(keycode == 0xAA || keycode == 0xB6) {
            shift_pressed = 0;
        }
        return 0;
    }
    
    if(keycode == 0x2A || keycode == 0x36) {
        shift_pressed = 1;
        return 0;
    }
    
    if(keycode == 0x3A) {
        caps_lock = !caps_lock;
        return 0;
    }
    
    return keycode;
}

char get_ascii_char(unsigned char keycode) {
    static const char ascii_table[128] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
        '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    
    static const char ascii_table_shift[128] = {
        0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0, 0,
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A', 'S',
        'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V',
        'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
        '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    
    char ascii_char = 0;
    
    int is_letter = (keycode >= KEY_A && keycode <= KEY_L) || 
                    (keycode >= KEY_Z && keycode <= KEY_M) ||
                    (keycode >= KEY_Q && keycode <= KEY_P);
    
    if (is_letter && ((shift_pressed && !caps_lock) || (!shift_pressed && caps_lock))) {
        ascii_char = ascii_table_shift[keycode];
    } else if (shift_pressed) {
        ascii_char = ascii_table_shift[keycode];
    } else {
        ascii_char = ascii_table[keycode];
    }
    
    return ascii_char;
}

void keyboard_read_line(char* buffer, int max_length) {
    int i = 0;
    unsigned char keycode = 0;
    char c = 0;
    
    while(i < max_length - 1) {
        keycode = get_input_keycode();
        
        if(keycode == 0) {
            continue;
        }
        
        c = get_ascii_char(keycode);
        
        if(keycode == KEY_ENTER) {
            kprint("\n");
            buffer[i] = '\0';
            return;
        }
        
        if(keycode == KEY_BACKSPACE && i > 0) {
            i--;
            cursor_x--;
            if(cursor_x < 0) {
                cursor_x = 79;
                cursor_y--;
            }
            unsigned char* video_memory = (unsigned char*) 0xb8000;
            int offset = (cursor_y * 80 + cursor_x) * 2;
            video_memory[offset] = ' ';
            video_memory[offset+1] = 0x07;
            update_cursor();
            continue;
        }
        
        if(c == 0) {
            continue;
        }
        
        buffer[i++] = c;
        char str[2];
        str[0] = c;
        str[1] = '\0';
        kprint(str);
    }
    
    buffer[i] = '\0';
} 