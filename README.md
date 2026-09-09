# `00_Template`

## 1. 模板功能

这是 STM32F103C8T6 最小系统板的基础工程。它完成 HAL 初始化、72 MHz
系统时钟配置、PC13 板载 LED 引脚初始化，并把应用入口固定为
`APP_Init()` 与 `APP_Process()`。模板本身不让 LED 闪烁。

## 2. 典型应用场景

- 复制后创建一个新的裸机 HAL 例程；
- 验证 ARM GCC、CMake、链接脚本和启动文件；
- 作为 CubeMX 与手写 App/BSP/Port 代码之间的稳定边界。

## 3. 硬件

- MCU：STM32F103C8T6
- Board：常见 STM32F103C8T6 minimum system board / Blue Pill
- Clock：8 MHz HSE，PLL × 9，SYSCLK 72 MHz
- Debug：PA13 SWDIO、PA14 SWCLK
- Optional LED：PC13，常见板型为低电平点亮

## 4. 硬件连接

模板只需要最小系统板供电和 SWD：

```text
ST-Link 3.3V  ── Board 3.3V
ST-Link GND   ── Board GND
ST-Link SWDIO ── PA13
ST-Link SWCLK ── PA14
```

## 5. 工作原理

复位后启动文件建立栈并调用 `main()`。`HAL_Init()` 建立 HAL 时间基准，
`SystemClock_Config()` 切换到 72 MHz，`MX_GPIO_Init()` 应用 CubeMX 硬件
配置，随后应用层在 super-loop 中获得持续执行机会。

## 6. 数据与控制流

```text
Reset → startup → main
                    ↓
       HAL/clock/CubeMX initialization
                    ↓
              APP_Init once
                    ↓
            APP_Process repeatedly
```

## 7. CubeMX 配置

### Clock

- HSE Crystal/Ceramic Resonator：8 MHz
- PLL source：HSE
- PLL multiplier：×9
- SYSCLK/HCLK：72 MHz
- APB1：36 MHz
- APB2：72 MHz

### GPIO

- PC13：`GPIO_Output`，label `LED`
- Initial output：High，使常见低有效 LED 保持熄灭
- Output mode：Push-Pull
- Speed：Low

### SYS

- Debug：Serial Wire，保留 SWD 并释放 JTAG-only 引脚
- Timebase：SysTick

## 8. 关键 HAL API

- `HAL_Init()`：复位 HAL 状态并建立 SysTick；在其他 HAL API 之前调用。
- `HAL_RCC_OscConfig()`：启动 HSE 和 PLL，失败时返回非 `HAL_OK`。
- `HAL_RCC_ClockConfig()`：选择 PLL 为系统时钟并配置总线分频、Flash 等待周期。
- `HAL_GPIO_Init()`：把 PC13 配置为推挽输出；不涉及中断或 DMA。
- `HAL_IncTick()`：由 SysTick ISR 每毫秒调用，维护 HAL 时间基准。

## 9. 软件架构

`Core/` 保留 CubeMX/启动代码；`App/` 只暴露应用入口；`BSP/`、`Port/`
在具体例程确有需要时使用。HAL/CMSIS 位于上级 `Platform/`，避免在每个
例程中重复一份第三方代码。

## 10. 程序执行流程

```text
HAL_Init
↓
SystemClock_Config
↓
MX_GPIO_Init
↓
APP_Init
↓
while (1) → APP_Process
```

## 11. 关键代码讲解

`main.c` 不承载业务逻辑。新例程应把行为放在 `App/Src/app.c`，把端口和
板级极性放在 BSP，把窄 HAL 适配放在 Port。CubeMX 重新生成后，应检查
`USER CODE` 区域和 CMake 自定义源文件列表。

## 12. 中断与 DMA 流程

模板只启用 Cortex-M SysTick，没有外设 IRQ 或 DMA。SysTick ISR 只更新
HAL tick，不执行应用逻辑。

## 13. 缓冲区与内存设计

模板没有应用缓冲区和动态分配。链接脚本配置 20 KB RAM、64 KB Flash，
默认 heap 0x200、stack 0x400。新增静态缓冲区后需检查链接器内存报告。

## 14. 构建方法

```powershell
cmake --preset Debug
cmake --build --preset Debug
```

Release：

```powershell
cmake --preset Release
cmake --build --preset Release
```

构建会生成 `.elf`、`.hex`、`.bin` 和 `.map`。

## 15. 硬件验证方法

模板没有可见闪烁行为。下载后应能停在 `APP_Process()`，并在调试器中确认：

- `SystemCoreClock == 72000000`
- PC13 配置寄存器为推挽输出
- 程序没有进入 `Error_Handler()`

## 16. 调试指南

在 `main()`、`SystemClock_Config()` 和 `Error_Handler()` 设置断点。若时钟
配置失败，检查板载晶振、供电、BOOT0 和 `.ioc` 的 HSE 模式。

## 17. 常见错误

- 从其他目录复用旧 `build/`：CMake 缓存会记录绝对路径，应重新配置。
- 未安装 `arm-none-eabi-gcc` 或 Ninja：Preset 无法完成配置。
- 将板载 LED 当作高电平有效：下载后观察到的亮灭逻辑会相反。

## 18. 迁移到其他 STM32

通常不变：`APP_Init/APP_Process` 约定和应用层结构。

必须重新配置：启动文件、链接脚本、芯片宏、时钟树、GPIO 映射、HAL/CMSIS
来源和 CMake CPU 选项。

## 19. 迁移到其他工程

复制所需 App/BSP/Port 模块，加入目标工程源文件与 include path；在目标
CubeMX 工程完成外设初始化后调用 `APP_Init()`，主循环调用 `APP_Process()`。

## 20. 迁移到 Keil MDK

- 不复制 `cmake/`、GNU 启动文件、`.ld` 或 `Platform/GCC`；
- 使用 Keil 工程自己的 startup、scatter file、HAL/CMSIS；
- 将 App/BSP/Port `.c` 加入 Source Group；
- 添加对应 `Inc` include path；
- 检查芯片宏和 C11 支持；
- 编译到 0 errors，并逐项处理 warnings。

## 21. 相关 C 语言知识

模块私有 `static`、固定宽度整数、头文件 include guard、函数作用域、链接脚本
与启动入口。

## 22. 面试题

1. 为什么 `HAL_Init()` 必须在外设 HAL API 之前执行？
2. APB1 为 36 MHz 时，为什么部分定时器时钟可能仍是 72 MHz？
3. 为什么业务代码不应直接堆在 `main.c`？

## 23. 工程化改进

实际产品可增加统一错误记录、看门狗、硬件自检和 CI 构建矩阵，但不应把这些
机制强塞进每个最小外设例程。

## 24. 快速复习

```text
MCU: STM32F103C8T6
Clock: HSE 8 MHz → PLL ×9 → 72 MHz
Entry: APP_Init() + APP_Process()
Build: CMake Presets + Ninja + ARM GCC
```
