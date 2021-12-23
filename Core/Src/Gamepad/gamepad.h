#include "usbd_customhid.h"
#include "usb_device.h"

//TODO - why do I have to extern this myself?
extern USBD_HandleTypeDef hUsbDeviceFS;

void GamepadInit();
void GamepadPeriodic();
