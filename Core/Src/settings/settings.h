#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <stdint.h>
#include <string.h>


typedef struct UserSettings {
  char deviceName[32];
  uint8_t ucIPAddress[4];
  uint8_t ucNetMask[4];
  uint8_t ucGatewayAddress[4];
  uint8_t ucDNSServerAddress[4];
} userSettings;


extern userSettings curSettings;


void settings_init(void);
void settings_save(void);

#endif
