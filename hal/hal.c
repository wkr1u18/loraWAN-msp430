#include "driverlib.h"
#include "spi.h"
#include "hal.h"

void init_clock() {
    //Set DCO frequency to max DCO setting
    CS_setDCOFreq(CS_DCORSEL_0, CS_DCOFSEL_3);
    //Select DCO as the clock source for SMCLK with no frequency divider
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
}

void init_io(void) {

    // SPI chip select
    GPIO_setAsOutputPin(
            GPIO_PORT_P7,
            GPIO_PIN3
    );

    //DEBUG led
    GPIO_setAsOutputPin(
            GPIO_PORT_P1,
            GPIO_PIN0
    );


    //NSS is high on default
    GPIO_setOutputHighOnPin(
            GPIO_PORT_P7,
            GPIO_PIN3
    );

    // RX and TX switches
    // RX
    GPIO_setAsOutputPin(
            GPIO_PORT_P7,
            GPIO_PIN5
    );

    // TX
    GPIO_setAsOutputPin(
            GPIO_PORT_P7,
            GPIO_PIN6
    );

    // RF_RST
    GPIO_setAsOutputPin(
            GPIO_PORT_P8,
            GPIO_PIN0
    );

}

void hal_init(void) {
    init_clock();
    init_io();
    init_spi();
}

/*
 * drive radio NSS pin (0=low, 1=high).
 */
void hal_pin_nss(uint8_t val) {
    spi_nss(val);
}

/*
 * drive radio RX/TX pins (0=rx, 1=tx).
 */
void hal_pin_rxtx (uint8_t val) {
    if(val==0) { // RX
        GPIO_setOutputHighOnPin(
                GPIO_PORT_P7,
                GPIO_PIN5
        );
        GPIO_setOutputLowOnPin(
                GPIO_PORT_P7,
                GPIO_PIN6
        );
    } else { // TX
        GPIO_setOutputHighOnPin(
                GPIO_PORT_P7,
                GPIO_PIN6
        );
        GPIO_setOutputLowOnPin(
                GPIO_PORT_P7,
                GPIO_PIN5
        );
    }
}

void hal_debug_led(uint8_t val) {
    if(val==0) {
        GPIO_setOutputLowOnPin(
                GPIO_PORT_P1,
                GPIO_PIN0
        );
    }
    else {
        GPIO_setOutputHighOnPin(
                GPIO_PORT_P1,
                GPIO_PIN0
        );
    }
}

/*
 * control radio RST pin (0=low, 1=high, 2=floating)
 */
void hal_pin_rst (uint8_t val) {
    if (val == 0) {
        GPIO_setOutputLowOnPin(
                GPIO_PORT_P8,
                GPIO_PIN0
        );

    } else if (val == 1){
        GPIO_setOutputHighOnPin(
                GPIO_PORT_P8,
                GPIO_PIN0
        );

    } else {

        GPIO_setAsInputPin(
                GPIO_PORT_P8,
                GPIO_PIN0
        );


    }

}

/*
 * perform 8-bit SPI transaction with radio.
 *   - write given byte 'outval'
 *   - read byte and return value
 */
uint8_t hal_spi(uint8_t outval) {
    spi_transmit(outval);
    return spi_receive();
}

