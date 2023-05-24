//
// Created by chalu on 5/6/2023.
//

#include <stdio.h>
#include "../include/k/atapi.h"
/*
 * Attente, decourverte, communication
 */

static u8 block[BLOCK_SIZE];

// 0 : reg
// 1: drive
static u16 atapi[2];


void busy_wait(u16 drive) {
    while(inb(ATA_REG_STATUS(drive)) & BSY); // attend qu'il change de status
}

void wait_device_selection(u16 drive) {
    // attend 420 nanoseconde
    for (unsigned i = 0; i < 14; i++)
        inb(ATA_REG_STATUS(drive)); // 14 fois = 420 nanosecond
}

void wait_packet_request(u16 drive) {
    u8 status;
    do {
        status = inb(ATA_REG_STATUS(drive));
    } while ((status & DRQ) == 0 && (status & BSY));
    //while((inb(ATA_REG_STATUS(drive)) & BSY) && (inb(ATA_REG_STATUS(drive)) & DRQ) == 0);
}


void select_drive(u16 bus, u8 slave) {
    outb(ATA_REG_DRIVE(bus), slave);
}

int is_atapi_drive(u16 bus) {
    u8 sig[4];
    /* Look for ATAPI signature */
    sig[0] = inb(ATA_REG_SECTOR_COUNT(bus));
    sig[1] = inb(ATA_REG_LBA_LO(bus));
    sig[2] = inb(ATA_REG_LBA_MI(bus));
    sig[3] = inb(ATA_REG_LBA_HI(bus));

    if (sig[0] == ATAPI_SIG_SC && sig[1] == ATAPI_SIG_LBA_LO
        && sig[2] == ATAPI_SIG_LBA_MI && sig[3] == ATAPI_SIG_LBA_HI)
        return 1;
    return -1;
}



void discover_atapi_drive() {

    // Send ‘Software Reset’ to the controller’s DCR
    // To put it on a stable state
    outb(PRIMARY_DCR, SRST);
    outb(SECONDARY_DCR, SRST);
    // Send ‘Disable IRQ’ to the controller’s DCR
    // We do not want any IRQ triggering
    outb(PRIMARY_DCR, INTERRUPT_DISABLE);
    outb(SECONDARY_DCR, INTERRUPT_DISABLE);

    //Test all possibility until one is found

    // PRIMARY REG
    /* Select current drive */
    select_drive(PRIMARY_REG, ATA_PORT_MASTER);

    /* Delay of response for drive selection*/
    wait_device_selection(PRIMARY_REG);

    /* Look for ATAPI signature */
    if (is_atapi_drive(PRIMARY_REG) == 1) {
        atapi[0] = PRIMARY_REG;
        atapi[1] = ATA_PORT_MASTER;
        return;
    }


    /* Select current drive */
    select_drive(PRIMARY_REG, ATA_PORT_SLAVE);

    /* Delay of response for drive selection*/
    wait_device_selection(PRIMARY_REG);

    /* Look for ATAPI signature */
    if (is_atapi_drive(PRIMARY_REG) == 1) {
        atapi[0] = PRIMARY_REG;
        atapi[1] = ATA_PORT_SLAVE;
        return;
    }


    // SECONDARY REG
    /* Select current drive */
    select_drive(SECONDARY_REG, ATA_PORT_MASTER);

    /* Delay of response for drive selection*/
    wait_device_selection(SECONDARY_REG);

    /* Look for ATAPI signature */
    /* Look for ATAPI signature */
    if (is_atapi_drive(SECONDARY_REG) == 1)  {
        atapi[0] = SECONDARY_REG;
        atapi[1] = ATA_PORT_MASTER;
        return;
    }


    /* Select current drive */
    select_drive(SECONDARY_REG, ATA_PORT_SLAVE);

    /* Delay of response for drive selection*/
    wait_device_selection(SECONDARY_REG);

    /* Look for ATAPI signature */
    if (is_atapi_drive(SECONDARY_REG) == 1)  {
        atapi[0] = SECONDARY_REG;
        atapi[1] = ATA_PORT_SLAVE;
        return;
    }
}

void init_ATAPI_driver() {
    discover_atapi_drive();
    printf("REGISTER: %x\n", atapi[0]);
    printf("DRIVE : %x\n", atapi[1]);
}

int send_packet(struct SCSI_packet *pkt, u16 drive,
                u16 size) {
    printf("before busy wait\n");
    //To send a packet to the drive:
    busy_wait(drive);
    printf("after busy wait\n");

    outb(ATA_REG_FEATURES(drive), 0); // No overlap/no DM
    outb(ATA_REG_SECTOR_COUNT(drive), 0); // No queuing
    outb(ATA_REG_LBA_MI(drive), (u8) size);
    outb(ATA_REG_LBA_HI(drive), size >> 8);
    outb(ATA_REG_COMMAND(drive), PACKET); /* PACKET */
    // To wait for a packet to be requested:
    // Poll the status register until BSY is cleared and DRQ is set
    printf("before wait packet request\n");

    wait_packet_request(drive);
    printf("after wait packet request\n");

    // Send SCSI packet with ‘READ 12’ commmand
    // Write the SCSI packet to the Data Register
    // One word at a time (you must use outw())
    u16* pkt_data = (u16*)pkt;
    for (size_t i = 0; i < PACKET_SZ / 2; i++)
        outw(ATA_REG_DATA(drive), pkt_data[i]);
    printf("after outw\n");

    // Read Sector Count Reg while it doesn’t return
    //    DATA TRANSMIT (0x2)
    // while (inb(ATA_REG_SECTOR_COUNT(drive)) != PACKET_DATA_TRANSMIT);
    printf("after data transmit\n");

    return 1;
}

// block de taille fixe
/*
 * poll de donnée statique read block a ecrire le block dessus
 */
void *read_block(size_t lba) {

    struct SCSI_packet pkt = {
            .op_code = READ_12,
            .flags_lo = 0,
            .lba_hi = (lba >> 0x18) & 0xFF,
            .lba_mihi = (lba >> 0x10) & 0xFF,
            .lba_milo = (lba >> 0x08) & 0xFF,
            .lba_lo = (lba >> 0x00) & 0xFF,
            .transfer_length_hi = 0,//(CD_BLOCK_SZ >> 0x18) & 0xFF,
            .transfer_length_mihi = 0,//(CD_BLOCK_SZ >> 0x10) & 0xFF,
            .transfer_length_milo = 0,//(CD_BLOCK_SZ >> 0x08) & 0xFF,
            .transfer_length_lo = 1,//CD_BLOCK_SZ & 0xFF, // only one block 1
            .flags_hi = 0,
            .control = 0,
    };
    printf("before packer\n");
    send_packet(&pkt, atapi[1], CD_BLOCK_SZ);
    printf("after packer\n");

    //Once the SCSI packet has been sent:
    //Read CDROM BLK SIZE word by word:
    for (size_t i = 0; i < CD_BLOCK_SZ / 2; i++) {
        // Get u16 then 2 block
        u16 buf = inw(ATA_REG_DATA(atapi[0]));
        block[i*2] = (u8) buf;
        block[i*2+1] = (u8) (buf >> 8);
    }

    //Read Sector Count Register while it does not return
      //      PACKET COMMAND COMPLETE (0x3)
    while (inb(ATA_REG_SECTOR_COUNT(atapi[1]) != PACKET_COMMAND_COMPLETE));

    return block;

}
