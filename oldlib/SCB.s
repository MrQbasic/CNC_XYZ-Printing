    .syntax unified
    .thumb

    .equ SCB_REG_CPUID,      0xE000ED00
    .equ SCB_REG_VTOR,       0xE000ED08

    .section .text


//CPUID
SCB_CPUID_read:
    ldr r1, =SCB_REG_CPUID
    ldr r0, [r1]
    bx lr

SCB_VTOR_read:
    ldr r1, =SCB_REG_VTOR
    ldr r0, [r1]
    bx lr
