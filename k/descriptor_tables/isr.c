//
// Created by chalu on 2/22/2023.
//


#include "../include/k/isr.h"

void init_isr()
{
    // SEGMENT SELECTOR: A Segment Selector with multiple fields which must point to a valid code segment in your GDT.
    // FLAGS: P:1 DPL:00 0 D:1 110
    set_gate((u32)isr_0, 0x08, 0x8E, &idt[0]);
    set_gate((u32)isr_1, 0x08, 0x8E, &idt[1]);
    set_gate((u32)isr_2, 0x08, 0x8E, &idt[2]);
    set_gate((u32)isr_3, 0x08, 0x8E, &idt[3]);
    set_gate((u32)isr_4, 0x08, 0x8E, &idt[4]);
    set_gate((u32)isr_5, 0x08, 0x8E, &idt[5]);
    set_gate((u32)isr_6, 0x08, 0x8E, &idt[6]);
    set_gate((u32)isr_7, 0x08, 0x8E, &idt[7]);
    set_gate((u32)isr_8, 0x08, 0x8E, &idt[8]);
    set_gate((u32)isr_9, 0x08, 0x8E, &idt[9]);
    set_gate((u32)isr_10, 0x08, 0x8E, &idt[10]);
    set_gate((u32)isr_11, 0x08, 0x8E, &idt[11]);
    set_gate((u32)isr_12, 0x08, 0x8E, &idt[12]);
    set_gate((u32)isr_13, 0x08, 0x8E, &idt[13]);
    set_gate((u32)isr_14, 0x08, 0x8E, &idt[14]);
    set_gate((u32)isr_15, 0x08, 0x8E, &idt[15]);
    set_gate((u32)isr_16, 0x08, 0x8E, &idt[16]);
    set_gate((u32)isr_17, 0x08, 0x8E, &idt[17]);
    set_gate((u32)isr_18, 0x08, 0x8E, &idt[18]);
    set_gate((u32)isr_19, 0x08, 0x8E, &idt[19]);
    set_gate((u32)isr_20, 0x08, 0x8E, &idt[20]);
    set_gate((u32)isr_21, 0x08, 0x8E, &idt[21]);
    set_gate((u32)isr_22, 0x08, 0x8E, &idt[22]);
    set_gate((u32)isr_23, 0x08, 0x8E, &idt[23]);
    set_gate((u32)isr_24, 0x08, 0x8E, &idt[24]);
    set_gate((u32)isr_25, 0x08, 0x8E, &idt[25]);
    set_gate((u32)isr_26, 0x08, 0x8E, &idt[26]);
    set_gate((u32)isr_27, 0x08, 0x8E, &idt[27]);
    set_gate((u32)isr_28, 0x08, 0x8E, &idt[28]);
    set_gate((u32)isr_29, 0x08, 0x8E, &idt[29]);
    set_gate((u32)isr_30, 0x08, 0x8E, &idt[30]);
    set_gate((u32)isr_31, 0x08, 0x8E, &idt[31]);
//set_gate((u32)isr_32, 0x08, 0x8E, &idt[32]);
}

/*void generic_c_handler(struct interrupt_register *int_reg)
{
    printf("An ISR was called\r\n");
    printf("EIP : %d\r\n", int_reg->eip);
    printf("CS : %d\r\n", int_reg->cs);
    printf("Flags : %d\r\n", int_reg->flags);
    printf("ESP : %d\r\n", int_reg->esp);
    printf("SS : %d\r\n", int_reg->ss);
}*/

void generic_c_handler(void)
{
    printf("An ISR was called\r\n");

}

void test_isr()
{
    printf("test");
    printf("test1 %i", 1/0);
    //asm volatile ("int $0"); // Breakpoint
    //asm volatile ("int $0x80"); // Syscall
    printf("test2");

}