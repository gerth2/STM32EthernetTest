#include "eeprom.h"
#include "settings.h"

/**
 * EEPROM block table - add your blocks here
 */
eeprom_block_t eepromBlocks[EEPROM_NUM_BLOCKS] = {
		{EEPROM_SETTINGS_BID, 0, EEPROM_SETTINGS_SIZE},
		{EEPROM_GYRO_X_OFFSET_BID, 0, EEPROM_GYRO_X_OFFSET_SIZE},
		{EEPROM_GYRO_Y_OFFSET_BID, 0, EEPROM_GYRO_Y_OFFSET_SIZE},
		{EEPROM_GYRO_Z_OFFSET_BID, 0, EEPROM_GYRO_Z_OFFSET_SIZE}
};

//Private forward declarations
uint8_t calcChecksum(void * data, size_t len);


///////////////////////////////////////////////////////////
// EEPROM API

/**
 * Inits all fields in the eepromBlocks table
 */
void eeprom_initBlocks(){
	uint16_t addrPtr = 0x0000;

	for(uint8_t blockIdx = 0; blockIdx < EEPROM_NUM_BLOCKS; blockIdx++){
		eepromBlocks[blockIdx].dataAddr = addrPtr;
		addrPtr += eepromBlocks[blockIdx].dataLen;
		eepromBlocks[blockIdx].checksumAddr = addrPtr;
		addrPtr += EEPROM_CHECKSUM_SIZE;
	}
}

/**
 * Read the specified EEPROM block into the dst pointer
 * Returns 0 if the operation succeeded
 * Returns -1 if the operation failed (including checksum mismatch)
 */
int8_t eeprom_readBlock(uint8_t blockID, void * dst){

	uint8_t storedChecksum;

	for(uint8_t blockIdx = 0; blockIdx < EEPROM_NUM_BLOCKS; blockIdx++){
		if(eepromBlocks[blockIdx].blockID == blockID){
			//Block found

			//Read out data
			uint8_t dataReadSuccess = EEPROM_ReadBytes(eepromBlocks[blockIdx].dataAddr, (uint8_t *)dst, eepromBlocks[blockIdx].dataLen);
			//Read out checksum
			uint8_t checksumReadSuccess = EEPROM_ReadBytes(eepromBlocks[blockIdx].checksumAddr, &storedChecksum, EEPROM_CHECKSUM_SIZE);

			//Report status
			if(dataReadSuccess != 1 || checksumReadSuccess != 1){
				return -1; //EEPROM device read failure
			} else if( calcChecksum(dst, eepromBlocks[blockIdx].dataLen) != storedChecksum ){
				return -1; //Checksum invalid
			} else {
				return 0; //success
			}
		}
	}

	return -1; //Block ID not found
}

/**
 * Read the specified EEPROM block from the src pointer
 * Returns 0 if the operation succeeded
 * Returns -1 if the operation failed (including checksum mismatch)
 */
int8_t eeprom_writeBlock(uint8_t blockID, void * src){

	for(uint8_t blockIdx = 0; blockIdx < EEPROM_NUM_BLOCKS; blockIdx++){
		if(eepromBlocks[blockIdx].blockID == blockID){
			//Block found

			uint8_t checksumVal = calcChecksum(src, eepromBlocks[blockIdx].dataLen);

			//Write Data
			uint8_t dataReadSuccess = EEPROM_WriteBytes(eepromBlocks[blockIdx].dataAddr, (uint8_t *)src, eepromBlocks[blockIdx].dataLen);
			//Write checksum
			uint8_t checksumReadSuccess = EEPROM_WriteBytes(eepromBlocks[blockIdx].checksumAddr, &checksumVal, EEPROM_CHECKSUM_SIZE);

			//Report status
			if(dataReadSuccess != 1 || checksumReadSuccess != 1){
				return -1; //EEPROM device read failure
			} else {
				return 0; //success
			}
		}
	}

	return -1; //Block ID not found


}


uint8_t calcChecksum(void * data, size_t len){
	uint8_t retVal = 0;
	while(len-- > 0){
		retVal += *(uint8_t *)data;
	}
	retVal ^= EEPROM_CHECKSUM_STRIPES;
	return retVal;
}
