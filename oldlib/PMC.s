    .syntax unified
    .thumb

    .equ PMC_REG_BASE, 0x400E0400

    .equ PMC_REG_PCE0, PMC_REG_BASE + 0x0010
    .equ PMC_REG_PCD0, PMC_REG_BASE + 0x0014
    .equ PMC_REG_PCS0, PMC_REG_BASE + 0x0018
    
    .equ PMC_REG_PCE1, PMC_REG_BASE + 0x0100
    .equ PMC_REG_PCD1, PMC_REG_BASE + 0x0104
    .equ PMC_REG_PCS1, PMC_REG_BASE + 0x0108

    .section .text





//R0         -> PID
//R1 - BIT 0 -> status
//Return:
//  R0 = 0x0 -> OK
//  R0 = 0xF -> invalid PID
PMC_Peripheral_Clock_Set:
    //check for a valid PID
    cmp r0, #7
    bl .error
    cmp r0, #47
    bhi .error
    //check which reg to write to
    cmp r0, #31
    bhi .reg1
    ldr r3, =PMC_REG_PCE0
    b .state
.reg1:
    sub r0, r0, #32
    ldr r3, =PMC_REG_PCE1
.state:
    //check if we need to enable or disable
    and r1, r1, 1
    lsl r1, r1, #2
    add r3, r3, r1
    //shift bit acording to pid
    mov r2, #1
    lsl r2, r2, r0

    str r2, [r3]



.error:
    mov r0, #0xf
.exit:
    bx lr
