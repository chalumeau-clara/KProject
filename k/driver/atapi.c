//
// Created by chalu on 5/6/2023.
//

#include "../include/k/atapi.h"
/*
 * Attente, decourverte, communication
 */

static u8 block[8192];
static list_atapi_drive listAtapiDrive = {
        .ulist = NULL,
};

void busy_wait(u16 drive) {
    while(inb(ATA_REG_STATUS(drive)) & BSY); // attend qu'il change de status
}

void wait_device_selection(u16 drive) {
    // attend 420 nanoseconde
    for (unsigned i = 0; i < 14; i++)
        inb(ATA_REG_STATUS(drive)); // 14 fois = 420 nanosecond
}

void wait_packet_request(u16 drive) {
    while(((inb(ATA_REG_STATUS(drive)) & DRQ) == 0) && (inb(ATA_REG_STATUS(drive)) & BSY));
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

    // PRIMARY REG
    /* Select current drive */
    select_drive(PRIMARY_REG, ATA_PORT_MASTER);

    /* Delay of response for drive selection*/
    wait_device_selection(PRIMARY_REG);

    /* Look for ATAPI signature */
    if (is_atapi_drive(PRIMARY_REG) == 1)
        ulist_add(listAtapiDrive.ulist, PRIMARY_REG, ATA_PORT_MASTER);


    /* Select current drive */
    select_drive(PRIMARY_REG, ATA_PORT_SLAVE);

    /* Delay of response for drive selection*/
    wait_device_selection(PRIMARY_REG);

    /* Look for ATAPI signature */
    if (is_atapi_drive(PRIMARY_REG) == 1)
        ulist_add(listAtapiDrive.ulist, PRIMARY_REG, ATA_PORT_SLAVE);


    // SECONDARY REG
    /* Select current drive */
    select_drive(SECONDARY_REG, ATA_PORT_MASTER);

    /* Delay of response for drive selection*/
    wait_device_selection(SECONDARY_REG);

    /* Look for ATAPI signature */
    /* Look for ATAPI signature */
    if (is_atapi_drive(SECONDARY_REG) == 1)
        ulist_add(listAtapiDrive.ulist, SECONDARY_REG, ATA_PORT_MASTER);


    /* Select current drive */
    select_drive(SECONDARY_REG, ATA_PORT_SLAVE);

    /* Delay of response for drive selection*/
    wait_device_selection(SECONDARY_REG);

    /* Look for ATAPI signature */
    if (is_atapi_drive(SECONDARY_REG) == 1)
        ulist_add(listAtapiDrive.ulist, SECONDARY_REG, ATA_PORT_SLAVE);

}

int send_packet(struct SCSI_packet *pkt, u16 drive,
                u16 size) {
    //To send a packet to the drive:
    busy_wait(drive);

    outb(ATA_REG_FEATURES(drive), 0); // No overlap/no DM
    outb(ATA_REG_SECTOR_NB(drive), 0); // No queuing
    outb(ATA_REG_LBA_MI(drive), (u8) CD_BLOCK_SZ);
    outb(ATA_REG_LBA_HI(drive), CD_BLOCK_SZ >> 8);
    outb(ATA_REG_COMMAND(drive), PACKET); /* PACKET */
    // To wait for a packet to be requested:
    // Poll the status register until BSY is cleared and DRQ is set
    wait_packet_request(drive);

    // Send SCSI packet with ‘READ 12’ commmand
    // Write the SCSI packet to the Data Register
    // One word at a time (you must use outw())
    outw(ATA_REG_DATA(PRIMARY_REG), (u16) pkt);

    // Read Sector Count Reg while it doesn’t return
    //    DATA TRANSMIT (0x2)
    while (inb(ATA_REG_SECTOR_COUNT(PRIMARY_REG)) != PACKET_DATA_TRANSMIT);

    return 1;
}

// block de taille fixe
/*
 * poll de donnée statique read block a ecrire le block dessus
 */
void *read_block(size_t lba) {

    discover_atapi_drive(); // ??

    struct SCSI_packet pkt = {
            .op_code = READ_12,
            .flags_lo = 0,
            .lba_hi = (lba >> 0x18) & 0xFF,
            .lba_mihi = (lba >> 0x10) & 0xFF,
            .lba_milo = (lba >> 0x08) & 0xFF,
            .lba_lo = (lba >> 0x00) & 0xFF,
            .transfer_length_hi = 0,
            .transfer_length_mihi = 0,
            .transfer_length_milo = 0,
            .transfer_length_lo = 0,
            .flags_hi = 0,
            .control = 0,
    };

    send_packet(&pkt, listAtapiDrive.ulist->drive, sizeof (pkt));

    //Once the SCSI packet has been sent:
    //Read CDROM BLK SIZE word by word:
    u16 data = inw(ATA_REG_DATA(listAtapiDrive.ulist->reg));

    //Read Sector Count Register while it does not return
      //      PACKET COMMAND COMPLETE (0x3)
    while (inb(ATA_REG_SECTOR_COUNT(listAtapiDrive.ulist->drive) != PACKET_COMMAND_COMPLETE));

    return (void *)3;

}
