/**
 * @file stm32f4xx_hal_tim.c
 * @brief TIM HAL driver source
 */
#include "stm32f4xx_hal_tim.h"

/* Private variables ---------------------------------------------------------*/
static TIM_HandleTypeDef tim_handle_instance;

/**
 * @brief Initialize TIM
 */
HAL_StatusTypeDef HAL_TIM_Init(TIM_HandleTypeDef *htim)
{
    (void)htim;
    return HAL_OK;
}

/**
 * @brief Deinitialize TIM
 */
HAL_StatusTypeDef HAL_TIM_DeInit(TIM_HandleTypeDef *htim)
{
    (void)htim;
    return HAL_OK;
}

/**
 * @brief TIM Msp Init callback
 */
void __attribute__((weak)) HAL_TIM_MspInit(TIM_HandleTypeDef *htim)
{
    (void)htim;
}

/**
 * @brief TIM Msp DeInit callback
 */
void __attribute__((weak)) HAL_TIM_MspDeInit(TIM_HandleTypeDef *htim)
{
    (void)htim;
}

/**
 * @brief Initialize TIM base
 */
HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_InitTypeDef *htim)
{
    (void)htim;
    return HAL_OK;
}

/**
 * @brief Deinitialize TIM base
 */
HAL_StatusTypeDef HAL_TIM_Base_DeInit(TIM_HandleTypeDef *htim)
{
    (void)htim;
    return HAL_OK;
}

/**
 * @brief Start TIM base
 */
HAL_StatusTypeDef HAL_TIM_Base_Start(TIM_HandleTypeDef *htim)
{
    (void)htim;
    return HAL_OK;
}

/**
 * @brief Stop TIM base
 */
HAL_StatusTypeDef HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim)
{
    (void)htim;
    return HAL_OK;
}

/**
 * @brief Start TIM base with interrupt
 */
HAL_StatusTypeDef HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim)
{
    (void)htim;
    return HAL_OK;
}

/**
 * @brief Stop TIM base with interrupt
 */
HAL_StatusTypeDef HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim)
{
    (void)htim;
    return HAL_OK;
}

/**
 * @brief Initialize TIM PWM
 */
HAL_StatusTypeDef HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim)
{
    (void)htim;
    return HAL_OK;
}

/**
 * @brief Configure TIM PWM channel
 */
HAL_StatusTypeDef HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim, void *sConfig, uint32_t Channel)
{
    (void)htim;
    (void)sConfig;
    (void)Channel;
    return HAL_OK;
}

/**
 * @brief Start TIM PWM
 */
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel)
{
    (void)htim;
    (void)Channel;
    return HAL_OK;
}

/**
 * @brief Stop TIM PWM
 */
HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel)
{
    (void)htim;
    (void)Channel;
    return HAL_OK;
}

/**
 * @brief TIM IRQ handler
 */
void HAL_TIM_IRQHandler(TIM_HandleTypeDef *htim)
{
    (void)htim;
}

/**
 * @brief Get TIM base state
 */
uint32_t HAL_TIM_Base_GetState(TIM_HandleTypeDef *htim)
{
    (void)htim;
    return 0;
}