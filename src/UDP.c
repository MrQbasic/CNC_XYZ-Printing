#include "UDP.h"
#include "PMC.h"
#include "IRQ.h"
#include "LCD.h"
#include "MATRIX.h"
#include "tools.h"

#define UDP_REG_BASE                    0x40084000

// UDP register map struct (packed for memory-mapped HW)
typedef struct __attribute__((packed)) {
    volatile int frame_num;                // 0x00
    volatile int global_state;             // 0x04
    volatile int function_addr;            // 0x08
    volatile int rsvd0;                    // 0x0C
    volatile int interrupt_enable;         // 0x10
    volatile int interrupt_disable;        // 0x14
    volatile int interrupt_mask;           // 0x18
    volatile int interrupt_status;         // 0x1C
    volatile int interrupt_clear;          // 0x20
    volatile int rsvd1;                    // 0x24
    volatile int endpoint_reset;           // 0x28
    volatile int rsvd2;                    // 0x2C
    volatile int endpoint_ctrl_status[8];  // 0x30 - 0x4C
    volatile int endpoint_fifo_data[8];    // 0x50 - 0x6C
    volatile int rsvd3;                    // 0x70
    volatile int transeiver_ctrl;          // 0x74
} UDP_REG;

#define UDP ((UDP_REG *)UDP_REG_BASE)

#define PID_UDP 35


//gets called on an interrupt
void __attribute__((interrupt)) UDP_ISR(void){
    LCD_writeString("USB-Int!");
    LCD_setCursor(0,1);
    LCD_writeIntHex(UDP->interrupt_status);
    IRQ_resetPending(PID_UDP);
    while(1);
}


void UDP_init(){
    LCD_writeString("Start");
    //enable USB pins (should be default but you never know)
    MATRIX_IO_USB(0);
    //setup the interrupt
    IRQ_setFunction(IRQ_ID_INT_BASE + PID_UDP, &UDP_ISR);
    IRQ_enable(IRQ_ID_INT_BASE + PID_UDP);
    //enable the clocks
    PMC_enablePeripheralClock(PID_UDP);
    PMC_setUSBClock(1);
    //setup pull-up
    //reg_write_check(&UDP->transeiver_ctrl, 0x200);
    UDP->interrupt_clear = 0xFFFFFFFF;
    UDP->interrupt_enable = 0xFFFFFFFF;



    IRQ_trigger(IRQ_ID_INT_BASE + PID_UDP);



    UDP->endpoint_reset = 0x000000FF;

    //init procedure
    while(1);
    while(1){
        LCD_setCursor(0,0);
        LCD_writeIntHex(UDP->frame_num);
        LCD_setCursor(0,1);
        LCD_writeIntHex(UDP->global_state);
        LCD_setCursor(0,2);
        LCD_writeIntHex(UDP->interrupt_mask);
        LCD_setCursor(0,3);
        LCD_writeIntHex(UDP->interrupt_status);
    }

    LCD_writeString("UDP Init");
}
