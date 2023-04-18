//
// Created by chalu on 4/10/2023.
//

#include "../include/k/irq.h"
#include "../io.h" // for inb and outb

void init_pic(void){
    // ICW1
    // 10001 ICW4 present, cascade mode, edge triggered mode
    outb(MASTER_PIC_A, 0x11);    // Send $0x11 to the master PIC
    outb(SLAVE_PIC_A, 0x11);     // Send $0x11 to the slave PIC

    // ICW2
    outb(MASTER_PIC_B, 0x40); // Set the master PIC's base offset to 64
    outb(SLAVE_PIC_B, 0x50); // Set the slave PIC's base offset to 80

    // ICW3
    outb(MASTER_PIC_B, 0x04);
    outb(SLAVE_PIC_B, 0x02);

    // ICW4
    // 0000001 Special mode normal, Buffering mode, normal (clear) EOI
    outb(MASTER_PIC_B, 0x01);
    outb(SLAVE_PIC_B, 0x01);

    // mask interrupt
    mask_irq(0);
    mask_irq(1);
    mask_irq(2);
    mask_irq(3);
    mask_irq(4);
    mask_irq(5);
    mask_irq(6);
    mask_irq(7);
    mask_irq(8);
    mask_irq(9);
    mask_irq(10);
    mask_irq(11);
    mask_irq(12);
    mask_irq(13);
    mask_irq(14);
    mask_irq(15);

    // set idt
    set_gate((u32)irq_0, 0x08, 0x8E, 32);
    set_gate((u32)irq_1, 0x08, 0x8E, 33);
    set_gate((u32)irq_2, 0x08, 0x8E, 34);
    set_gate((u32)irq_3, 0x08, 0x8E, 35);
    set_gate((u32)irq_4, 0x08, 0x8E, 36);
    set_gate((u32)irq_5, 0x08, 0x8E, 37);
    set_gate((u32)irq_6, 0x08, 0x8E, 38);
    set_gate((u32)irq_7, 0x08, 0x8E, 39);
    set_gate((u32)irq_8, 0x08, 0x8E, 40);
    set_gate((u32)irq_9, 0x08, 0x8E, 41);
    set_gate((u32)irq_10, 0x08, 0x8E, 42);
    set_gate((u32)irq_11, 0x08, 0x8E, 43);
    set_gate((u32)irq_12, 0x08, 0x8E, 44);
    set_gate((u32)irq_13, 0x08, 0x8E, 45);
    set_gate((u32)irq_14, 0x08, 0x8E, 46);
    set_gate((u32)irq_15, 0x08, 0x8E, 47);

    // enable hardware
    asm volatile("sti");
}

void mask_irq(u8 irq) {
    // OCW1
    /*For example, if we want to mask the IRQ 3 of the master PIC, we could do something like:

    mov $1, %bl
    shl $3, %bl
    inb $0x21, %al
    or %bl, %al
    outb %al, $0x21*/
    u8 port;

    if (irq >= 8) {
        port = SLAVE_PIC_B;
        irq -= 8;
    } else {
        port = MASTER_PIC_B;
    }

    u8 value = inb(port) | (1 << irq);
    outb(port, value);

}

void send_end_of_interrupt_EOI(void) {
    // OCW2
    outb(MASTER_PIC_A, 0x20); // Send ACK to master PIC
    outb(SLAVE_PIC_A, 0x20); // Send ACK to slave PIC
}

void irq_handler(void) {

}