//
// Created by chalu on 4/18/2023.
//

#ifndef K_KEYBOARD_H
#define K_KEYBOARD_H
#include "kstd.h"
#include "types.h"
#include "../io.h"
#include "stdio.h"
#include "fifo.h"

#define IRQ_KEYBOARD 1
#define KEYBOARD_IO_BUFFER 0x60
#define KEYBOARD_STATUS_REGISTER 0x64

typedef struct queue_keyboard {
    fifo *fifo;
} queue_keyboard;

void handler_keyboard();
void init_queue(void);

int getkey(void);


#endif //K_KEYBOARD_H
