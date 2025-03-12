#ifndef SCREEN_H
#define SCREEN_H

extern int cursor_x;
extern int cursor_y;

void clear_screen();
void kprint(const char* str);
void scroll_screen();
void update_cursor();

#endif 