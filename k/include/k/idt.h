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

#define NB_IDT_DESCRIPTOR 256
#define KERNEL_CODE_SEGMENT 0x08

struct idt_gate
{
    u16 offset_low; // Offset to procedure entry point 0 -- 15
    u16 segment_selector; // segment selector for destination code segment 16 -- 31 KERNEL_CODE_SEGMENT_OFFSET = 0x08

    u8 reserved_and_set_zero; // reserved 0 -- 4 and zero 5 -- 7
    u8 flags; // 0D111 8 -- 12 with D = 1 : 32 bits 0 : 16 bits
    // DPL (Descriptor priviledge level) 13 -- 14 like gdt here set to 0
    // P segment present flag 14 -- 15 like gdt here 1
    // 10001110 = 0x8E | 11101110 = 0xEE (accessible by the user)
    u16 offset_high; // Offset to procedure entry point 16 -- 31
} __attribute__((packed));
typedef struct idt_gate idt_g;

struct idt_register
{
    u16 limit;
    u32 base_address;
} __attribute__((packed));
typedef struct idt_register idt_r;


void init_idt();
void set_gate(u32 offset, u16 segment_selector, u8 flags, u8 nb );

#endif //K_IDT_H
