//
// Created by chalu on 4/18/2023.
//

#include "../include/k/keyboard.h"

void handler_keyboard() {

    // If Output buffer full is set by the keyboard controller
    if (inb(KEYBOARD_STATUS_REGISTER) & 0x01) {
        // Then read scan code
        u8 scan_code = inb(KEYBOARD_IO_BUFFER);
        printf("scan code %d", scan_code);
    }
};