#include "LCD.h"
#include "PIO.h"


#define delayPulse 50 * 10 * 3 * 2
#define delayChar 40 * 10 * 3 * 2

//data -> 8bit data
//type -> 0 = command; 1 = data
void LCD_writeData(int data, int type, int delay){
    //set data
    PIO_output_clear(0xFF, PIO_GROUP_C);
    PIO_output_clear(1 << 18, PIO_GROUP_C);
    PIO_output_set((data & 0xFF), PIO_GROUP_C);
    PIO_output_set((type & 0x1) << 18, PIO_GROUP_C);
    for(int i=0; i<delayPulse; i++){ __asm volatile ("nop");}
    //pulse Enable
    PIO_output_set(1 << 22, PIO_GROUP_A);
    for(int i=0; i<delayPulse; i++){ __asm volatile ("nop");}
    PIO_output_clear(1 << 22, PIO_GROUP_A); 
    for(int i=0; i<delay; i++){ __asm volatile ("nop");}
}

void LCD_init(){
    //mark bits as output
    PIO_setMode(0b1000000010011111111, PIO_GROUP_C, PIO_MODE_OUTPUT);
    PIO_setMode(1 << 22, PIO_GROUP_A, PIO_MODE_OUTPUT);
    //init sequence
    LCD_writeData(0x30, 0, 1800);
    LCD_writeData(0x30, 0, 1800);
    LCD_writeData(0x30, 0, 1800);
    LCD_writeData(0x38, 0, 1800);
    LCD_writeData(0x08, 0, 1800);
    LCD_writeData(0x01, 0, 1800);  //Clear screen
    LCD_writeData(0x06, 0, 1800);   
    LCD_writeData(0x0C, 0, 1800);
    LCD_writeData(0x80, 0, 1800);  // Set cursor to line 1, pos 0

    //enable display LED after init
    PIO_output_set(1<<10, PIO_GROUP_C); 
}


int curY = 0;
int curX = 0;


void LCD_writeChar(char c){
    curX++;
    if(curX == 17){
        curX = 1;
        curY++;
        if(curY == 4){
            curY = 0;
            LCD_clearScreen();
        }
        LCD_setCursor(curX, curY);
    }
    LCD_writeData((int)c, 1, delayChar);
}

void LCD_clearScreen() {
    LCD_writeData(0x01, 0, 800);
    LCD_writeData(0x02, 0, 800);
}


void LCD_writeString(const char* str){
    int cnt = 0;
    while(1){
        if(str[cnt] == '\0') return;
        LCD_writeChar(str[cnt]);
        cnt++;
    }
}

int cursorLineCommand[] = {0x80, 0xC0 , 0x90, 0xD0};

void LCD_setCursor(int x, int y){
    LCD_writeData(cursorLineCommand[y]+x, 0, 600);
    curX = x;
    curY = y;
}


const char* hexChars = "0123456789ABCDEF";

void LCD_writeIntHex(int val){
    LCD_writeString("0x");
    for(int i=7; i>=0; i--){
        LCD_writeChar(hexChars[(val >> (i*4)) & 0xF]);
    }
}

//only prints a byte
void LCD_writeByteBin(int val){
    LCD_writeString("0b");
    for(int i=7; i>=0; i--){
        LCD_writeChar((val & (1 << i)) ? '1' : '0');
    }
}


void LCD_writeInt(int val){
    // Handle negative numbers
    if(val < 0){
        LCD_writeChar('-');
        val = -val;
    }
    // Find highest digit
    int div = 1000000000;
    int started = 0;
    for(; div >= 1; div /= 10){
        int digit = val / div;
        if(digit || started || div == 1){
            LCD_writeChar('0' + digit);
            started = 1;
        }
        val %= div;
    }
}