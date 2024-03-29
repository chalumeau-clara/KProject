//
// Created by chalu on 2/19/2023.
//

#include "../include/k/gdt.h"
#include <stdio.h>
#define GDT_SIZE 6
static gdt_e gdt[GDT_SIZE];



void gdt_pretty_print()
{
    for (size_t i = 0; i < GDT_SIZE; i++)
    {
        // Print in hexadecimal
        printf("%x\r\n", gdt[i].limit);
        printf("%x\r\n", gdt[i].base_address);
        printf("%x\r\n", gdt[i].base_address_low);
        printf("%x\r\n", gdt[i].segment_type);
        printf("%x\r\n", gdt[i].limit_segment);
        printf("%x\r\n", gdt[i].base_address_high);
        printf("\r\n");
    }
}

void set_segment_descriptor(u16 base, u32 limit, u8 type, u8 flag, gdt_e *gdtE) {

    // set limit bits 0:15
    gdtE->limit = limit & 0xFFFF;

    // set base bits 16:32
    gdtE->base_address = base & 0xFFFF;

    // set base bits 0:7
    gdtE->base_address_low = (base >> 16) & 0xFF;

    // set bits segment type 8:15
    gdtE->segment_type = type;

    // set bits limit 16:24
    gdtE->limit_segment = (limit >> 16) & 0x0F;
    gdtE->limit_segment |= flag & 0xF0;

    // set bits base 24:31
    gdtE->base_address_high = (base >> 24) & 0xFF;
}

void init_gdt()
{


    // Null segment
    // Offset 0
    set_segment_descriptor(0,0,0,0, &gdt[0]);

    // Kernel Code segment
    // type: RX priv :0
    // TYPE : present:1 DPL: 00 S:1 Type:1010 Read/Exec
    // FLAG : G: 1 D/B: 1 L: 0 AVL: 0 Limit:1111
    set_segment_descriptor(0, 0xFFFFFFFF, 0x9A, 0xCF, &gdt[1]);

    // Kernel Data segment
    // Type: RW priv: 0
    // TYPE : present:1 DPL: 00 S:1 Type:0010 Read/Write
    // FLAG : G: 1 D/B: 1 L: 0 AVL: 0 Limit:1111
    set_segment_descriptor(0, 0xFFFFFFFF, 0x92, 0xCF, &gdt[2]);

    // Userland Code segment
    // type: RX priv: 3
    // TYPE : present:1 DPL: 11 S:1 Type:1010 Read/Exec
    // FLAG : G: 1 D/B: 1 L: 0 AVL: 0 Limit:1111
    set_segment_descriptor(0, 0xFFFFFFFF, 0xFA, 0xCF, &gdt[3]);

    // Userland Data segment
    // Type: RW Priv: 3
    // TYPE : present:1 DPL: 11 S:1 Type:0010 Read/Write
    // FLAG : G: 1 D/B: 1 L: 0 AVL: 0 Limit:1111
    set_segment_descriptor(0, 0xFFFFFFFF, 0xF2, 0xCF, &gdt[4]);

    // Task State Segment
    // TODO
    set_segment_descriptor(0, 0, 0, 0, &gdt[5]);



    static gdt_r gdtR;
    gdtR.base_address = (u32)gdt;
    gdtR.limit = sizeof (gdt) - 1;
    // gdt_pretty_print();

    // Load GDT
    asm volatile("lgdt %0\n"
            : /* no output */
            : "m" (gdtR)
            : "memory");

    // Set the PE (Protection Enable) bit in the cr0 register
    asm volatile ("movl %cr0, %eax");
    asm volatile ("or $1, %al");
    asm volatile("movl %eax, %cr0");

    // Setting data into segment register
    asm volatile("movw $0x10, %ax");
    asm volatile("movw %ax, %ds");
    asm volatile("movw %ax, %es");
    asm volatile("movw %ax, %fs");
    asm volatile("movw %ax, %gs");
    asm volatile("movw %ax, %ss");

    // Switching to protected mode
    asm volatile("ljmp $0x08, $1f");
    asm volatile("1:");

}