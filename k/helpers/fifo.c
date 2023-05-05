//
// Created by chalu on 5/4/2023.
//

#include "../include/k/fifo.h"

struct fifo *fifo_init(void)
{
    fifo *fifo = memory_reserve(sizeof(*fifo));
    if (fifo == NULL)
        return NULL;
    fifo->head = fifo->tail = NULL;
    fifo->size = 0;
    return fifo;
}

size_t fifo_size(fifo *fifo)
{
    return fifo->size;
}

void fifo_push(fifo *fifo, int elt)
{
    list1 *temp = NULL;
    temp = memory_reserve(sizeof(list1));
    temp->scan_code = elt;
    temp->next = NULL;
    fifo->size++;
    if (fifo->head == NULL)
        fifo->tail = fifo->head = temp;
    else
    {
        fifo->tail->next = temp;
        fifo->tail = fifo->tail->next;
    }
}

u8 fifo_head(fifo *fifo)
{
    return fifo->head->scan_code;
}

u8 fifo_pop(fifo *fifo)
{
    u8 scan_code = 0;
    if (fifo->head != NULL)
    {
        scan_code = fifo->head->scan_code;
        list1 *temp = fifo->head;
        fifo->head = fifo->head->next;
        if (fifo->head == NULL)
            fifo->tail = NULL;
        fifo->size--;
        memory_release(temp);
    }
    return scan_code;
}

void fifo_clear(fifo *fifo)
{
    while (fifo->size != 0)
        fifo_pop(fifo);
}

void fifo_destroy(fifo *fifo)
{
    fifo_clear(fifo);
    memory_release(fifo);
}

void print(list1 *list1)
{
    if (list1 == NULL)
        return;
    else
    {
        printf("%d\n", list1->scan_code);
        print(list1->next);
    }
}

void fifo_print(const fifo *fifo)
{
    print(fifo->head);
}