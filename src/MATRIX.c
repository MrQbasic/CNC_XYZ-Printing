#define MATRIX_REG_CCFG_SYSIO 0x400E0314

void MATRIX_IO_USB(int state){
    volatile int* reg = (int*) MATRIX_REG_CCFG_SYSIO;
    if(state) *reg |=  ((1 << 10)|(1 << 11));
    else      *reg &= ~((1 << 10)|(1 << 11));
    
}