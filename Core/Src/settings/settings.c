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

void settings_getCurSettingsAsJson(char * buf){

	threadSafeSPrintf(buf, "{\"deviceName\":\"%s\",\"ucIPAddress\":\"%d:%d:%d:%d\",\"ucNetMask\":\"%d:%d:%d:%d\",\"ucGatewayAddress\":\"%d:%d:%d:%d\",\"ucDNSServerAddress\":\"%d:%d:%d:%d\",\"nt4ServerAddress\":\"%d:%d:%d:%d\"}",
			curSettings.deviceName,
			curSettings.ucIPAddress[0],curSettings.ucIPAddress[1],curSettings.ucIPAddress[2],curSettings.ucIPAddress[3],
			curSettings.ucNetMask[0],curSettings.ucNetMask[1],curSettings.ucNetMask[2],curSettings.ucNetMask[3],
			curSettings.ucGatewayAddress[0],curSettings.ucGatewayAddress[1],curSettings.ucGatewayAddress[2],curSettings.ucGatewayAddress[3],
			curSettings.ucDNSServerAddress[0],curSettings.ucDNSServerAddress[1],curSettings.ucDNSServerAddress[2],curSettings.ucDNSServerAddress[3],
			curSettings.nt4ServerAddress[0],curSettings.nt4ServerAddress[1],curSettings.nt4ServerAddress[2],curSettings.nt4ServerAddress[3]
			);

}

void settings_parseSettingsFromJson(char * buf){

}
