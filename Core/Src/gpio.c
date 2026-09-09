#include "gpio.h"

void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef gpio_init = {0};

    /* 配置或访问 PC13 之前必须先开启 GPIOC 外设时钟。 */
    __HAL_RCC_GPIOC_CLK_ENABLE();

    /*
     * 在把 PC13 切换为输出模式前，先写入期望的输出锁存值。
     *
     * 常见 Blue Pill 板载 LED 为低有效，因此 SET 高电平表示熄灭。先写入安全
     * 电平，再将引脚从复位后的输入状态切换为推挽输出，可减少上电瞬间闪亮。
     */
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

    gpio_init.Pin = LED_Pin;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GPIO_Port, &gpio_init);
}
