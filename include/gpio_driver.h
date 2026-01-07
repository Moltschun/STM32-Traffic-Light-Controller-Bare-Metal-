
#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_

#include "stm32f1xx.h"
#include <stdbool.h> // Используем угловые скобки для стандартных библиотек

// Определение пинов для удобства
#define BTN_PIN_RED  0  // PA0
#define BTN_PIN_YELLOW 2 // PA2
#define BTN_PIN_GREEN 4 // PA4
#define BTN_PIN_BUTTON 7 // PA7

#define LIGHT_ON true
#define LIGHT_OFF false


void GPIO_Init(void);

bool PIN_Read(void);
void Traffic_SetLights(bool red, bool yellow, bool green);
void Delay_Ms(uint32_t ms);

#endif /* GPIO_DRIVER_H_ */