#include "main.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "app.h"
/* USER CODE END Includes */

static void SystemClock_Config(void);

int main(void)
{
    /*
     * 复位 HAL 管理的外设状态，并把 SysTick 配置为 HAL 时间基准。任何其他
     * HAL 服务都依赖这一步，因此它必须最先执行。
     */
    HAL_Init();

    /* 使用 8 MHz 外部晶振和 PLL 配置 72 MHz 系统时钟。 */
    SystemClock_Config();

    /* 调用 CubeMX 生成的硬件初始化；业务  模块初始化不放在这里。 */
    MX_GPIO_Init();

    /* USER CODE BEGIN 2 */
    APP_Init();
    /* USER CODE END 2 */

    while (1)
    {
        /* USER CODE BEGIN WHILE */
        APP_Process();
        /* USER CODE END WHILE */
    }
}

static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef oscillator = {0};
    RCC_ClkInitTypeDef clocks = {0};

    oscillator.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    oscillator.HSEState = RCC_HSE_ON;
    oscillator.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    oscillator.HSIState = RCC_HSI_ON;
    oscillator.PLL.PLLState = RCC_PLL_ON;
    oscillator.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    oscillator.PLL.PLLMUL = RCC_PLL_MUL9;

    if (HAL_RCC_OscConfig(&oscillator) != HAL_OK)
    {
        Error_Handler();
    }

    clocks.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                       RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clocks.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clocks.AHBCLKDivider = RCC_SYSCLK_DIV1;
    clocks.APB1CLKDivider = RCC_HCLK_DIV2;
    clocks.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&clocks, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

void Error_Handler(void)
{
    /*
     * 禁止中断并停留在此处，让调试器能够检查导致致命启动错误的调用栈以及
     * RCC/HAL 状态。产品工程可在此基础上增加错误记录或安全降级策略。
     */
    __disable_irq();
    while (1)
    {
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
    (void)file;
    (void)line;
    Error_Handler();
}
#endif
