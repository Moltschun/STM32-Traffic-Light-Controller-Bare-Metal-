# STM32 Traffic Light Controller (Bare Metal)

![Language](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Platform](https://img.shields.io/badge/Platform-STM32F103-03234B?style=for-the-badge&logo=stmicroelectronics&logoColor=white)
![Architecture](https://img.shields.io/badge/Architecture-ARM_Cortex_M3-0091BD?style=for-the-badge&logo=arm&logoColor=white)

Проект системы управления светофором, реализованный на микроконтроллере **STM32F103C8T6 (Blue Pill)**.
Код написан на **чистом C с использованием регистров (CMSIS)**, без использования библиотек HAL или SPL, для глубокого понимания архитектуры MCU.

---

## Hardware Manifest (Аппаратная часть)

* **MCU:** STM32F103C8T6 "Blue Pill"
* **Debug:** ST-Link V2 (SWD)
* **Clock Speed:** 72 MHz (External Crystal 8 MHz + PLL x9)

### Pinout / Схема подключения

| Component | Pin (STM32) | Mode | Примечание |
| :--- | :--- | :--- | :--- |
| **RED LED** | `PA0` | Output Push-Pull | Красный сигнал |
| **YELLOW LED** | `PA2` | Output Push-Pull | Желтый сигнал |
| **GREEN LED** | `PA4` | Output Push-Pull | Зеленый сигнал |
| **BUTTON** | `PA7` | Input Pull-Up | Кнопка пешехода / Сервис |

---

## Architecture & Drivers (Архитектура)

Проект разделен на независимые драйверы для модульности.

### 1. RCC Driver (System Clock)
Драйвер переводит микроконтроллер с базовых 8 МГц на боевые **72 МГц**.
* **HSE:** Включение внешнего кварца.
* **PLL:** Умножение частоты (x9).
* **Flash Latency:** Настройка задержки доступа к памяти (2WS).
* **Bus Prescalers:** APB1 (36 MHz), APB2 (72 MHz).

> **Справка по регистрам RCC:**
> * **RCC_CR:** "Пульт управления". Включает HSE (`HSEON`), PLL (`PLLON`) и защиту (`CSSON`). Флаги `RDY` сообщают о готовности.
> * **RCC_CFGR:** "Маршрутизация". `PLLSRC` выбирает источник, `PLLMUL` задает множитель, `SW/SWS` переключают питание ядра.
> * **RCC_APB2ENR:** Включение питания периферии (GPIO, ADC, SPI1).

### 2. GPIO Driver
Управление портами ввода-вывода через прямое манипулирование регистрами `CRL`, `ODR`, `IDR`, `BSRR`.
* Используется атомарная запись через `BSRR` для предотвращения ошибок Read-Modify-Write.
* Реализована защита от наложения масок при включении нескольких светодиодов.

### 3. Main Logic (State Machine)
Логика светофора реализована через **Конечный Автомат (FSM)**:
* `STATE_RED` ➔ `STATE_RED_YELLOW` ➔ `STATE_GREEN` ➔ `STATE_YELLOW`
* **Maintenance Mode:** При нажатии кнопки система переходит в режим "Мигающий желтый".

---
