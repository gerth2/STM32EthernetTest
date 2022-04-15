
#include "debugUtils.h"

void threadSafePrintf(const char *fmt, ...) {
    va_list args;
    taskENTER_CRITICAL();
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    taskEXIT_CRITICAL();
}
