#ifndef _MDNS_H
#define _MDNS_H

#include <stdint.h>
#include "socket.h"
#include <ctype.h>
#include "settings.h"
#include "debugUtils.h"
#define INSTANCE_LENGTH 20
#define SERVICE_LENGTH 30

#define IP_LEN 4

void advertise();

void mdns_init(void);
void mdns_update(void);

#endif
