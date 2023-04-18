//
// Created by chalu on 4/10/2023.
//

#ifndef K_IRQ_H
#define K_IRQ_H
#include "idt.h"
#define MASTER_PIC_A 0x20
#define MASTER_PIC_B 0x21
#define SLAVE_PIC_A		0xA0
#define SLAVE_PIC_B		0xA1

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
void mask_irq(u8 irq);
void irq_handler(void);

#endif //K_IRQ_H
