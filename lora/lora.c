/*
 * lora.c
 *
 *  Created on: May 8, 2019
 *      Author: wkr1u18
 */

#include "lora.h"
#include "hal.h"

void init_lora () {
    //Reset Radio
    hal_pin_rst(0);
    DELAY_MILIS(100);
    hal_pin_rst(2);
    DELAY_MILIS(5);
}


