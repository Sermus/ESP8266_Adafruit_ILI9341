/*
 * hspi.h
 *
 *  Created on: 12 џэт. 2015 у.
 *      Author: Sem
 */

#ifndef INCLUDE_HSPI_H_
#define INCLUDE_HSPI_H_

#include "driver/spi_register.h" // from 0.9.4 IoT_Demo
#include <ets_sys.h>
#include <osapi.h>
#include <os_type.h>
#include <gpio.h>

#define SPI         0
#define HSPI        1

extern void hspi_init(void);
extern void hspi_wait_ready(void);
extern void hspi_send_uint8(const uint8_t data);
extern void hspi_send_uint16(const uint16_t data);
extern void hspi_send_uint16_r(const uint16_t data, uint32_t repeats);
extern void hspi_send_uint32(const uint32_t data);
extern void hspi_send_data(const uint8_t * data, uint8_t datasize);

#endif /* INCLUDE_HSPI_H_ */
