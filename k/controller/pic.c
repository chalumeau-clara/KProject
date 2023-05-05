//
// Created by chalu on 4/10/2023.
//

#include "../include/k/pic.h"


u32 pic_handler[] = {
        (u32) irq_0,
        (u32) irq_1,
        (u32) irq_2,
        (u32) irq_3,
        (u32) irq_4,
        (u32) irq_5,
        (u32) irq_6,
        (u32) irq_7,
        (u32) irq_8,
        (u32) irq_9,
        (u32) irq_10,
        (u32) irq_11,
        (u32) irq_12,
        (u32) irq_13,
        (u32) irq_14,
        (u32) irq_15
};


void init_pic(){
    // ICW1 (Initialization Command Words)
    // Controlling the PIC’s mode of operation
    // 10001 ICW4 present, cascade mode, edge triggered mode
    outb(MASTER_PIC_A, 0x11);    // Send $0x11 to the master PIC
    outb(SLAVE_PIC_A, 0x11);     // Send $0x11 to the slave PIC

    // ICW2
    // informs the PIC about the base offset in the interrupt descriptor table (IDT) to use when transmitting an IRQ
    // from a device to the CPU.
    outb(MASTER_PIC_B, 0x20); // Set the master PIC's base offset to 32 | IDT entry number 32
    outb(SLAVE_PIC_B, 0x28); // Set the slave PIC's base offset to 40 | IDT entry number 40

    // ICW3
    // how master and slave PICs are connected
    // Master pic 00000100
    // Slave pic 00000010
    outb(MASTER_PIC_B, 0x04);
    outb(SLAVE_PIC_B, 0x02);

    // ICW4
    // additional operation modes
    // 0000001 Special mode normal, Buffering mode, normal (clear) EOI
    outb(MASTER_PIC_B, 0x01);
    outb(SLAVE_PIC_B, 0x01);

    for (size_t i = 0; i < IRQ_NUMBER; i++) {
        // mask interrupt
        OCW1(i, 0);
        // set idt
        set_gate(pic_handler[i], KERNEL_CODE_SEGMENT, 0x8E, i + 32);
        // printf ( "handler %u %x", i, pic_handler[i]);

    }
    // Enable Timer
    OCW1(IRQ_TIMER, 1);
    printf("Timer Init");

    // Enable keyboad
    OCW1(IRQ_KEYBOARD, 1);
    init_queue();
    printf("Keyboard Init");

    // enable hardware
    asm volatile("sti");
}

void OCW1(u8 irq, size_t mask) {
    // OCW1 (Operation Control Words)
    // used to mask/unmask IRQs on a PIC

    u8 port;
    u8 value;

    if (irq >= 8) {
        port = SLAVE_PIC_B;
        irq -= 8;
    } else {
        port = MASTER_PIC_B;
    }

    value = mask == 0 ? inb(port) | (1 << irq) : inb(port) & ~(1 << irq);
    outb(port, value);

}

void OCW2(u8 irq) {
    // OCW2
    // It allows the kernel, among other things, to acknowledge an IRQ
    if (irq >= 8)
        outb(SLAVE_PIC_A, PIC_EOI); // Send ACK to slave PIC
    outb(MASTER_PIC_A, PIC_EOI); // Send ACK to master PIC
}
