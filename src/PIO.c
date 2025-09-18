
#include "PIO.h"

#define PIO_REG_BASE          0x400E0E00
#define PIO_REG_GROUP_OFFSET  0x00000200

typedef struct  __attribute__((packed)){
    volatile int control_enable;
    volatile int control_disable;
    volatile int control_status;
    volatile int rsvd0;
    volatile int out_enable;
    volatile int out_disable;
    volatile int out_status;
    volatile int rsvd1;
    volatile int inp_glichFilter_enable;
    volatile int inp_glichFilter_disable;
    volatile int inp_glichFilter_status;
    volatile int rsvd2;
    volatile int out_data_set;
    volatile int out_data_clear;
    volatile int out_data_status;
    volatile int pin_data; //efectivly read
    volatile int int_enable;
    volatile int int_disable;
    volatile int int_mask;
    volatile int int_status;
    volatile int multi_driver_enable;
    volatile int multi_driver_disable;
    volatile int multi_driver_status;
    volatile int rsvd3;
    volatile int pull_up_disable;
    volatile int pull_up_enable;
    volatile int pull_up_status;
    volatile int rsvd4[2];
    volatile int peripheral_select1;
    volatile int peripheral_select2;
    volatile int rsvd5;
    volatile int inp_filter_slow_disable;
    volatile int inp_filter_slow_enable;
    volatile int inp_filter_slow_status;
    volatile int slow_divider_debounce;
    volatile int pull_down_disable;
    volatile int pull_down_enable;
    volatile int pull_down_status;
    volatile int rsvd6;
    //used to mask out_data_status for write
    volatile int out_write_disable;
    volatile int out_write_enable;      
    volatile int out_write_status;
    volatile int rsvd7;
    volatile int int_additional_enable;
    volatile int int_additional_disable;
    volatile int int_additional_mask;
    volatile int rsvd8;
    volatile int int_trigger_mode_edge;
    volatile int int_trigger_mode_level;
    volatile int int_trigger_mode_status;
    volatile int rsvd9;
    volatile int int_trigger_state_low_falling;
    volatile int int_trigger_state_high_rising;
    volatile int int_trigger_state_status;

} PIO_REG;


#define PIO_REG_ENABLE              0x0000
#define PIO_REG_DISABLE             0x0004
#define PIO_REG_OUT_ENABLE          0x0010
#define PIO_REG_OUT_DISABLE         0x0014
#define PIO_REG_OUT_DATA_SET        0x0030
#define PIO_REG_OUT_DATA_CLEAR      0x0034

int PIO_setMode(int pins, int group, int mode){
    volatile PIO_REG* pioReg = (PIO_REG*) (PIO_REG_BASE + (PIO_REG_GROUP_OFFSET * (group%5)));
    switch(mode){
        case PIO_MODE_OUTPUT:
            pioReg->control_enable = pins;
            pioReg->out_enable = pins;
            break;
        case PIO_MODE_INPUT_BUTTON:
            pioReg->control_enable = pins;
            pioReg->inp_glichFilter_enable = pins;
            pioReg->out_disable = pins;
            break;
        case PIO_MODE_INPUT_INT:
            pioReg->control_enable = pins;
            pioReg->out_disable = pins;
            pioReg->inp_glichFilter_enable = pins;
            pioReg->int_additional_enable = pins;
            pioReg->int_trigger_mode_edge = pins;
            pioReg->int_trigger_state_low_falling = pins;
            pioReg->int_enable =  pins;
            break;
        case PIO_MODE_UNUSED:
        return 1;
    }

    return 0;
}

#define PIO_PULL_NONE 0
#define PIO_PULL_UP   1
#define PIO_PULL_DOWN 2

int PIO_setPullResistor(int pins, int group, int pull){ 
    volatile PIO_REG* pioReg = (PIO_REG*) (PIO_REG_BASE + (PIO_REG_GROUP_OFFSET * (group%5)));
    switch(pull){
        case PIO_PULL_NONE:
            pioReg->pull_up_disable = pins;
            pioReg->pull_down_disable = pins;
            break;
        case PIO_PULL_UP:
            pioReg->pull_up_enable = pins;
            pioReg->pull_down_disable = pins;
            break;
        case PIO_PULL_DOWN:
            pioReg->pull_down_enable = pins;
            pioReg->pull_up_disable = pins;
            break;
    }
    return 0;
}


int PIO_output_write(int pins, int group){
    volatile PIO_REG* pioReg = (PIO_REG*) (PIO_REG_BASE + (PIO_REG_GROUP_OFFSET * (group%5)));
    pioReg->out_data_clear = ~pins;
    pioReg->out_data_set   =  pins;
    if(pioReg->out_data_status != pins) return 1;
    return 0;
}


int PIO_output_set(int pins, int group){
    volatile PIO_REG* pioReg = (PIO_REG*) (PIO_REG_BASE + (PIO_REG_GROUP_OFFSET * (group%5)));
    pioReg->out_data_set = pins;
    return 0;
}


int PIO_output_clear(int pins, int group){
    volatile PIO_REG* pioReg = (PIO_REG*) (PIO_REG_BASE + (PIO_REG_GROUP_OFFSET * (group%5)));
    pioReg->out_data_clear = pins;
    return 0;
}


int PIO_getData(int group){
    volatile PIO_REG* pioReg = (PIO_REG*) (PIO_REG_BASE + (PIO_REG_GROUP_OFFSET * (group%5)));
    return pioReg->pin_data;
}

