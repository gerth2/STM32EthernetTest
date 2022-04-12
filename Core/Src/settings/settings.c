#include "settings.h"


const userSettings dfltSettings = {
		"network_imu",
		{ 10, 111, 76, 123 },
		{ 255, 255, 255, 0 },
		{ 10, 111, 76, 1 },
		{ 1, 1, 1, 1 }
};

userSettings curSettings;

void copy_settings(userSettings * dst, userSettings * src){
	memcpy(dst, src, sizeof(userSettings));
}

void settings_init(){
	copy_settings(&curSettings, &dfltSettings); //TODO read from EEPROM
}

void settings_save(){
	//TODO write to EEPROM
	//TODO reboot?
}

