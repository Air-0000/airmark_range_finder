/**
 * @file stm32f4xx_hal_cortex.c
 * @brief CORTEX HAL driver
 */
#include "stm32f4xx_hal.h"

/**
 * @brief  Enable or disable the SYSTICK clock source
 */
void HAL_SYSTICK_CLKSourceConfig(uint32_t Src)
{
    (void)Src;
}

/**
 * @brief  Configure the SysTick
 */
uint32_t HAL_SYSTICK_Config(uint32_t ticks)
{
    (void)ticks;
    return 0U;
}

/**
 * @brief  This function handles SYSTICK interrupt request
 */
void SysTick_Handler(void)
{
    HAL_SYSTICK_IRQHandler();
}

/**
 * @brief  SYSTICK interrupt handler (weak, can be overridden)
 */
void __attribute__((weak)) HAL_SYSTICK_IRQHandler(void)
{
    HAL_IncTick();
}

/**
 * @brief  Set the priority for SYSTICK interrupt
 */
void HAL_SYSTICK_PriorityConfig(uint32_t Priority)
{
    NVIC_SetPriority(SysTick_IRQn, Priority & 0xFFUL);
}

/**
 * @brief  Enable or disable the SYSTICK interrupt
 */
void HAL_SYSTICK_INTConfig(FunctionalState State)
{
    if (State != DISABLE) {
        SysTick->CTRL |= SYSTICK_CTRL_TICKINT;
    } else {
        SysTick->CTRL &= ~SYSTICK_CTRL_TICKINT;
    }
}
