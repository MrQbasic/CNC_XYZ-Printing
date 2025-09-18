
#define WDT_REG_CONTROL 0x400E1850
#define WDT_REG_MODE    0x400E1854
#define WDT_REG_STATUS  0x400E1858


void WDT_disable(){
    int* reg = (int*) WDT_REG_MODE;
    *reg = 1 << 15; 
}