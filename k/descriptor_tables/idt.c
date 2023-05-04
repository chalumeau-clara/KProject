//
// Created by chalu on 2/22/2023.
//

#include "../include/k/idt.h"
#include "../include/k/isr.h"
#include "../include/k/pic.h"


static idt_g idt[NB_IDT_DESCRIPTOR];

void pretty_print_set_gate()
{
    for (int i = 0; i < NB_IDT_DESCRIPTOR; i++)
    {
        idt_g *idtG = &idt[i];
        printf("IDT nb : %u \n\t:", i);
        printf("offset low %x\n\t", idtG->offset_low);
        printf("segment_selector %x\n\t", idtG->segment_selector);
        printf("flags %x\n\t", idtG->flags);
        printf("offset_high %x\n\t", idtG->offset_high);
    }

}

void set_gate(u32 offset, u16 segment_selector, u8 flags, u8 nb)
{
    idt_g *idtG = &idt[nb];

    // set offset 0 -- 15
    idtG->offset_low = (u16)(offset & 0x0000FFFF);

    // Set segment selector 15 -- 31
    idtG->segment_selector = segment_selector;

    // Set reverved 0 -- 7
    idtG->reserved_and_set_zero = 0;

    // Set flags 8 -- 15
    idtG->flags = flags;

    // Set offset 16 -- 31
    idtG->offset_high = (u16)((offset & 0xFFFF0000) >> 16);

    //pretty_print_set_gate(idtG);
}

void init_idt()
{
    static idt_r idtR;

    idtR.base_address = (u32)idt; // idt base address
    idtR.limit = sizeof(idt) - 1; // idt size - 1

//    printf("itdR %x\n", &idtR);
//    printf("itdR base add %x\n", idtR.base_address);
//    printf("itdR limit %x\r\n", idtR.limit);


    // Init isr ?
    init_isr();
    printf("ISR init\r\n");

    init_pic();
    printf("PIC Init\r\n");
//     pretty_print_set_gate();

    // Load IDT
    __asm__ volatile("lidt %0\n"
            : /* no output */
            : "m" (idtR)
            : "memory");

    //while (1);


}

