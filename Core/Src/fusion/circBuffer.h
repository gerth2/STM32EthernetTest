
#ifndef _CIRC_BUFFER_H
#define _CIRC_BUFFER_H

#include <assert.h>

#define BUFFER_MAX_LEN 100

typedef struct circularBuffer{
	double buffer[BUFFER_MAX_LEN];
	unsigned int headPtr;
	unsigned int len;
	double runningAccum;
} circularBuffer;


void circBuffer_init(circularBuffer * buff, double len, double defaultVal);
double circBuffer_add(circularBuffer * buff, double newVal);
double circBuffer_getAvg(circularBuffer * buff);


#endif
