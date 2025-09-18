#include "IRQ.h"
#include "LCD.h"


#define SCB_VTOR    0xE000ED08          //Vector Table Offset Register


#define SCB_CFSR    0xE000ED28  //Configurable Fault Status Register
#define SCB_HFSR    0xE000ED2C  //HardFault Status Register
#define SCB_MMFAR   0xE000ED34  //Memory Management Fault Address Register
#define SCB_BFAR    0xE000ED38  //Bus Fault Address Register


//NVIC REGISTERS
#define NVIC_REG_SET_ENABLE         0xE000E100 //8 Registers
#define NVIC_REG_CLEAR_ENABLE       0xE000E180 //8 Registers
#define NVIC_REG_SET_PENDING        0xE000E200 //8 Registers
#define NVIC_REG_CLEAR_PERNDING     0xE000E280 //8 Registers
#define NVIC_REG_ACTIVE             0xE000E300 //8 Registers
#define NVIC_REG_PRIORITY           0xE000E400 //12 Registers
#define NVIC_REG_SOFTWARE_TRIGGER   0xE000EF00 //1 Registers


volatile unsigned int vectorTable[256] __attribute__((aligned(0x80)));

extern void* _start;

void __attribute__((interrupt)) defaultISR(){
    LCD_clearScreen();
    LCD_setCursor(0,0);
    LCD_writeString("Unhandled INT!");
    LCD_setCursor(0,1);
    LCD_writeIntHex(*(volatile int*) NVIC_REG_ACTIVE);
    LCD_setCursor(0,2);
    LCD_writeIntHex(*(volatile int*) (NVIC_REG_ACTIVE+4));
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
}

void IRQ_setFunction(int exceptionNumber, void* func){
    vectorTable[exceptionNumber] = (int) func;
}




void IRQ_enable(int exceptionNumber){
    if(exceptionNumber < IRQ_ID_INT_BASE) return;
    exceptionNumber -= IRQ_ID_INT_BASE;
    if(exceptionNumber < 32) {
        *((volatile int*) NVIC_REG_SET_ENABLE) = (1 << exceptionNumber);
        return;
    }else if(exceptionNumber <= 46){
        *((volatile int*) (NVIC_REG_SET_ENABLE+4)) = (1 << (exceptionNumber-32));

        while(1);
        return;
    }
    LCD_clearScreen();
    LCD_writeString("IRQ Enable Err");
    LCD_setCursor(0,1);
    LCD_writeString("Num: ");
    LCD_writeIntHex(exceptionNumber);
}

void IRQ_disable(int exceptionNumber){
    if(exceptionNumber < IRQ_ID_INT_BASE) return;
    exceptionNumber -= IRQ_ID_INT_BASE;
    if(exceptionNumber < 32) {
        *((volatile int*) NVIC_REG_CLEAR_ENABLE) = (1 << exceptionNumber);
    }else if(exceptionNumber <= 46){
        exceptionNumber -= 32;
        *((volatile int*) NVIC_REG_CLEAR_ENABLE+4) = (1 << exceptionNumber);
    }
    LCD_clearScreen();
    LCD_writeString("IRQ Enable Err");
    LCD_setCursor(0,1);
    LCD_writeString("Num: ");
    LCD_writeIntHex(exceptionNumber);
}

void IRQ_trigger(int exceptionNumber){
    if(exceptionNumber < IRQ_ID_INT_BASE) return;
    exceptionNumber -= IRQ_ID_INT_BASE;
    *((volatile int*) NVIC_REG_SOFTWARE_TRIGGER) = exceptionNumber;
}

void IRQ_resetPending(int IRQ_ID){
    if(IRQ_ID < 32){
        *((int*) NVIC_REG_CLEAR_PERNDING) = IRQ_ID;
        //while((*((int*) NVIC_REG_SET_PENDING)) & IRQ_ID);
    }else{
        IRQ_ID -= 32;
        *((int*) NVIC_REG_CLEAR_PERNDING+4) = IRQ_ID;
        //while((*((int*) NVIC_REG_SET_PENDING+1)) & IRQ_ID);

    }
}