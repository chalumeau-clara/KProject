//
// Created by chalu on 2/22/2023.
//

#include "../include/k/isr.h"

u32 interrupt_handler[] = {
        (u32) isr_0,
        (u32) isr_1,
        (u32) isr_2,
        (u32) isr_3,
        (u32) isr_4,
        (u32) isr_5,
        (u32) isr_6,
        (u32) isr_7,
        (u32) isr_8,
        (u32) isr_9,
        (u32) isr_10,
        (u32) isr_11,
        (u32) isr_12,
        (u32) isr_13,
        (u32) isr_14,
        (u32) isr_15,
        (u32) isr_16,
        (u32) isr_17,
        (u32) isr_18,
        (u32) isr_19,
        (u32) isr_20,
        (u32) isr_21,
        (u32) isr_22,
        (u32) isr_23,
        (u32) isr_24,
        (u32) isr_25,
        (u32) isr_26,
        (u32) isr_27,
        (u32) isr_28,
        (u32) isr_29,
        (u32) isr_30,
        (u32) isr_31,
        (u32) isr_128,
};


void init_isr()
{
    for (size_t i = 0; i < ISR_BASE_NUMBER; i++) {
        // SEGMENT SELECTOR: A Segment Selector with multiple fields which must point to a valid code segment in your GDT.
        // FLAGS: P:1 DPL:00 0 D:1 110
        set_gate(interrupt_handler[i], KERNEL_CODE_SEGMENT, 0x8E, i);
    }
    set_gate((u32)isr_128, KERNEL_CODE_SEGMENT, 0x8E, ISR_SYSCALL);
//set_gate((u32)isr_32, 0x08, 0x8E, 32);

}

void generic_c_handler(volatile struct interrupt_register *int_reg)
{
    printf("An ISR was called\r\n");
    printf("interrupt_number : %x\r\n", int_reg->interrupt_number);
    printf("error_code : %x\r\n", int_reg->error_code);
    printf("EIP : %x\r\n", int_reg->eip);
    printf("CS : %x\r\n", int_reg->cs);
    printf("Flags : %x\r\n", int_reg->eflags);
    printf("ESP : %x\r\n", int_reg->esp);
    printf("SS : %x\r\n", int_reg->ss);
}


void test_isr()
{
    //printf("test3\n\t");
    //isr_0();
    printf("test\r\n");
    // printf("test1 %i", 1/0);
    asm volatile ("int3"); // Breakpoint
    //asm volatile ("int $0x80"); // Syscall
    printf("test2\r\n");

}

