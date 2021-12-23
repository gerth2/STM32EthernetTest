#include "LEDBlinker.h"

uint8_t blinkCounter;

void ledOFF(){
	 HAL_GPIO_WritePin(GPIO_LED_GPIO_Port, GPIO_LED_Pin, GPIO_PIN_RESET);
}

void ledON(){
	 HAL_GPIO_WritePin(GPIO_LED_GPIO_Port, GPIO_LED_Pin, GPIO_PIN_SET);
}

void LEDBlinkerInit(){
	blinkCounter = 0;
	ledOFF();

}

void LEDBlinkerPeriodic(){

	 if(blinkCounter++ > 128){
		 ledOFF();
	 } else {
		 ledON();
	 }
}
