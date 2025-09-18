#include "tools.h"
#include "LCD.h"

#define checkCntMax 1024


void reg_write_check(volatile int* reg, int val){
    *reg = val;
    int cnt=0;
    while(*reg != val){
        cnt++;
        if(cnt > checkCntMax){
            LCD_clearScreen();
            LCD_writeString("Reg IO Error:");
            LCD_setCursor(0,1);
            LCD_writeString("ADDR: ");
            LCD_writeIntHex((int)reg);
            LCD_setCursor(0,2);
            LCD_writeString("WANT: ");
            LCD_writeIntHex(val);
            LCD_setCursor(0,3);
            LCD_writeString("IS:   ");
            LCD_writeIntHex(*reg);
            while(1);
        }
    }
}




void reg_write_check_mask(volatile int* reg, int val, int mask){
    *reg = val;
    int cnt=0;
    while((*reg & mask) != (val & mask)){
        cnt++;
        if(cnt > checkCntMax){
            LCD_clearScreen();
            LCD_writeString("Register IO Error:");
            LCD_setCursor(0,1);
            LCD_writeString("ADDR: ");
            LCD_writeIntHex((int)reg);
            LCD_setCursor(0,2);
            LCD_writeString("WANT: ");
            LCD_writeIntHex(val);
            LCD_setCursor(0,3);
            LCD_writeString("IS:   ");
            LCD_writeIntHex(*reg);
            while(1);
        }
    }
}