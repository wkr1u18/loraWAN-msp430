#include "driverlib.h"
#include "spi.h"

void init_spi() {
    /*
     * Select Port 2
     * Set Pin 0, 1 to input Secondary Module Function, (UCA0TXD/UCA0SIMO, UCA0RXD/UCA0SOMI).
     */
    GPIO_setAsPeripheralModuleFunctionInputPin(
            GPIO_PORT_P7,
            GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2,
            GPIO_PRIMARY_MODULE_FUNCTION
    );

    //Here should be GPIO configuration for NSS

    //Initialize Master
    EUSCI_A_SPI_initMasterParam param = {0};
    param.selectClockSource = EUSCI_A_SPI_CLOCKSOURCE_SMCLK;
    param.clockSourceFrequency = CS_getSMCLK();
    param.desiredSpiClock = 500000;
    param.msbFirst = EUSCI_A_SPI_MSB_FIRST;
    param.clockPhase = EUSCI_A_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT; // CPOL = 0 and CPHA = 0
    param.clockPolarity = EUSCI_A_SPI_CLOCKPOLARITY_INACTIVITY_LOW; //Worth checking, whether needed
    param.spiMode = EUSCI_A_SPI_3PIN;
    EUSCI_A_SPI_initMaster(EUSCI_A2_BASE, &param);

    //Enable SPI module
    EUSCI_A_SPI_enable(EUSCI_A2_BASE);
}

void spi_nss(uint8_t val) {
    if(val==0) {
        GPIO_setOutputLowOnPin(
            GPIO_PORT_P7,
            GPIO_PIN3
        );
    } else {
        GPIO_setOutputHighOnPin(
            GPIO_PORT_P7,
            GPIO_PIN3
        );
    }
}

void spi_transmit(uint8_t TXData) {
    EUSCI_A_SPI_transmitData(EUSCI_A2_BASE, TXData);
}

uint8_t spi_receive(void) {
    return EUSCI_A_SPI_receiveData(EUSCI_A2_BASE);
}
