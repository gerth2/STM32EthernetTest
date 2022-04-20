
#include "circBuffer.h"




void circBuffer_init(circularBuffer * buff, double len, double defaultVal){
	buff->len = len;
	assert(len <= BUFFER_MAX_LEN);
	for(int idx = 0; idx < len; idx++){
		circBuffer_add(buff, defaultVal);
	}
}

double circBuffer_add(circularBuffer * buff, double newVal){
	buff->headPtr++;

	if(buff->headPtr > buff->len){
		buff->headPtr = 0;
	}

	double oldVal = buff->buffer[buff->headPtr];
	buff->runningAccum -= oldVal;

	buff->buffer[buff->headPtr] = newVal;
	buff->runningAccum += newVal;

	return oldVal;
}

double circBuffer_getAvg(circularBuffer * buff){
	return buff->runningAccum / (double) buff->len;
}
