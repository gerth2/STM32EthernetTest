
#include <perfMon.h>

size_t perfmon_getFreeHeap(){
	return xPortGetFreeHeapSize();
}


void perfmon_printHeapStats(){

	//Assumes Heap 4
	HeapStats_t tmp;
	vPortGetHeapStats( &tmp );

	threadSafePrintf("[PERFMON]===============================\n");
	threadSafePrintf("[PERFMON] Heap Free: %d\n", tmp.xAvailableHeapSpaceInBytes);
	threadSafePrintf("[PERFMON] Max Free Block: %d\n", tmp.xSizeOfLargestFreeBlockInBytes);
	threadSafePrintf("[PERFMON] Min Free Block: %d\n", tmp.xSizeOfSmallestFreeBlockInBytes);
	threadSafePrintf("[PERFMON] Low Water Mark: %d\n", tmp.xMinimumEverFreeBytesRemaining);
	threadSafePrintf("[PERFMON] Allocs: %d\n", tmp.xNumberOfSuccessfulAllocations);
	threadSafePrintf("[PERFMON] Frees: %d\n", tmp.xNumberOfSuccessfulFrees);
	threadSafePrintf("[PERFMON]===============================\n");


}
