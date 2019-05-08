#include "driverlib.h"
#include "hal.h"
#include "lora.h"

void main(void)
{
    //Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);
    hal_init();
    PMM_unlockLPM5();

    init_lora();

    while(1) {
        hal_pin_nss(0);
        hal_spi(0x43 & 0x7F);
        uint8_t val = hal_spi(0x00);
        hal_pin_nss(1);
    }
}

