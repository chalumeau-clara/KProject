//
// Created by chalu on 2/22/2023.
//

#include "../include/k/idt.h"
#include "../include/k/isr.h"

void set_gate(u32 offset, u16 segment_selector, u8 flags, idt_g *idtG)
{
    // set offset 0 -- 15
    idtG->offset_low = offset & 0xFFFF;

    // Set segment selector 15 -- 31
    idtG->segment_selector = segment_selector;

    // Set reverved 0 -- 7
    idtG->reserved_and_set_zero = 0;

    // Set flags 8 -- 15
    idtG->flags = flags;

    // Set offset 16 -- 31
    idtG->offset_high = (offset >> 16) & 0xFFFF0000;

    return;
}

void init_idt()
{
    idt_r idtR;

    idtR.base_address = (u32)&idt; // idt base address
    idtR.limit = sizeof(idt) - 1; // idt size - 1

    // Initialize to zero all the idt fields
    memset(&idt, 0, sizeof(idt_g) * 256);

    // Init isr ?
    //init_isr();
    // Load IDT
    __asm__ volatile("lidt %0\n"
            : /* no output */
            : "m" (idtR)
            : "memory");
}

