//writes to a register and checks it afterwards
void reg_write_check(volatile int* reg, int val);
//writes to a register and checks only the bits in the mask afterwards
void reg_write_check_mask(volatile int* reg, int val, int mask);