#define IRQ_ID_RESET        1
#define IRQ_ID_NMI          2
#define IRQ_ID_HARD         3
#define IRQ_ID_MEMORY       4
#define IRQ_ID_BUS          5
#define IRQ_ID_USAGE        6
#define IRQ_ID_SVCALL       11
#define IRQ_ID_PENDSV       14
#define IRQ_ID_SYSTICK      15
#define IRQ_ID_INT_BASE     16

//allocates the vector table to ram so it can be modified
//sets a defualt function for all faults and exceptions but not interrupts
void IRQ_init();

//overrites the function pointer int the VT
//use IRQ_ID_ for exception number
void IRQ_setFunction(int exceptionNumber, void* func);

//enables the IRQ in the NVIC. only works on IRQs not exceptions
void IRQ_enable(int exceptionNumber);
//disables the IRQ in the NVIC. only works on IRQs not exceptions
void IRQ_disable(int exceptionNumber);

void IRQ_trigger(int exceptionNumber);

void IRQ_resetPending(int IRQ_ID);