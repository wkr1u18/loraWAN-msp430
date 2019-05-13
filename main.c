#include "driverlib.h"
#include "hal.h"
#include "lora.h"
#include "wan.h"
#include <stdio.h>
#include <msp430.h>

void check(u1_t expected, u1_t recorded) {
    if(expected==recorded) {
        hal_debug_led(1);
        DELAY_MILIS((unsigned int) 500);
        hal_debug_led(0);
    }

}

void main(void)
{
    //Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);
    hal_init();
    PMM_unlockLPM5();

    init_lora();
    init_wan();
    uint32_t devaddr = 0x26011E1C;
    uint8_t port = 1;
    uint8_t data[2];
    uint8_t nwkKey[16] = {0x61,0x46,0x48,0xFE,0xA1,0x7C,0x13,0x3C,0x38,0xF4,0xCA,0x09,0xFB,0xFC,0xE6,0x2F};
    uint8_t artKey[16] = {0x6D,0xB9,0x27,0x6B,0x45,0xFE,0x6F,0x5E,0xB1,0xB6,0xFF,0x6E,0xFA,0x71,0x69,0x62};
    data[0] = (uint8_t) 'h';
    data[1] = (uint8_t) 'i';
    uint8_t dlen = 2;
    uint8_t* frame = build_frame(devaddr, port, data, dlen, nwkKey, artKey);
    uint8_t txlen = 12+dlen;
    //txlora(frame, txlen);

}

