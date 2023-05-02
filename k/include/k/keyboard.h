//
// Created by chalu on 4/18/2023.
//

#ifndef K_KEYBOARD_H
#define K_KEYBOARD_H
#include "kstd.h"
#include "types.h"
#include "../io.h"
#include "stdio.h"

#define KEYBOARD_IO_BUFFER 0x60
#define KEYBOARD_STATUS_REGISTER 0x64

void handler_keyboard();

#endif //K_KEYBOARD_H
