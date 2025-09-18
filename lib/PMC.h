void PMC_enablePeripheralClock(int PID);

//state = 1 -> on
//state = 0 -> off 
void PMC_setUSBClock(int state);

void PMC_enableExternalClock();