    .syntax unified
    .thumb

    .section .vectors, "a", %progbits
    .global _vector_table
_vector_table:
    .word _estack               //Stack start
    .word _start  + 1            //Reset       enable thumb mode with +1 ?
    .word isr_NMI
    .word isr_HardFault
    .word isr_MemManage
    .word isr_BusFault

    /* External Interrupts (fill with 0 or handlers) */
    .rept (0x400 / 4 - 6)
        .word 0
    .endr


    .section .text
    .extern main
    .global _start
_start:

    bl main

hang:
    b hang


isr_NMI:
    bx lr

isr_HardFault:
    LDR R0, =0xE000ED2C   @ HFSR: HardFault Status Register
    LDR R1, [R0]
    BKPT #0               @ Break here to inspect R1 in debugger
    B .

isr_MemManage:
    bx lr

isr_BusFault:
    bx lr
