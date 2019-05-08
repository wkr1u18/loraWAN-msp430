/*
 * hal.h
 *
 *  Created on: May 7, 2019
 *      Author: wkr1u18
 */

#ifndef HAL_HAL_H_
#define HAL_HAL_H_
#include <stdint.h>
#define DELAY_MICROS(t) (__delay_cycles(8* (uint32_t) t))
#define DELAY_MILIS(t) (__delay_cycles(8000* (uint32_t) t))

void init_clock();
void init_io(void);
void hal_init(void);
void hal_pin_nss(uint8_t val);
void hal_pin_rxtx (uint8_t val);
void hal_pin_rst (uint8_t val);

uint8_t hal_spi(uint8_t outval);

#endif /* HAL_HAL_H_ */
