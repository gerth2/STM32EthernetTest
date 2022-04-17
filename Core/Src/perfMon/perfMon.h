
#ifndef _PERFMON_H
#define _PERFMON_H

#include "FreeRTOS.h"
#include "debugUtils.h"

size_t perfmon_getFreeHeap(void);
void perfmon_printHeapStats(void);




#endif
