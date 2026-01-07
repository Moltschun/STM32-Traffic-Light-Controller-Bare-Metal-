/**
 * @file gpio_driver.c
 * @brief Реализация функций драйвера.
 */

#include "gpio_driver.h"

void GPIO_Init(void){
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    
    GPIOA->CRL &= ~(0xF << (BTN_PIN_RED * 4)); // RED
    GPIOA->CRL &= ~(0xF << (BTN_PIN_YELLOW * 4)); // YELLOW
    GPIOA->CRL &= ~(0xF << (BTN_PIN_GREEN * 4)); // GREEN
    GPIOA->CRL &= ~(0xF << (BTN_PIN_BUTTON * 4)); // BUTTON
    
    GPIOA->CRL |= (0x2 << (BTN_PIN_RED * 4)); // RED
    GPIOA->CRL |= (0x2 << (BTN_PIN_YELLOW * 4)); // YELLOW
    GPIOA->CRL |= (0x2 << (BTN_PIN_GREEN * 4)); // GREEN


    GPIOA->CRL |= (0x8 << (BTN_PIN_BUTTON * 4));
    GPIOA->ODR |= (1 << BTN_PIN_BUTTON);
}

bool PIN_Read(void){
    // Проверяем 0-й бит регистра IDR.
    // Если (IDR & 1) == 0, значит кнопка замкнула пин на землю.
    if ((GPIOA->IDR & (1 << BTN_PIN_BUTTON)) == 0){
        return true; // Нажата
    }
    else{
        return false; // Отпущена
    }
}

void Traffic_SetLights(bool red, bool yellow, bool green){

    uint32_t reset_mask_led = (1 << (BTN_PIN_RED + 16)) | (1 << (BTN_PIN_YELLOW + 16)) | (1 << (BTN_PIN_GREEN + 16));
    uint32_t set_mask_led = 0;
    if (red) set_mask_led |= (1 << BTN_PIN_RED);
    if (yellow) set_mask_led |= (1 << BTN_PIN_YELLOW);
    if (green) set_mask_led |= (1 << BTN_PIN_GREEN);

    GPIOA->BSRR = reset_mask_led;
    GPIOA->BSRR = set_mask_led;
}

void Delay_Ms(uint32_t Ms){
    // Простой цикл задержки.
    // volatile нужен, чтобы компилятор не удалил цикл при оптимизации.
    for (volatile uint32_t i = 0; i < Ms * 7200; i++) {
        __NOP();
    }
}