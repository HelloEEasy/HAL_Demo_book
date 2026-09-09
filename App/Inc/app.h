#ifndef APP_H
#define APP_H

/**
 * @brief 初始化应用层拥有的模块和状态。
 *
 * @note 所需 CubeMX `MX_xxx_Init()` 全部完成后调用一次。
 * @note 不允许在中断上下文调用。
 */
void APP_Init(void);

/**
 * @brief 执行一次非阻塞应用处理。
 *
 * @note 由主循环持续调用。
 * @note 如果确需加入长时间阻塞操作，必须说明它对其他协作模块的影响。
 */
void APP_Process(void);

#endif /* APP_H */
