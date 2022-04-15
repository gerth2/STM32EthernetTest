
#include <perfMon.h>

size_t perfmon_getFreeHeap(){
	return xPortGetFreeHeapSize();
}
