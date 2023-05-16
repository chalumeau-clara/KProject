//
// Created by chalu on 5/10/2023.
//

#ifndef K_ULIST_H
#define K_ULIST_H


#include <stddef.h>
#include "types.h"
#include "../memory.h"

typedef struct ulist
{
    u16 reg;
    u16 drive;
    struct ulist *next;
} ulist;

struct ulist *ulist_add(struct ulist *l, u16 reg, u16 drive);



#endif //K_ULIST_H
