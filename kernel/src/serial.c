#include <stdint.h>
#include "serial.h"
#include "io.h"

#define COM1 0x3F8              //first serial port, an I/O port number

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
