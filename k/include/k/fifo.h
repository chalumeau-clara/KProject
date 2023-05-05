//
// Created by chalu on 5/4/2023.
//

#ifndef K_FIFO_H
#define K_FIFO_H

#include <stddef.h>
#include <stdio.h>
#include "types.h"
#include "../memory.h"


typedef struct list1
{
    u8 scan_code;
    struct list1 *next;
} list1;

typedef struct fifo
{
    struct list1 *head;
    struct list1 *tail;
    size_t size;
} fifo;

fifo *fifo_init(void);
size_t fifo_size(fifo *fifo);
void fifo_push(fifo *fifo, int elt);
u8 fifo_head(fifo *fifo);
u8 fifo_pop(fifo *fifo);
void fifo_clear(fifo *fifo);
void fifo_destroy(fifo *fifo);
void fifo_print(const fifo *fifo);

#endif //K_FIFO_H
