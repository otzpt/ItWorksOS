#ifndef CONSOLE_H
#define CONSOLE_H

#include <limine.h>

void console_init(struct limine_framebuffer *fb);
void console_clear(void);
void console_putc(char c);
void console_puts(const char *s);

#endif
