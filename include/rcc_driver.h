#ifndef RCC_DRIVER_H_
#define RCC_DRIVER_H_

#include "stm32f1xx.h"

/**
 * @brief Настройка системного тактирования на 72 МГц.
 * HSE (8 MHz) -> PLL (x9) -> SYSCLK (72 MHz)
 */
void RCC_Init(void);

#endif /* RCC_DRIVER_H_ */