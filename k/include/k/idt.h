//
// Created by chalu on 2/22/2023.
//
// ***************************
// Interrupt Descriptor Table
// ***************************
// The interrupt descriptor table (IDT) associates each exception or interrupt vector with a gate descriptor for the
// procedure or task used to service the associated exception or interrupt.


#ifndef K_IDT_H
#define K_IDT_H

#include <stdio.h>
#include <string.h>
#include "types.h"



struct idt_gate
{
    u16 offset_low; // Offset to procedure entry point 0 -- 15
    u16 segment_selector; // segment selector for destination code segment 16 -- 31

    u8 reserved_and_set_zero; // reserved 0 -- 4 and zero 5 -- 7
    u8 flags; // 0D111 8 -- 12 D = 1 : 32 bits 0 : 16 bits
    // DPL (Descriptor priviledge level) 13 -- 14 like gdt
    // P segment present flag 14 -- 15 like gdt
    u16 offset_high; // Offset to procedure entry point 16 -- 31
} __attribute__((packed));
typedef struct idt_gate idt_g;

struct idt_register
{
    u16 limit;
    u32 base_address;
} __attribute__((packed));
typedef struct idt_register idt_r;

#define NB_IDT_DESCRIPTOR 256
static idt_g idt[NB_IDT_DESCRIPTOR];


void init_idt();
void set_gate(u32 offset, u16 segment_selector, u8 flags, idt_g *idtG);

#endif //K_IDT_H
