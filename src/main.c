#include "gpio_driver.h"
#include "rcc_driver.h"

typedef enum {
    STATE_RED,
    STATE_RED_YELLOW,
    STATE_GREEN,
    STATE_YELLOW,
    STATE_MAINTENANCE // Режим обслуживания
} TrafficState_t;

int main(void){
    // Инициализация систем
    RCC_Init();
    GPIO_Init();

    TrafficState_t state = STATE_RED;

    while (1)
    {
        // Проверка кнопки (режим обслуживания)
        if (PIN_Read()){
            state = STATE_MAINTENANCE;
            // Ждем отпускания кнопки, чтобы не проскакивать режимы
            while(PIN_Read()); 
            Delay_Ms(50); // Антидребезг
        }

        switch (state)
        {
        case STATE_RED:
            Traffic_SetLights(LIGHT_ON, LIGHT_OFF, LIGHT_OFF);
            Delay_Ms(2000);
            state = STATE_RED_YELLOW;
            break;
        
        case STATE_RED_YELLOW:
            // Теперь горят оба диода!
            Traffic_SetLights(LIGHT_ON, LIGHT_ON, LIGHT_OFF);
            Delay_Ms(2000);
            state = STATE_GREEN;
            break;

        case STATE_GREEN:
            Traffic_SetLights(LIGHT_OFF, LIGHT_OFF, LIGHT_ON);
            Delay_Ms(2000);
            state = STATE_YELLOW;
            break;

        case STATE_YELLOW:
            Traffic_SetLights(LIGHT_OFF, LIGHT_ON, LIGHT_OFF);
            Delay_Ms(2000);
            state = STATE_RED;
            break;

        case STATE_MAINTENANCE:
            // Мигаем желтым 5 раз
            for (int i=0; i < 5; i++){
                Traffic_SetLights(LIGHT_OFF, LIGHT_ON, LIGHT_OFF);
                Delay_Ms(200);
                Traffic_SetLights(LIGHT_OFF, LIGHT_OFF, LIGHT_OFF);
                Delay_Ms(200);
            }
            state = STATE_RED; // Возврат в нормальный режим
            break;
        }
    }
}