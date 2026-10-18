#include <stdint.h>
#include "io.h"
#include "keyboard.h"

#define KB_DATA 0x60
#define KB_STATUS 0x64
#define MAP_SIZE 58

// scancode set 1, key press code, index = scancode, 0 = no chars
static const char map[MAP_SIZE] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' '
};

static const char map_shift[MAP_SIZE] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' '
};

static int shift_down;

char keyboard_getc(void)
{
    for (;;) {
        while ((inb(KB_STATUS) & 0x01) == 0) {}
        uint8_t sc = inb(KB_DATA);

        if (sc == 0x2A || sc == 0x36) { shift_down = 1; continue; }
        if (sc == 0xAA || sc == 0xB6) { shift_down = 0; continue; }
        if (sc & 0x80) continue;

        if (sc < MAP_SIZE) {
            char c = shift_down ? map_shift[sc] : map[sc];
            if (c) return c;
        }
    }
}
