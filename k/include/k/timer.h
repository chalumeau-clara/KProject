//
// Created by chalu on 5/5/2023.
//

// ***************************
// Timer controller
// ***************************

/*
 * Mode 0: Interrupt on terminal count
Mode 1: hardware retriggerable one-shot
Mode 2: rate generator
Mode 3: square generator
Mode 4: Software Triggered Strobe
Mode 5: Hardware Triggered Strobe
 */

/*
 * Pic configuration
 * c c io io m m m b
---- ----- ------- |
| | | +-- (1)
| | +-------- (2)
| +--------------- (3)
+---------------------- (4)
1 Binary counter (unset) or BCD counter (set)
2 Mode to use
3 Registers read/write policy (LSB first then MSB)
4 Counter to setup
 */

#ifndef K_TIMER_H
#define K_TIMER_H
#include "../io.h"
#include "types.h"
#define IRQ_TIMER 0

/*
 *
 */
#define COUNTER_0 0x40  // fire an interrupt at a user-defined frequency.
#define COUNTER_1 0x41 // historically used in order to periodically refresh the RAM, but it not used anymore.
#define COUNTER_2 0x42 //  linked with the PC speaker, so you can use it in order to generate sound
#define COUNTER_REGISTER 0x43
#define INTERRUPT_RATE 100
#define INTERNAL_FREQ 1193182

/*
Counter 0
Mode 2
Interrupt rate : 100 Hz
*/
unsigned long gettick(void);
void handler_timer();
void init_timer(void);



#endif //K_TIMER_H
