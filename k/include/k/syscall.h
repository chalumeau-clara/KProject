//
// Created by chalu on 5/9/2023.
//

#ifndef K_SYSCALL_H
#define K_SYSCALL_H
#include "isr.h"
#include "pic.h"
#include "libvga.h"
#include "iso9660.h"

void handler_syscall(volatile struct interrupt_register *int_reg);
#endif //K_SYSCALL_H
