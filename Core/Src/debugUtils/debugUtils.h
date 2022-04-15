#ifndef _DEBUG_UTILS_H
#define _DEBUG_UTILS_H


#include <stdarg.h>
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

void threadSafePrintf(const char *fmt, ...);

#endif
