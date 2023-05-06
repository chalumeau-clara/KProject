//
// Created by chalu on 5/5/2023.
//

#include "../include/k/timer.h"

static u64 ticks = 0;

unsigned long gettick(void) {
    return ticks * 10;
}

void handler_timer() {
    ticks++;
}


void init_timer(void) {
    // Write into control register
    // 00110100 = 0x34
    // 0 unset binary counter
    // 010 Mode 2
    // 11 read/write LSB first, then MSB
    // 00 set counter to 0
    outb(COUNTER_REGISTER, 0X34);

    // Write the divider on the counter register
    u16 divider = INTERNAL_FREQ / INTERRUPT_RATE;
    outb(COUNTER_0, (u8)divider);
    outb(COUNTER_0, divider >> 8);
}

