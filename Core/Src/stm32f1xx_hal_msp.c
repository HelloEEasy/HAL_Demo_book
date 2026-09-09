#include "main.h"

void HAL_MspInit(void)
{
    __HAL_RCC_AFIO_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();

    /*
     * 保留 SWD 下载和调试功能，同时释放只供 JTAG 使用的引脚。`.ioc` 中的
     * "Serial Wire" 配置与此设置对应。
     */
    __HAL_AFIO_REMAP_SWJ_NOJTAG();
}
