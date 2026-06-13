/**
 * @file stm32f4xx_hal_rcc.c
 * @brief RCC HAL driver source
 */
#include "stm32f4xx_hal_rcc.h"

/* Private macros ------------------------------------------------------------*/
#define CLOCKSWITCH_TIMEOUT_VALUE  5000U

/* Private variables ---------------------------------------------------------*/
static uint32_t SystemD2Clock = 168000000U;

/**
 * @brief Deinitialize RCC
 */
HAL_StatusTypeDef HAL_RCC_DeInit(void)
{
    return HAL_OK;
}

/**
 * @brief Configure oscillators
 */
HAL_StatusTypeDef HAL_RCC_OscConfig(void *OscInitStruct)
{
    (void)OscInitStruct;
    return HAL_OK;
}

/**
 * @brief Configure system clock
 */
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *ClkInitStruct, uint32_t FLatency)
{
    (void)ClkInitStruct;
    (void)FLatency;
    return HAL_OK;
}

/**
 * @brief Configure MCO
 */
void HAL_RCC_MCOConfig(uint32_t RCC_MCOx, uint32_t RCC_MCOSource, uint32_t RCC_MCODiv)
{
    (void)RCC_MCOx;
    (void)RCC_MCOSource;
    (void)RCC_MCODiv;
}

/**
 * @brief Enable CSS
 */
void HAL_RCC_EnableCSS(void)
{
}

/**
 * @brief Disable CSS
 */
void HAL_RCC_DisableCSS(void)
{
}

/**
 * @brief Get SYSCLK source
 */
uint32_t HAL_RCC_GetSYSCLKSource(void)
{
    return 0x00000000U;
}

/**
 * @brief Get HCLK frequency
 */
uint32_t HAL_RCC_GetHCLKFreq(void)
{
    return SystemD2Clock;
}

/**
 * @brief Get PCLK1 frequency
 */
uint32_t HAL_RCC_GetPCLK1Freq(void)
{
    return SystemD2Clock / 4;
}

/**
 * @brief Get PCLK2 frequency
 */
uint32_t HAL_RCC_GetPCLK2Freq(void)
{
    return SystemD2Clock / 2;
}

/**
 * @brief Get OSC config
 */
void HAL_RCC_GetOscConfig(void *OscInitStruct)
{
    (void)OscInitStruct;
}

/**
 * @brief Get clock config
 */
void HAL_RCC_GetClockConfig(RCC_ClkInitTypeDef *ClkInitStruct, uint32_t *pFLatency)
{
    (void)ClkInitStruct;
    (void)pFLatency;
}

/**
 * @brief RCC NMI IRQ handler
 */
void HAL_RCC_NMI_IRQHandler(void)
{
    HAL_RCC_CSSCallback();
}

/**
 * @brief RCC CSS callback
 */
void __attribute__((weak)) HAL_RCC_CSSCallback(void)
{
}