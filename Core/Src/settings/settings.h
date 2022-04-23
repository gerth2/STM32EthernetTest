#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "debugUtils.h"
#include "mjson.h"
#include "eeprom.h"
#include "shutdown.h"

typedef struct UserSettings {
  char deviceName[32];
  uint8_t ucIPAddress[4];
  uint8_t ucNetMask[4];
  uint8_t ucGatewayAddress[4];
  uint8_t ucDNSServerAddress[4];
  uint8_t nt4ServerAddress[4];
} userSettings;


extern userSettings curSettings;


void settings_init(void);
void settings_save(void);
void settings_getCurSettingsAsJson(char * buf);
void settings_parseSettingsFromJson(char * buf, int len);

#endif
