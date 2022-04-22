#include <statusLED.h>

void ledOFF(){
	 HAL_GPIO_WritePin(GPIO_LED_GPIO_Port, GPIO_LED_Pin, GPIO_PIN_RESET);
}

void ledON(){
	 HAL_GPIO_WritePin(GPIO_LED_GPIO_Port, GPIO_LED_Pin, GPIO_PIN_SET);
}

void statusLED_init(){
	ledON();
}

//Produce a 100ms update rate from a 10ms call rate
#define PERIODIC_CALL_DECIMATION 10
uint8_t callCounter = 0;


///////////////////////////////////////////////
// Pattern Definitions
#define _ 0,
#define X 1,

#define PATTERN_LEN 10
uint8_t pattern0[PATTERN_LEN] = {_ _ _ _ _ X X X X X};
uint8_t pattern1[PATTERN_LEN] = {X _ X _ X _ X _ X _};
uint8_t pattern2[PATTERN_LEN] = {X _ _ X _ _ _ _ _ _};

#undef _
#undef X

//////////////////////////////////////////////
// State variables
uint8_t networkUp = 0;
uint8_t clientConnected = 0;
uint8_t patternStep = 0;


void statusLED_setNetworkUp(uint8_t in){
	networkUp = in;
}

void statusLED_clientConnected(uint8_t in){
	clientConnected = in;
}
///////////////////////////////////////////////


void statusLED_update(){

	if(callCounter++ >= PERIODIC_CALL_DECIMATION){
		callCounter = 0;

		//100ms update
		uint8_t * curPattern = pattern0; //default
		if(clientConnected){
			curPattern = pattern1;
		}else if(networkUp){
			curPattern = pattern2;
		}

		if(curPattern[patternStep++]){
			ledON();
		} else {
			ledOFF();
		}

	}

}



