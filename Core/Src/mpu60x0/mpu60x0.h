#ifndef MPU60x0_H_
#define MPU60x0_H_

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"


///////////////////////////////////////////////////////////////////////////////
// MPU6000 Defines and Variables
///////////////////////////////////////////////////////////////////////////////

// Registers

#define MPU6050_ADDR 0xD1 //addr pulled high


#define SMPLRT_DIV_REG 0x19
#define GYRO_CONFIG_REG 0x1B
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_XOUT_H_REG 0x43
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75


#define BITS_WHOAMI_EXP             0b01101000


// Public API
void mpu60x0_update(void);
void mpu60x0_init(I2C_HandleTypeDef i2cHandle);
float mpu60x0_getXAccel(void);
float mpu60x0_getYAccel(void);
float mpu60x0_getZAccel(void);
float mpu60x0_getXGyro(void);
float mpu60x0_getYGyro(void);
float mpu60x0_getZGyro(void);

#endif
