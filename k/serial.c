#include "include/k/types.h" // stddef for size_t
#include "io.h" // for inb and outb
#include "include/k/serial.h"

#define COM1_BASE_ADDRESS 0x3f8  // COM1 base address
#define LCR_OFFSET 0x03
#define LATCH_LOW 0x00
#define LATCH_HIGH 0x01
#define MCR_OFFSET 4 // Modem control register
#define FIFO_CR_OFFSET 2 // FIFO control register


// RECEIVING DATA

int serial_received() {
    return inb(COM1_BASE_ADDRESS + 5) & 1;
}

char read_serial() {
    while (serial_received() == 0);

    return inb(COM1_BASE_ADDRESS);
}

// SENDING DATA
int is_transmit_empty() {
    return inb(COM1_BASE_ADDRESS + 5) & 0x20;
}

void write_serial(char a) {
    while (is_transmit_empty() == 0);

    outb(COM1_BASE_ADDRESS,a);
}

int init_serial() {
    // 	Enable Transmitter Holding Register Empty Interrupt
    outb(COM1_BASE_ADDRESS + LATCH_HIGH, 0x02);

    // Enable DLAB (set baud rate divisor)
    outb(COM1_BASE_ADDRESS + LCR_OFFSET, 0x80);
    outb(COM1_BASE_ADDRESS + LATCH_LOW, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    outb(COM1_BASE_ADDRESS + LATCH_HIGH, 0x00);    //                  (hi byte)

    //  (8 bits length) | (No parity) one stop bit
    // 00000011
    outb(COM1_BASE_ADDRESS + LCR_OFFSET, 0x03);

    // (FIFO Enable) | (Interrupt trigger level 14 bytes) |
    //(Clear transmit FIFO) | (Clear receive FIFO)
    // 11000111
    outb(COM1_BASE_ADDRESS + FIFO_CR_OFFSET, 0xC7);

    // IRQs enabled, RTS(Request to send)/DSR (Data terminal ready) set
    outb(COM1_BASE_ADDRESS + MCR_OFFSET, 0x0B);
    return 0;
}


int write(const char* buf, size_t count)
{
    for (size_t i = 0; i < count; i++)
        write_serial(buf[i]);
    return count;
}