#include "at24cxx.h"

#include <string.h>
#include <stdio.h>

int at24_SequentialRead(I2C_HandleTypeDef *hi2c, uint8_t DevAddress,
		uint16_t MemAddress, uint8_t *pData, uint16_t RxBufferSize);
int at24_SequentialWrite(I2C_HandleTypeDef *hi2c, uint8_t DevAddress,
		uint16_t MemAddress, uint8_t *pData, uint16_t TxBufferSize);

int EEPROM_WriteBytes(uint16_t MemAddress, uint8_t *pData,
		uint16_t TxBufferSize) {
	return at24_SequentialWrite(EEPROM_I2C_PTR, EEPROM_DEVICE_ADDR, MemAddress,
			pData, TxBufferSize);
}
int EEPROM_ReadBytes(uint16_t MemAddress, uint8_t *pData, uint16_t RxBufferSize) {
	return at24_SequentialRead(EEPROM_I2C_PTR, EEPROM_DEVICE_ADDR, MemAddress,
			pData, RxBufferSize);
}

int at24_SequentialRead(I2C_HandleTypeDef *hi2c, uint8_t DevAddress,
		uint16_t MemAddress, uint8_t *pData, uint16_t RxBufferSize) {
	taskENTER_CRITICAL();

	//Read in 16-byte chunks
	while ((RxBufferSize - 16) > 0) {
		while (HAL_I2C_Mem_Read(hi2c, (uint16_t) DevAddress,
				(uint16_t) MemAddress, I2C_MEMADD_SIZE_8BIT, pData,
				(uint16_t) 16, (uint32_t) 1000) != HAL_OK)
			;
		RxBufferSize -= 16;
		pData += 16;
		MemAddress += 16;
	}
	while (HAL_I2C_Mem_Read(hi2c, (uint16_t) DevAddress, (uint16_t) MemAddress,
			I2C_MEMADD_SIZE_8BIT, pData, (uint16_t) RxBufferSize,
			(uint32_t) 1000) != HAL_OK)
		;
	taskEXIT_CRITICAL();

	return 1;
}

int at24_SequentialWrite(I2C_HandleTypeDef *hi2c, uint8_t DevAddress,
		uint16_t MemAddress, uint8_t *pData, uint16_t TxBufferSize) {
	taskENTER_CRITICAL();

	//Write in 16-byte chunks
	while ((TxBufferSize - 16) > 0) {
		while (HAL_I2C_Mem_Write(hi2c, (uint16_t) DevAddress,
				(uint16_t) MemAddress, I2C_MEMADD_SIZE_8BIT, pData,
				(uint16_t) 16, 1000) != HAL_OK)
			;
		TxBufferSize -= 16;
		pData += 16;
		MemAddress += 16;
	}
	while (HAL_I2C_Mem_Write(hi2c, (uint16_t) DevAddress, (uint16_t) MemAddress,
			I2C_MEMADD_SIZE_8BIT, pData, (uint16_t) TxBufferSize, 1000)
			!= HAL_OK)
		;

	taskEXIT_CRITICAL();

	return 1;
}

