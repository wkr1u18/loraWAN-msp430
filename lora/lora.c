/*
 * lora.c
 *
 *  Created on: May 8, 2019
 *      Author: wkr1u18
 */
#include "lora.h"

// RADIO STATE
// (initialized by radio_init(), used by radio_rand1()) !!! Assume random for now
static uint8_t randbuf[16];

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
    DELAY_MICROS(100);
    hal_pin_rst(2);
    DELAY_MILIS(5);

<<<<<<< HEAD
    //Go to sleep
    opmode(OPMODE_SLEEP);

    //uint8_t version = readReg(RegVersion);

=======
    //Put Radio into Sleep state (default state)
    opmode(OPMODE_SLEEP);

    //TODO: Initialize randbuf[] to be true random
    randbuf[0] = 0x16;
>>>>>>> branch 'master' of https://github.com/wkr1u18/loraWAN-msp430.git
}

<<<<<<< HEAD
void txlora() {
    opmode_lora();
    //Warm-up - go to standby mode
    opmode(OPMODE_STANDBY);
=======
static void opmode (uint8_t mode) {
    writeReg(RegOpMode, (readReg(RegOpMode) & ~OPMODE_MASK) | mode);
>>>>>>> branch 'master' of https://github.com/wkr1u18/loraWAN-msp430.git
}

static void writeReg (uint8_t addr, uint8_t data ) {
    hal_pin_nss(0);
    hal_spi(addr | 0x80);
    hal_spi(data);
    hal_pin_nss(1);
}

static uint8_t readReg (uint8_t addr) {
    hal_pin_nss(0);
    hal_spi(addr & 0x7F);
    uint8_t val = hal_spi(0x00);
    hal_pin_nss(1);
    return val;
}

static void writeBuf (uint8_t addr, uint8_t* buf, uint8_t len) {
    hal_pin_nss(0);
    hal_spi(addr | 0x80);
    for (uint8_t i=0; i<len; i++) {
        hal_spi(buf[i]);
    }
    hal_pin_nss(1);
}

static void readBuf (uint8_t addr, uint8_t* buf, uint8_t len) {
    hal_pin_nss(0);
    hal_spi(addr & 0x7F);
    for (uint8_t i=0; i<len; i++) {
        buf[i] = hal_spi(0x00);
    }
    hal_pin_nss(1);
}

static void opmodeLora() {
    uint8_t u = OPMODE_LORA;
    u |= 0x8;   // TBD: sx1276 high freq
    writeReg(RegOpMode, u);
}

static void configLoraModem (){

}
static void configChannel (){

}
static void configPower (){

}

static void txlora (uint8_t * frame_ptr, uint8_t len) {
    // select LoRa modem (from sleep mode)
    //writeReg(RegOpMode, OPMODE_LORA);
    opmodeLora();

    // enter standby mode (required for FIFO loading))
    opmode(OPMODE_STANDBY);
    // configure LoRa modem (cfg1, cfg2)
    configLoraModem();
    // configure frequency
    configChannel();
    // configure output power
    writeReg(RegPaRamp, (readReg(RegPaRamp) & 0xF0) | 0x08); // set PA ramp-up time 50 uSec
    configPower();
    // set sync word
    writeReg(LORARegSyncWord, LORA_MAC_PREAMBLE);

    // set the IRQ mapping DIO0=TxDone DIO1=NOP DIO2=NOP
    writeReg(RegDioMapping1, MAP_DIO0_LORA_TXDONE|MAP_DIO1_LORA_NOP|MAP_DIO2_LORA_NOP);
    // clear all radio IRQ flags
    writeReg(LORARegIrqFlags, 0xFF);
    // mask all IRQs but TxDone
    writeReg(LORARegIrqFlagsMask, ~IRQ_LORA_TXDONE_MASK);

    // initialize the payload size and address pointers
    writeReg(LORARegFifoTxBaseAddr, 0x00);
    writeReg(LORARegFifoAddrPtr, 0x00);
    writeReg(LORARegPayloadLength, len);

    // download buffer to the radio FIFO
    writeBuf(RegFifo, frame_ptr, len);

    // enable antenna switch for TX
    hal_pin_rxtx(1);

    // now we actually start the transmission
    opmode(OPMODE_TX);
}
