    .syntax unified
    .thumb

    .equ EEFC0_BASE, 0x400E0A00
    .equ EEFC_FLASH_REG_RESULT,  EEFC0_BASE + 0x00
    .equ EEFC_FLASH_REG_CMD,     EEFC0_BASE + 0x04
    .equ EEFC_FLASH_REG_STATUS,  EEFC0_BASE + 0x08

    .equ EEFC_FLASH_CMD_DESCRIPTOR_GET,      0x00
    .equ EEFC_FLASH_CMD_WRITE_PAGE,          0x01
    .equ EEFC_FLASH_CMD_WRITE_PAGE_LOCK,     0x02
    .equ EEFC_FLASH_CMD_ERASE_WRITE,         0x03
    .equ EEFC_FLASH_CMD_ERASE_WRITE_LOCK,    0x04
    .equ EEFC_FLASH_CMD_ERASE_ALL,           0x05
    .equ EEFC_FLASH_CMD_ERASE_PAGES,         0x07
    .equ EEFC_FLASH_CMD_LOCK_SET,            0x09
    .equ EEFC_FLASH_CMD_LOCK_GET,            0x0A
    .equ EEFC_FLASH_CMD_GPNVM_SET,           0x0B
    .equ EEFC_FLASH_CMD_GPNVM_CLEAR,         0x0C
    .equ EEFC_FLASH_CMD_GPNVM_GET,           0x0D
    .equ EEFC_FLASH_CMD_UID_START_READ,      0x0E
    .equ EEFC_FLASH_CMD_UID_STOP_READ,       0x0F
    .equ EEFC_FLASH_CMD_GET_CAL,             0x10
    .equ EEFC_FLASH_CMD_ERASE_SECTOR,        0x11
    .equ EEFC_FLASH_CMD_USER_SIG_WRITE,      0x12
    .equ EEFC_FLASH_CMD_USER_SIG_ERASE,      0x13
    .equ EEFC_FLASH_CMD_USER_SIG_READ_START, 0x14
    .equ EEFC_FLASH_CMD_USER_SIG_READ_STOP,  0x15

    .equ EEFC_FLASH_CMD_KEY, 0x5A << 24


    .section .text
read_descriptor:
    //set CMD
    ldr r1, =EEFC_FLASH_REG_CMD
    ldr r2, =EEFC_FLASH_CMD_KEY
    ldr r3, =EEFC_FLASH_CMD_GPNVM_SET
    ldr r4, =(1 << 8)  //ARG: set bit Number 1
    
    orr r5, r2, r3
    orr r5, r5, r4

    str r5, [r1]

    b hang


fuckup:
    LDR r1, =0xFEFEFEFE
    MOV r2, r1
    MOV r3, r1
    MOV r4, r1
fuckup.hang:
    b fuckup.hang

