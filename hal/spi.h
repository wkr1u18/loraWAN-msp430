/*
 * spi.h
 *
 *  Created on: May 3, 2019
 *      Author: wkr1u18
 */

#ifndef HAL_SPI_H_
#define HAL_SPI_H_

void init_spi(void);
void spi_nss(uint8_t  val);
void spi_transmit(uint8_t val);
uint8_t spi_receive(void);

#endif /* HAL_SPI_H_ */
