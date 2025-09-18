#define PIO_MODE_OUTPUT         0
#define PIO_MODE_INPUT_BUTTON   1
#define PIO_MODE_INPUT_INT      3
#define PIO_MODE_UNUSED         2
#define PIO_MODE_PERIPHERAL_A   10
#define PIO_MODE_PERIPHERAL_B   10
#define PIO_MODE_PERIPHERAL_C   10
#define PIO_MODE_PERIPHERAL_D   10

//Peripheral IDs
#define PID_PIO_GROUP_A  9
#define PID_PIO_GROUP_B 10
#define PID_PIO_GROUP_C 11
#define PID_PIO_GROUP_D 12
#define PID_PIO_GROUP_E 13

//Groups
#define PIO_GROUP_A 0
#define PIO_GROUP_B 1
#define PIO_GROUP_C 2
#define PIO_GROUP_D 3
#define PIO_GROUP_E 4


int PIO_setMode(int pins, int group, int mode);

//clears all bits an writes new ones
int PIO_output_write(int pins, int group);

int PIO_output_set(int pins, int group);
int PIO_output_clear(int pins, int group);

int PIO_getData(int group);
