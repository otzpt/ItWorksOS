#include <stddef.h>
#include "console.h"
#include "keyboard.h"
#include "shell.h"

#define LINE_MAX 128

// reas on line with echo and backspace
static size_t read_line(char *buf, size_t max)
{
    size_t len = 0;
    for (;;) {
        char c = keyboard_getc();
        if (c == '\n') {
            console_putc('\n');
            buf[len] = 0;
            return len;
        }
        if (c == '\b') {
            if (len > 0) {
                len--;
                console_putc('\b');
            }
            continue;
        }
        if (len < max - 1) { // keep room for 0
            buf[len++] = c;
            console_putc(c);
        }
    }
}

static int streq(const char *a, const char *b)
{
    while (*a && *a == *b) {
        a++;
        b++;
    }
    return *a == *b;
}

static int starts_with(const char *s, const char *prefix)
{
    while (*prefix) {
        if (*s++ != *prefix++) return 0;
    }
    return 1;
}

void shell_run(void)
{
    char line[LINE_MAX];
    for (;;) {
        console_puts("> ");
        read_line(line, LINE_MAX);
        if (streq(line, "help")) {
            console_puts("commands: help clear echo\n");
        } else if (streq(line, "clear")) {
            console_clear();
        } else if (starts_with(line, "echo ")) {
            console_puts(line + 5);
            console_putc('\n');
        } else if (line[0] != 0) {
            console_puts("unknown command: ");
            console_puts(line);
            console_putc('\n');
        }
    }
}
