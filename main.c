#include "driverlib.h"
#include "hal.h"
#include "lora.h"
#include "wan.h"

void main(void)
{
    //Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);
    hal_init();
    PMM_unlockLPM5();
    init_wan();
    uint32_t devaddr = 0x0;
    uint8_t port = 0;
    uint8_t data[2];
    uint8_t nwkKey[16] = {0};
    uint8_t artKey[16] = {0};
    data[0] = (uint8_t) 'h';
    data[1] = (uint8_t) 'i';
    uint8_t dlen = 2;
    uint8_t* frame = build_frame(devaddr, port, data, dlen, nwkKey, artKey);
    uint8_t txlen = 12+dlen;
    init_lora();
    txlora();
}

