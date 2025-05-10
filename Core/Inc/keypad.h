#ifndef DEVICE_KEYPAD_H_
#define DEVICE_KEYPAD_H_

#include "stm32f4xx_hal.h"
#define PUSHED		0
#define RELEASED	1

void keypadInit();
uint8_t getKeypadState(uint8_t col, uint8_t row);
uint8_t keypadScan();
int key_cal(void);
#endif /* DEVICE_KEYPAD_H_ */
