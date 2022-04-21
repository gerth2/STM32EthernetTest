/*
 * at24_hal_i2c.h
 *
 *  Created on: Sep,11,2015
 *      Author: Sina Darvishi
 */

#ifndef DRIVERS_MYLIB_AT24_HAL_I2C_H_
#define DRIVERS_MYLIB_AT24_HAL_I2C_H_

#include "main.h"

#define EEPROM_I2C_PTR &hi2c1
#define EEPROM_DEVICE_ADDR 0xA0

int EEPROM_WriteBytes(uint16_t MemAddress, uint8_t *pData,uint16_t TxBufferSize);
int EEPROM_ReadBytes(uint16_t MemAddress, uint8_t *pData,uint16_t RxBufferSize);


#endif /* DRIVERS_MYLIB_AT24_HAL_I2C_H_ */
