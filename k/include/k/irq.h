//
// Created by chalu on 4/10/2023.
//

// ***************************
// Interrupt Request
// ***************************

// Master 8259
//IRQ0 – Intel 8253 or Intel 8254 Programmable Interval Timer, aka the system timer or PIT
//IRQ1 – Intel 8042 keyboard controller
//IRQ2 – not assigned in PC/XT; cascaded to slave 8259 INT line in PC/AT
//IRQ3 – 8250 UART serial port COM2 and COM4
//IRQ4 – 8250 UART serial port COM1 and COM3
//IRQ5 – hard disk controller in PC/XT; Intel 8255 parallel port LPT2 in PC/AT
//IRQ6 – Intel 82072A floppy disk controller
//IRQ7 – Intel 8255 parallel port LPT1 / spurious interrupt


//Slave 8259 (PC/AT and later only)
//IRQ8 – real-time clock (RTC)
//IRQ9 – no common assignment
//IRQ10 – no common assignment
//IRQ11 – no common assignment
//IRQ12 – Intel 8042 PS/2 mouse controller
//IRQ13 – math coprocessor
//IRQ14 – hard disk controller 1
//IRQ15 – hard disk controller 2

#ifndef K_IRQ_H
#define K_IRQ_H
#include "idt.h"
#define MASTER_PIC_A 0x20
#define MASTER_PIC_B 0x21
#define SLAVE_PIC_A		0xA0
#define SLAVE_PIC_B		0xA1
#define IRQ_NUMBER 16

extern void irq_0();
extern void irq_1();
extern void irq_2();
extern void irq_3();
extern void irq_4();
extern void irq_5();
extern void irq_6();
extern void irq_7();
extern void irq_8();
extern void irq_9();
extern void irq_10();
extern void irq_11();
extern void irq_12();
extern void irq_13();
extern void irq_14();
extern void irq_15();

void init_pic(void);
void OCW1(u8 irq, size_t mask);
void OCW2(void);
void irq_handler(void);

#endif //K_IRQ_H
