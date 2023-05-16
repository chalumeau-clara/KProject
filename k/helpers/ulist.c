//
// Created by chalu on 5/10/2023.
//

#include "../include/k/ulist.h"


struct ulist *ulist_add(struct ulist *l, u16 reg, u16 drive)
{
    struct ulist *temp = memory_reserve(sizeof(struct ulist));
    if (temp == NULL)
        return l;
    temp->reg = reg;
    temp->drive = drive;
    if (l == NULL)
        temp->next = NULL;
    else
        temp->next = l;
    return temp;
}