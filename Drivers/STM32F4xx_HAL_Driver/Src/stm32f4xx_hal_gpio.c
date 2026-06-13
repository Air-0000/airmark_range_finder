/**
 * @file stm32f4xx_hal_gpio.c
 * @brief GPIO HAL driver source
 */
#include "stm32f4xx_hal.h"

/**
 * @brief Initialize GPIO
 */
void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
    (void)GPIOx;
    (void)GPIO_Init;
    /* Stub implementation */
}

/**
 * @brief Deinitialize GPIO
 */
void HAL_GPIO_DeInit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin)
{
    (void)GPIOx;
    (void)GPIO_Pin;
    /* Stub implementation */
}

/**
 * @brief Read GPIO pin
 */
uint32_t HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    (void)GPIOx;
    (void)GPIO_Pin;
    return 0;
}

/**
 * @brief Write GPIO pin
 */
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t PinState)
{
    (void)GPIOx;
    (void)GPIO_Pin;
    (void)PinState;
}

/**
 * @brief Toggle GPIO pin
 */
void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    (void)GPIOx;
    (void)GPIO_Pin;
}

/**
 * @brief Lock GPIO pin
 */
HAL_StatusTypeDef HAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    (void)GPIOx;
    (void)GPIO_Pin;
    return HAL_OK;
}

/**
 * @brief GPIO EXTI IRQ handler
 */
void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin)
{
    (void)GPIO_Pin;
    HAL_GPIO_EXTI_Callback(GPIO_Pin);
}

/**
 * @brief GPIO EXTI callback
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    (void)GPIO_Pin;
}