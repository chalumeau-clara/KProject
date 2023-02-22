//
// Created by chalu on 2/19/2023.
//

#ifndef K_GDT_H
#define K_GDT_H

#include "k/types.h"

struct gdt_entry
{
    u16 limit;
    u16 base_address;

    u8 base_address_low; // Base 0 -- 7
    u8 segment_type; // Access
    // Type 8 -- 11
    // S 11 -- 12 Descriptor type (0 = system; 1 = code or data)
    // DPL 13 -- 14 Descriptor privilege level 0-3
    // P 15 -- 16 Segment present

    u8 limit_segment; // Seg.Limit 16 -- 19 Segment Limit (always 1111)
    // AVL 19 -- 20 Available for use by system software always 0
    // L 20 -- 21 64-bit code segment (IA-32e mode only)
    // D/B 21 -- 22 Default operation size (0 = 16-bit segment; 1 = 32-bit segment)
    // G 23 -- 24 Granularity 0 = 1 byte, 1 = kbyte
    u8 base_address_high; // Base 24 -- 31
} __attribute__((packed));
typedef struct gdt_entry gdt_e;

struct gdt_register
{
    u16 limit;
    u32 base_address;
} __attribute__((packed));
typedef struct gdt_register gdt_r;

void init_gdt();

#endif //K_GDT_H
