//
// Created by chalu on 5/9/2023.
//

#include "../include/k/syscall.h"

void handler_syscall(volatile struct interrupt_register *int_reg) {
    u8 syscall_number = int_reg->ax;
    u8 syscall_param1 = int_reg->bx;
    u8 syscall_param2 = int_reg->cx;
    u8 syscall_param3 = int_reg->dx;

    switch (syscall_number) {
        case SYSCALL_WRITE :
            break;
        case SYSCALL_SBRK :
            break;
        case SYSCALL_GETKEY:
            getkey();
            break;
        case SYSCALL_GETTICK :
            gettick();
            break;
        case SYSCALL_OPEN :
            open((const char *) syscall_param1, syscall_param2);
            break;
        case SYSCALL_READ :
            read(syscall_param1, (void *) syscall_param2, syscall_param3);
            break;
        case SYSCALL_SEEK :
            seek(syscall_param1, syscall_param2, syscall_param3);
            break;
        case SYSCALL_CLOSE :
            close(syscall_param1);
            break;
        case SYSCALL_SETVIDEO :
            setvideo(syscall_param1);
            break;
        case SYSCALL_SWAP_FRONTBUFFER:
            swap_frontbuffer((const void *) syscall_param1);
            break;
        case SYSCALL_PLAYSOUND :
            break;
        case SYSCALL_SETPALETTE :

            break;
        default :
            printf("Syscall number not know %u\n", syscall_number);
            break;
    }
}