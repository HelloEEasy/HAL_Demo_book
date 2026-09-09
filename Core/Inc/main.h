#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

void Error_Handler(void);

/*
 * 以下宏来自 CubeMX 的 LED 引脚标签，是本开发板的硬件映射。
 *
 * 常见 STM32F103C8T6 最小系统板使用 PC13 驱动板载 LED，并且通常为低电平
 * 点亮：RESET 表示亮，SET 表示灭。应用层不得直接使用这些宏；需要操作 LED
 * 的例程应由 BSP 统一管理端口、引脚和电气极性。
 */
#define LED_Pin       GPIO_PIN_13
#define LED_GPIO_Port GPIOC

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
