//
// Created by chalu on 2/22/2023.
//

#include "../include/k/idt.h"
#include "../include/k/isr.h"

void set_gate(u32 offset, u16 segment_selector, u8 flags, idt_g *idtG)
{
    // set offset 0 -- 15
    idtG->offset_low = offset ;

    // Set segment selector 15 -- 31
    idtG->segment_selector = segment_selector;

    // Set reverved 0 -- 7
    idtG->reserved_and_set_zero = 0;

    // Set flags 8 -- 15
    idtG->flags = flags | 0x60;

    // Set offset 16 -- 31
    idtG->offset_high = (offset >> 16) ;
    //printf("IDT 0x%.16llX\n", idtG);
    return;
}

void init_idt()
{
    idt_r idtR;

    idtR.base_address = (u32)&idt; // idt base address
    idtR.limit = sizeof(idt) - 1; // idt size - 1

    // Initialize to zero all the idt fields
    memset(&idt, 0, sizeof(idt));

    // Init isr ?
    //init_isr();
    // Load IDT
    __asm__ volatile("lidt %0\n"
            : /* no output */
            : "m" (idtR)
            : "memory");
}

