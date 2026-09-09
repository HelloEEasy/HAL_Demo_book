#include "main.h"
#include "stm32f1xx_it.h"

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
    while (1)
    {
    }
}

void MemManage_Handler(void)
{
    while (1)
    {
    }
}

void BusFault_Handler(void)
{
    while (1)
    {
    }
}

void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
    /*
     * 更新 HAL 毫秒时间基准。HAL_GetTick()、带超时参数的 HAL API 和
     * HAL_Delay() 都依赖 SysTick 每次进入时执行该调用。
     */
    HAL_IncTick();
}
