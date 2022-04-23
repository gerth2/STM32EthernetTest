#include <shutdown.h>

void reboot(void){
	__disable_irq();
    NVIC_SystemReset();
    while (1); //we should never get here
}

/**
 * Call when we want to restart the unit due to an expected condition
 * For example, user has changed settings which require full reinit of the
 * hardware
 */
void shutdown_restartExpected(){
	threadSafePrintf("[SHUTDOWN] Performing expected reboot...");

}

/**
 * Call when we want to restart the unit due to an unexpected condition
 * For example, stack overflow
 */
void shutdown_restartUnExpected(){
	threadSafePrintf("[SHUTDOWN] Performing unexpected reboot...");

}
