    .syntax unified
    .thumb

    .equ EEFC0_BASE, 0x400E0A00
    .equ EEFC_FCR,   EEFC0_BASE + 0x04
    .equ EEFC_FSR,   EEFC0_BASE + 0x08
    .equ EEFC_FRR,   EEFC0_BASE + 0x0C

    .equ KEY,        0x5A
    .equ CMD_SET_GPNVM, 0x0D
    .equ CMD_GET_GPNVM, 0x0F
    .equ GPNVM_BIT, 1

    .global main
main:
    ldr r1, =EEFC_FCR

    /* Set GPNVM bit 1 */
    mov r2, #(KEY << 24)
    mov r3, #(CMD_SET_GPNVM << 8)
    mov r4, #GPNVM_BIT
    orr r2, r2, r3
    orr r2, r2, r4
    str r2, [r1]

wait_ready_set:
    ldr r0, =EEFC_FSR
    ldr r5, [r0]
    tst r5, #1
    beq wait_ready_set

    /* Issue Get GPNVM command */
    mov r2, #(KEY << 24)
    mov r3, #(CMD_GET_GPNVM << 8)
    mov r4, #0
    orr r2, r2, r3
    orr r2, r2, r4
    str r2, [r1]

wait_ready_get:
    ldr r0, =EEFC_FSR
    ldr r5, [r0]
    tst r5, #1
    beq wait_ready_get

    /* Read result into r0 */
    ldr r0, =EEFC_FRR
    ldr r0, [r0]

hang:
    b hang
