/**
 * @file rcc_driver.c
 * @brief Реализация функций настройки RCC.
 */

#include "rcc_driver.h"

void RCC_Init(void) {
    // 1. Включаем HSE (внешний кварц)
    RCC->CR |= RCC_CR_HSEON;
    // Ждем готовности (HSERDY)
    while (!(RCC->CR & RCC_CR_HSERDY));

    // 2. Настройка Flash Latency (2 такта для 72 МГц)
    FLASH->ACR |= FLASH_ACR_LATENCY_2;

    // 3. Настройка делителей шин
    // APB1 (Low Speed) макс 36 МГц -> делим на 2
    RCC->CFGR |= (RCC_CFGR_PPRE1_DIV2); 
    // APB2 и AHB оставляем без деления (DIV1 по умолчанию)

    // 4. Настройка PLL
    // Сначала очищаем настройки PLL
    RCC->CFGR &= ~(RCC_CFGR_PLLMULL | RCC_CFGR_PLLSRC);
    // Источник HSE, Множитель x9 (8*9 = 72)
    RCC->CFGR |= (RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9);

    // 5. Включаем PLL
    RCC->CR |= RCC_CR_PLLON;
    // Ждем готовности PLL (PLLRDY)
    while (!(RCC->CR & RCC_CR_PLLRDY));

    // 6. Переключаем System Clock на PLL
    RCC->CFGR &= ~RCC_CFGR_SW;      // Очистка
    RCC->CFGR |= RCC_CFGR_SW_PLL;   // Выбор PLL

    // 7. Ждем подтверждения переключения (SWS = 10)
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}