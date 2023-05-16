//
// Created by chalu on 2/22/2023.
//
// ***************************
// Interrupt Service Routines
// ***************************

#ifndef K_ISR_H
#define K_ISR_H
#include "idt.h"
#include "keyboard.h"
#include "syscall.h"
#define ISR_SYSCALL 0x80
#define ISR_KEYBOARD 33
#define ISR_TIMER 32
#define ISR_BASE_NUMBER 31


// 0 Divide by 0
//1 Debug
//2 NMI
//3 Breakpoint
//4 Overflow
//5 Bound Range
//Exceeded
//6 Invalid Opcode
// 7 Device Not Available
//8 Double Fault
//9 Coprocessor
//10 Invalid TSS
//11 Segment not present
//12 Stack Segment Fault
//13 General Protection
//14 Page Fault
// 15 (Intel reserved)
//16 x87 FPU (Math Fault)
//17 Alignment Check
//18 Machine Check
//19 SMD Floating Point Ex.
//20 Virtualization Ex.
//21-31 (Intel reserved)
//32-255 User Defined

extern void isr_0();
extern void isr_1();
extern void isr_2();
extern void isr_3();
extern void isr_4();
extern void isr_5();
extern void isr_6();
extern void isr_7();
extern void isr_8();
extern void isr_9();
extern void isr_10();
extern void isr_11();
extern void isr_12();
extern void isr_13();
extern void isr_14();
extern void isr_15();
extern void isr_16();
extern void isr_17();
extern void isr_18();
extern void isr_19();
extern void isr_20();
extern void isr_21();
extern void isr_22();
extern void isr_23();
extern void isr_24();
extern void isr_25();
extern void isr_26();
extern void isr_27();
extern void isr_28();
extern void isr_29();
extern void isr_30();
extern void isr_31();
//extern void isr_32();
extern void isr_128();




struct interrupt_register {
    u32 gs, fs, es, ds; // save registers
    u32 di, si, bp, sp, bx, dx, cx, ax; // pusha
    u32 interrupt_number, error_code;
    u32 eip, cs, eflags, esp, ss;
}__attribute__((packed));
typedef struct interrupt_register int_reg;

void init_isr();
void generic_c_handler(volatile struct interrupt_register *int_reg);
void test_isr();

#endif //K_ISR_H
