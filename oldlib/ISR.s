    .syntax unified
    .thumb

    .section .text

isr_NMI:
    bx lr

isr_HardFault:
    bkpt #0
    b .

isr_MemManage:
    bx lr

isr_BusFault:
    bx lr
