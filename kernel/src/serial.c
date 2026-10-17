#include <stdint.h>
#include "serial.h"

#define COM1 0x3F8              //first serial port, an I/O port number

// out/in talk to I/O ports, a separate address space from memory
static inline void outb(uint16_t port, uint8_t val) {
    // "a" = value in al, "Nd" = port as constant or in dx
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    //"=a" = output comes back in al
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void serial_init(void) {
    outb(COM1 + 1, 0x00);       //interrupts off, we poll
    outb(COM1 + 3, 0x80);       // DLAB on: set baud rate
    outb(COM1 + 0, 0x03);       // divisor 3 = 38400 baud (low byte)
    outb(COM1 + 1, 0x00);       // divisor high byte
    outb(COM1 + 3, 0x03);       // DLAB off, 8 bits, no parity, 1 stop
    outb(COM1 + 2, 0xC7);       // enable and clear FIFOs
    outb(COM1 + 4, 0x0B);       // modem control lines on
}

void serial_putc(char c) {
    while ((inb(COM1 + 5) & 0x20) == 0) {}   // wait until send buffer empty
    outb(COM1, (uint8_t)c);
}

void serial_puts(const char *s) {
    while (*s) {
        if (*s == '\n') serial_putc('\r');   // terminals want \r\n
        serial_putc(*s++);
    }
}

char serial_getc(void) {
    while ((inb(COM1 + 5) & 0x01) == 0) {}   // wait until a byte arrives
    return (char)inb(COM1);
}
