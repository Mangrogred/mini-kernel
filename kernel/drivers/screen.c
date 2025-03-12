#include "../include/screen.h"
#include "../include/io.h"

int cursor_x = 0;
int cursor_y = 0;

void clear_screen() {
    unsigned char* video_memory = (unsigned char*) 0xb8000;
    int i;
    
    for (i = 0; i < 80 * 25 * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i+1] = 0x07;
    }
    
    cursor_x = 0;
    cursor_y = 0;
    
    update_cursor();
}

void kprint(const char* str) {
    unsigned char* video_memory = (unsigned char*) 0xb8000;
    int i = 0;
    
    while (str[i] != 0) {
        if (str[i] == '\n') {
            cursor_x = 0;
            cursor_y++;
        } else {
            int offset = (cursor_y * 80 + cursor_x) * 2;
            video_memory[offset] = str[i];
            video_memory[offset+1] = 0x07;
            cursor_x++;
        }
        
        if (cursor_x >= 80) {
            cursor_x = 0;
            cursor_y++;
        }
        
        if (cursor_y >= 25) {
            scroll_screen();
            cursor_y = 24;
        }
        
        i++;
    }
    
    update_cursor();
}

void scroll_screen() {
    unsigned char* video_memory = (unsigned char*) 0xb8000;
    int i;
    
    for (i = 0; i < 24 * 80 * 2; i++) {
        video_memory[i] = video_memory[i + 80 * 2];
    }
    
    for (i = 24 * 80 * 2; i < 25 * 80 * 2; i += 2) {
        video_memory[i] = ' ';
        video_memory[i+1] = 0x07;
    }
}

void update_cursor() {
    unsigned short position = cursor_y * 80 + cursor_x;
    
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));
} 