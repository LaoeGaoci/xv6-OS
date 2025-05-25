#include "console.h"

//Task-1 print函数
void print_str(const char *s) {
    volatile char *video = (volatile char *)0xB8000;
    int pos = 0;

    while (*s) {
        char c = *s++;
        video[pos++] = c;
        video[pos++] = 0x07;  // 白字黑底
    }
}