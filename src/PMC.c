#include "PMC.h"
#include "LCD.h"

#define PMC_REG_SC_EN       0x400E0400
#define PMC_REG_SC_DI       0x400E0404
#define PMC_REG_SC_S        0x400E0408
#define PMC_REG_PCE_0       0x400E0410
#define PMC_REG_PCD_0       0x400E0414
#define PMC_REG_PCS_0       0x400E0418
#define PMC_REG_CKG_MOR     0x400E0420      //Main Oscillator Register
#define PMC_REG_PCE_1       0x400E0500
#define PMC_REG_PCD_1       0x400E0504
#define PMC_REG_PCS_1       0x400E0508
#define PMC_REG_USB         0x400E0438
#define PMC_REG_WPM         0x400E04E4      //Write Protect Mode Register
#define PMC_REG_STATUS      0x400E0468
#define PMC_REG_CKG_MFR     0x400E0424      //Main Clock Frequency Register
#define PMC_REG_CKG_PLLAR   0x400E0428      //PLL Setup
#define PMC_REG_MCK         0x400E0430      //Master Clock Register (for CPU etc.)
void PMC_enablePeripheralClock(int PID){
    if(PID >= 7 && PID <= 31){
        *((int*) PMC_REG_PCE_0) = (1 << PID);
        while((~*(volatile int*) PMC_REG_PCS_0) & (1 << PID));
        return;
    }
    if(PID <= 47){
        PID -= 32;
        *((int*) PMC_REG_PCE_1) = (1 << PID);
        while((~*(volatile int*) PMC_REG_PCS_1) & (1 << PID));
        return;
    }
    //do some error handling
}

//CPU IS STILL RUNNING SLOW
void PMC_enableExternalClock(){
    //enable the external clock
    *(volatile int*) PMC_REG_WPM = 0x504D4300;      //disable write protection if set for some reason
    int reg = *(volatile int*) PMC_REG_CKG_MOR;     //read current value
    reg |= (0x37 << 16);                            //add key
    reg &= ~(0x00000002);                           //disable the main bypass
    *(volatile int*) PMC_REG_CKG_MOR = reg;
    reg |= 0X00000001;                              //enable the external clock
    reg |= (255 << 8);                              //set startup time (max)
    *(volatile int*) PMC_REG_CKG_MOR = reg;
    while(~(*(volatile int*) PMC_REG_STATUS) & 1);  //wait for ext clock to stabilize
    reg |= (1 << 24);                               //enable the main clock
    *(volatile int*) PMC_REG_CKG_MOR = reg;
    //check the frequency
    *(volatile int*) PMC_REG_CKG_MFR = (1 << 20);           //reset the counter
    while (~(*(volatile int*) PMC_REG_CKG_MFR) & (1 << 16));//wait for measurement to complete
    int freq = (32768 * ((*(volatile int*) PMC_REG_CKG_MFR) & 0xFFFF)) / 16 / 1000;
    //setup PLL
    *(volatile int*) PMC_REG_CKG_PLLAR = (1 << 29) | (19 << 16) | (0b111111 << 8) | 1;    //div by 1, mul by 20, 40cycles till lock, >> 240MHz
    while (~(*(volatile int*) PMC_REG_STATUS) & (0x02));    //wait for PLL is ready
    //switch MCK to PLL
    *(volatile int*) PMC_REG_MCK |= (1<<12);                 //switch to PLL
    while (~(*(volatile int*) PMC_REG_STATUS) & (0x08));    //wait for MCK is ready
    *(volatile int*) PMC_REG_MCK = (1<<12) | 2;               //switch to PLL
    while (~(*(volatile int*) PMC_REG_STATUS) & (0x08));    //wait for MCK is ready
}




void PMC_setUSBClock(int state){
    *(volatile int*) PMC_REG_USB = (4<<8);      //set prescaler to div by (4+1)=5 to get 48Mhz

    int check = 0;
    if(state == 1){
        *((int*) PMC_REG_SC_EN) = (1 << 7);
        check = 1;
    }else{
        *((int*) PMC_REG_SC_DI) = (1 << 7);
    }
    int timeout = 0;
    int* status = (int*)(PMC_REG_SC_S);
    while((((*status) >> 7) & 1) != check){
        timeout++;
        if(timeout > 1000){
            LCD_writeString("FUCK!");
            break;
        }
    }
}