void LCD_init();

void LCD_writeData(int data, int type, int delay);

void LCD_writeChar(char c);

void LCD_clearScreen();

void LCD_setCursor(int x, int y);

void LCD_writeString(const char* str);

void LCD_writeIntHex(int val);

void LCD_writeInt(int val);

void LCD_writeByteBin(int val);