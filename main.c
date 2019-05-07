#include "driverlib.h"
#include "hal.h"

void main(void)
{
    //Stop watchdog timer
    WDT_A_hold(WDT_A_BASE);
    hal_init();
    PMM_unlockLPM5();

    //Reset Radio
    hal_pin_rst(0);
    DELAY_MILIS(100);
    hal_pin_rst(2);
    DELAY_MILIS(5);

    while(1) {
        hal_pin_nss(0);
        hal_spi(0x43 & 0x7F);
        uint8_t val = hal_spi(0x00);
        hal_pin_nss(1);
    }
}

