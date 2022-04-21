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

// from https://stackoverflow.com/questions/9211601/parsing-ip-adress-string-in-4-single-bytes
void parseIPString(uint8_t * dst, char * str){
    size_t index = 0;

    dst[0] = 0;
    dst[1] = 0;
    dst[2] = 0;
    dst[3] = 0;

    while (*str) {
        if (isdigit((unsigned char)*str)) {
        	dst[index] *= 10;
        	dst[index] += *str - '0';
        } else {
            index++;
        }
        str++;
    }
}

void settings_parseSettingsFromJson(char * buf, int len){
	char tmpStr[35];
	int resStrLen;

	resStrLen = mjson_get_string(buf, len, "$.deviceName", tmpStr, 35);
	if(resStrLen > 0){
		threadSafePrintf("[SETTINGS] Got new deviceName %s\n", tmpStr);
		strcpy(curSettings.deviceName, tmpStr);
	}

	resStrLen = mjson_get_string(buf, len, "$.ucIPAddress", tmpStr, 35);
	if(resStrLen > 0){
		threadSafePrintf("[SETTINGS] Got new ucIPAddress %s\n", tmpStr);
		parseIPString(curSettings.ucIPAddress, tmpStr);
	}

	resStrLen = mjson_get_string(buf, len, "$.ucNetMask", tmpStr, 35);
	if(resStrLen > 0){
		threadSafePrintf("[SETTINGS] Got new ucNetMask %s\n", tmpStr);
		parseIPString(curSettings.ucNetMask, tmpStr);
	}

	resStrLen = mjson_get_string(buf, len, "$.ucGatewayAddress", tmpStr, 35);
	if(resStrLen > 0){
		threadSafePrintf("[SETTINGS] Got new ucGatewayAddress %s\n", tmpStr);
		parseIPString(curSettings.ucGatewayAddress, tmpStr);
	}

	resStrLen = mjson_get_string(buf, len, "$.ucDNSServerAddress", tmpStr, 35);
	if(resStrLen > 0){
		threadSafePrintf("[SETTINGS] Got new ucDNSServerAddress %s\n", tmpStr);
		parseIPString(curSettings.ucDNSServerAddress, tmpStr);
	}

	resStrLen = mjson_get_string(buf, len, "$.nt4ServerAddress", tmpStr, 35);
	if(resStrLen > 0){
		threadSafePrintf("[SETTINGS] Got new nt4ServerAddress %s\n", tmpStr);
		parseIPString(curSettings.nt4ServerAddress, tmpStr);
	}
}
