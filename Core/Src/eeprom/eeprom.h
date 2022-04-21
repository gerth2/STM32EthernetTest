#ifndef _EEPROM_H
#define _EEPROM_H

#include <at24cxx.h>
#include <stdint.h>


typedef struct eeprom_block {
	uint8_t blockID;
	uint16_t dataAddr;
	size_t dataLen;
	uint16_t checksumAddr;
} eeprom_block_t;

/**
 * Checksum.
 * Each EEPROM data block is succeeded by a 1 byte checksum.
 * The checksum is calculated as the sum of all the data bytes, ignoring rollover into 1 byte,
 * Followed by XOR with EEPROM_CHECKSUM_STRIPES to ensure single 8-byte values don't match checksum
 * on a brand new device.
 */
#define EEPROM_CHECKSUM_SIZE sizeof(uint8_t)
#define EEPROM_CHECKSUM_STRIPES 0x55

///////////////////////////////////////////////////////////
// EEPROM layout configure

/**
 * EEPROM block id's. Add new block ID's here.
 */
#define EEPROM_SETTINGS_BID 0
#define EEPROM_GYRO_X_OFFSET_BID 1
#define EEPROM_GYRO_Y_OFFSET_BID 2
#define EEPROM_GYRO_Z_OFFSET_BID 3
#define EEPROM_NUM_BLOCKS 4

/**
 * EEPROM block data sizes. Align these to what you intend to store there.
 */
#define EEPROM_SETTINGS_SIZE sizeof(userSettings)
#define EEPROM_GYRO_X_OFFSET_SIZE sizeof(float)
#define EEPROM_GYRO_Y_OFFSET_SIZE sizeof(float)
#define EEPROM_GYRO_Z_OFFSET_SIZE sizeof(float)


void eeprom_initBlocks(void);
int8_t eeprom_readBlock(uint8_t blockID, void * dst);
int8_t eeprom_writeBlock(uint8_t blockID, void * src);

#endif
