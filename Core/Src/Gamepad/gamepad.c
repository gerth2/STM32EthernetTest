#include "gamepad.h"

uint8_t buffer[4]={0,0,0,0};
uint8_t counter = 0;

void GamepadInit(){

}

void GamepadPeriodic(){

  counter++;

  buffer[0]=0x00;  //Buttons
  buffer[1]=0x00;  //Buttons
  buffer[2]=counter; //  X Axis
  buffer[3]=0;       //  Y Axis

  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, buffer, sizeof(buffer));

}
