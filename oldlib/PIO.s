    .syntax unified
    .thumb

    .equ PIO_BASE,      0x400E0E00
    .equ PIO_GROUP_OFFSET,  0x0200

    .equ PIO_REG_E,     0x00
    .equ PIO_REG_D,     0x04
    .equ PIO_REG_OE,    0x10
    .equ PIO_REG_OD,    0x14
    .equ PIO_SOD,       0x30
    .equ PIO_COD,       0x34
    
    .equ PC10_MASK,  (1 << 10)

    .section .text



//set input or output of register
//R0 - pins to set
//R1 - PIO Group (A-E -> 0-4)
//R2 - pin Type (0->Output, 1->Input, 2->Float(Gives control pack to peripheral))
PIO_set_Mode: 
    //check if a valid group is selected
    cmp r1, 4
    bhi .exit
    //calc the offset
    ldr r3, =PIO_GROUP_OFFSET
    mul r1, r1, r3
    ldr r3, =PIO_BASE
    add r1, r1, r3
    //check what do 
    cmp r2, #0
    beq .output
    cmp r2, #1
    beq .input
    cmp r2, #2
    beq .float
    b .exit
    
.Loutput:
    //take control of the pins
    ldr r2, =PIO_REG_OE
    add r2, r2, r3
    str r0, [r2]
    b .enable
.Linput:

.Lenable:
    //enable the pin
    ldr r2, =PIO_REG_E
    add r2, r2, r3
    str r0, [r2]
    b .exit
.Lfloat:
    ldr r2, =PIO_REG_D
    add r2, r2, r3
    str r0, [r2]
.Lexit:
    bx lr



//R0 - what appears on the output bus
//R1 - PIO Group (A-E -> 0-4)
PIO_write:
    //check if a valid group is selected
    cmp r1, 4
    bhi .exit
    //calc the offset
    ldr r3, =PIO_GROUP_OFFSET
    mul r1, r1, r3
    ldr r3, =PIO_BASE
    add r1, r1, r3
    //check what do 
    cmp r2, #0
    beq .output
    cmp r2, #1
    beq .input
    cmp r2, #2
    beq .float
    b .exit
.Lexit:
    bx lr




PIO_test:
 // Enable PIO control of PC10
    LDR  R0, =PIOC_PER
    MOV  R1, #PC10_MASK
    STR  R1, [R0]

    // Set PC10 as output
    LDR  R0, =PIOC_OER
    MOV  R1, #PC10_MASK
    STR  R1, [R0]

loop:
    // Turn ON (set PC10 high)
    LDR  R0, =PIOC_SODR
    MOV  R1, #PC10_MASK
    STR  R1, [R0]

    // Dumb delay loop
    MOV  R2, #0x20000
delay_on:
    SUBS R2, R2, #1
    BNE  delay_on

    // Turn OFF (clear PC10)
    LDR  R0, =PIOC_CODR
    MOV  R1, #PC10_MASK
    STR  R1, [R0]

    // Dumb delay loop
    MOV  R2, #0x20000
delay_off:
    SUBS R2, R2, #1
    BNE  delay_off

    B loop
