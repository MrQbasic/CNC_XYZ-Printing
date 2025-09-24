#include "PIO.h"
#include "LCD.h"
#include "WDT.h"
#include "PMC.h"
#include "UDP.h"
#include "IRQ.h"
#include "MATRIX.h"
#include <stdint.h>

extern unsigned int _sidata, _sdata, _edata;
extern unsigned int _sbss, _ebss;

void init_runtime(void) {
    unsigned int *src = &_sidata;
    unsigned int *dst = &_sdata;
    while (dst < &_edata) *dst++ = *src++;

    dst = &_sbss;
    while (dst < &_ebss) *dst++ = 0;
}


int cnt=0;
void __attribute__((interrupt)) PIO_CALL(void){
    int x = *((int*) 0x400E0E4C);  //read ISR from PIO to reset int trigger

    cnt++;
    IRQ_setState(IRQ_ID_INT_BASE + PID_PIO_GROUP_A, 3);
}


void main(){
    
    WDT_disable();
    init_runtime();
    
    IRQ_init();

    LCD_init();
    
    PMC_enableExternalClock();


    //enable all PIO blocks
    PMC_enablePeripheralClock(PID_PIO_GROUP_A);
    PMC_enablePeripheralClock(PID_PIO_GROUP_B);
    PMC_enablePeripheralClock(PID_PIO_GROUP_C);
    PMC_enablePeripheralClock(PID_PIO_GROUP_D);
    PMC_enablePeripheralClock(PID_PIO_GROUP_E);
    
    //PIO_setMode(1 << 17, PIO_GROUP_A, PIO_MODE_INPUT_INT);
    //IRQ_setFunction(IRQ_ID_INT_BASE + PID_PIO_GROUP_A, &PIO_CALL );
    //IRQ_enable(IRQ_ID_INT_BASE + PID_PIO_GROUP_A);
    //IRQ_trigger(IRQ_ID_INT_BASE + PID_PIO_GROUP_A);

    UDP_init();

    while (1);
    


    //PIO_setMode(1 << 2, PIO_GROUP_A, PIO_MODE_OUTPUT);
    //PIO_output_set(1 << 2, PIO_GROUP_A);

    return;
}