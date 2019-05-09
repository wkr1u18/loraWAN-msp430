/*
 * lora.c
 *
 *  Created on: May 8, 2019
 *      Author: wkr1u18
 */

#include "lora.h"
#include "hal.h"

#define RegOpMode       0x01
#define OPMODE_SLEEP    0x00
#define OPMODE_MASK     0x07
#define RegVersion      0x42
#define OPMODE_LORA     0x80
#define OPMODE_STANDBY  0x01

static uint8_t readReg (uint8_t addr) {
    hal_pin_nss(0);
    hal_spi(addr & 0x7F);
    uint8_t val = hal_spi(0x00);
    hal_pin_nss(1);
    return val;
}

static void writeReg (uint8_t addr, uint8_t data ) {
    hal_pin_nss(0);
    hal_spi(addr | 0x80);
    hal_spi(data);
    hal_pin_nss(1);
}

static void opmode (uint8_t mode) {
    writeReg(RegOpMode, (readReg(RegOpMode) & ~OPMODE_MASK) | mode);
}

static void opmode_lora() {
    writeReg(RegOpMode, OPMODE_LORA);
}

void init_lora () {
    //Reset Radio
    hal_pin_rst(0);
    DELAY_MILIS(100);
    hal_pin_rst(2);
    DELAY_MILIS(5);

    //Go to sleep
    opmode(OPMODE_SLEEP);

    //uint8_t version = readReg(RegVersion);

}

void txlora() {
    opmode_lora();
    //Warm-up - go to standby mode
    opmode(OPMODE_STANDBY);
}

