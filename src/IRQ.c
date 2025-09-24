#include "IRQ.h"
#include "LCD.h"


#define SCB_VTOR    0xE000ED08                                    //Vector Table Offset Register
#define SCB_CFSR    0xE000ED28                                    //Configurable Fault Status Register
#define SCB_HFSR    0xE000ED2C                                    //Hard Fault Status Register

//NVIC REGISTERS as arrays for direct access
#define NVIC_REG_ENABLE_SET         ((volatile int*)0xE000E100)   //8 Registers
#define NVIC_REG_ENABLE_CLEAR       ((volatile int*)0xE000E180)   //8 Registers
#define NVIC_REG_PENDING_SET        ((volatile int*)0xE000E200)   //8 Registers
#define NVIC_REG_PENDING_CLEAR      ((volatile int*)0xE000E280)   //8 Registers
#define NVIC_REG_ACTIVE             ((volatile int*)0xE000E300)   //8 Registers
#define NVIC_REG_PRIORITY           ((volatile int*)0xE000E400)   //13 Registers
#define NVIC_REG_SOFTWARE_TRIGGER   ((volatile int*)0xE000EF00)   //1 Register


volatile unsigned int vectorTable[256] __attribute__((aligned(0x80)));

extern void* _start;

void __attribute__((interrupt)) defaultISR(){
    LCD_clearScreen();
    LCD_clearScreen();
    LCD_writeString("Unhandled INT!\n");



    while(1);
}

void __attribute__((interrupt)) ISR_Reset(){
    LCD_writeString("Reset");
    while(1);
}
void __attribute__((interrupt)) ISR_NMI(){
    LCD_writeString("NMI");
    while(1);
}
void __attribute__((interrupt)) ISR_HardFault(){
    LCD_clearScreen();
    LCD_writeString("HardFault-");
    switch (*(volatile int*) SCB_HFSR)
    {
    case 0x40000000:
        LCD_writeString("FORCED");
        LCD_writeIntHex(*(volatile int*) SCB_CFSR);
        __asm__("BKPT #0");
        break;
    case 0x80000000:
        LCD_writeString("DEBUG");
        break;
    case 0x2:
        LCD_writeString("VTBL");
        break;
    }
    while (1);
    
}
void __attribute__((interrupt)) ISR_MemManage(){
    LCD_writeString("MemManage");
    while(1);
}
void __attribute__((interrupt)) ISR_BusFault(){
    LCD_writeString("BusFault");
    while(1);
}
void __attribute__((interrupt)) ISR_UsageFault(){
    LCD_writeString("UsageFault");
    while(1);
}
void __attribute__((interrupt)) ISR_SVCall(){
    LCD_writeString("SVCall");
    while(1);
}
void __attribute__((interrupt)) ISR_PendSV(){
    LCD_writeString("PendSV");
    while(1);
}
void __attribute__((interrupt)) ISR_SysTick(){
    LCD_writeString("SysTick");
    while(1);
}

void IRQ_init(){
    //setup exception handlers
    vectorTable[0] = 0;
    vectorTable[1] = (int) &_start;
    vectorTable[2] = (int) &ISR_NMI;
    vectorTable[3] = (int) &ISR_HardFault;
    vectorTable[4] = (int) &ISR_MemManage;
    vectorTable[5] = (int) &ISR_BusFault;
    vectorTable[6] = (int) &ISR_UsageFault;
    // Reserved: 7-10
    vectorTable[11] = (int) &ISR_SVCall;
    // Reserved: 12-13
    vectorTable[14] = (int) &ISR_PendSV;
    vectorTable[15] = (int) &ISR_SysTick;
    //setup default handlers
    for(int i=16; i<256; i++){
        vectorTable[i] = (int) &defaultISR;
    }
    //switch table address
    *((int*) SCB_VTOR) = (int) vectorTable;

    //clear all pending 
    for(int i=IRQ_ID_INT_BASE; i<IRQ_ID_INT_BASE+47;i++){
        IRQ_setState(i, 3); 
    }
}

void IRQ_setFunction(int exceptionNumber, void* func){
    vectorTable[exceptionNumber] = (int) func;
}


//state: 0=disable, 1=enable, 2=pending, 3=clear pending
void IRQ_setState(int exceptionNumber, int state){
    //check if the interrupt is part of the NVIC
    if(exceptionNumber < IRQ_ID_INT_BASE) return;
    exceptionNumber -= IRQ_ID_INT_BASE;

    int reg = exceptionNumber / 32;
    int offset = exceptionNumber % 32;

    switch(state){
        //disable
        case 0:
            NVIC_REG_ENABLE_CLEAR[reg] = (1 << offset);
            if(NVIC_REG_ENABLE_SET[reg] & (1 << offset)){
                LCD_clearScreen("Failed to disable IRQ");
                while(1);
            }
            break;
        //enable
        case 1:
            NVIC_REG_ENABLE_SET[reg] = (1 << offset);
            if(!(NVIC_REG_ENABLE_SET[reg] & (1 << offset))){
                LCD_clearScreen("Failed to enable IRQ");
                while(1);
            }
            break;
        //set pending
        case 2:
            NVIC_REG_PENDING_SET[reg] = (1 << offset);
            if(!(NVIC_REG_PENDING_SET[reg] & (1 << offset))){
                LCD_clearScreen("Failed to set IRQ pending");
                while(1);
            }
            break;
        //clear pending
        case 3:
            NVIC_REG_PENDING_CLEAR[reg] = (1 << offset);
            if(NVIC_REG_PENDING_SET[reg] & (1 << offset)){          
                LCD_clearScreen("Failed to clear IRQ pending");
                while(1);
            }
            break;
        //unexpected error
        default:
            LCD_clearScreen("Unexpected stateError int IRQ_setState");
            while(1);
    }
}

void IRQ_trigger(int exceptionNumber){
    if(exceptionNumber < IRQ_ID_INT_BASE) return;
    exceptionNumber -= IRQ_ID_INT_BASE;
    *((int*) NVIC_REG_SOFTWARE_TRIGGER) = exceptionNumber;
}