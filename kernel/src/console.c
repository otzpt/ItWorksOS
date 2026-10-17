#include<stdint.h>
#include<stddef.h>
#include "console.h"
#include "font8x8.h"

#define SCALE 2                 // font pixel become SCALE X SCALE
#define CELL (8*SCALE)          // one char cell, in pixels
#define FG 0xFFFFFF             // white; works for any RGB channel order
#define BG 0x000000

static struct limine_framebuffer *fb;
static size_t col, row, cols, rows;

static void put_pixel(size_t x, size_t y, uint32_t color) {
    volatile uint32_t *p= fb->address;
    p[y * (fb->pitch /4)+ x] = color; //pitch is in bytes, p in pixels
}

// will change brackets style mid code cause i have free will
// and its easier to read and its consistent with other projects i have in c
static void draw_char(char c, size_t cx, size_t cy)
{
    const unsigned char *glyph = font8x8_basic[(unsigned char)c & 0x7F];
    for (size_t y = 0; y < CELL; y++) {
        for (size_t x = 0; x < CELL; x++) {
            // bit 0 is left most pixel
            uint32_t on = (glyph[y / SCALE] >> (x / SCALE)) & 1;
            put_pixel(cx * CELL + x, cy * CELL + y, on ? FG : BG);
        }
    }
}

static void scroll(void)
{
    volatile uint32_t *p = fb->address;
    size_t stride = fb->pitch / 4;  //pixels per line
    size_t shift = CELL * stride;  // pixels in one text row
    size_t total = fb->height * stride;
    for (size_t i = 0; i + shift < total; i++) p[i] = p[i + shift];
    for (size_t i = total - shift; i < total; i++) p[i] = BG;
}

void console_clear(void)
{
    volatile uint32_t *p = fb->address;
    size_t total = fb->height * (fb->pitch / 4);
    for (size_t i = 0; i < total; i++) p[i] = BG;
    col = 0;
    row = 0;
}

void console_init(struct limine_framebuffer *framebuffer) {
    fb = framebuffer;
    cols = fb->width / CELL;
    rows = fb->height / CELL;
    console_clear();
}

void console_putc(char c)
{
    if (c == '\n') {
        col = 0;
        row++;
    } else if (c == '\b') {
        if (col > 0) {
            col--;
            draw_char(' ', col, row); // erase previous char
        }
    } else if (c != '\r') {
        draw_char(c, col, row);
        col++;
    }
    if (col >= cols) { // wrap at the right edge
        col = 0;
        row++;
    }
    if (row >= rows) { // scroll at the bottom
        scroll();
        row = rows-1;
    }
}

void console_puts(const char *s)
{
    while (*s) console_putc(*s++);
}
