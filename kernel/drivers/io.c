#include "../include/io.h"

void outb(unsigned short port, unsigned char data) {
    asm volatile ("outb %0, %1" : : "a"(data), "Nd"(port));
}

unsigned char inb(unsigned short port) {
    unsigned char result;
    asm volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void outw(unsigned short port, unsigned short data) {
    asm volatile ("outw %0, %1" : : "a"(data), "Nd"(port));
} 