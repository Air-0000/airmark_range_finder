/**
 * @file stm32f4xx_hal.c
 * @brief HAL driver source
 */
#include "stm32f4xx_hal.h"

/* Private variables ---------------------------------------------------------*/
static uint32_t TickCounter = 0;

/**
 * @brief Initialize HAL
 */
uint32_t HAL_Init(void)
{
    SystemCoreClock = 168000000U;
    TickCounter = 0;
    HAL_MspInit();
    return HAL_OK;
}

/**
 * @brief Deinitialize HAL
 */
uint32_t HAL_DeInit(void)
{
    HAL_MspDeInit();
    return HAL_OK;
}

/**
 * @brief Initialize MSP
 */
void __attribute__((weak)) HAL_MspInit(void)
{
}

/**
 * @brief Deinitialize MSP
 */
void __attribute__((weak)) HAL_MspDeInit(void)
{
}

/**
 * @brief Increment tick
 */
void HAL_IncTick(void)
{
    TickCounter++;
}

/**
 * @brief Get tick
 */
uint32_t HAL_GetTick(void)
{
    return TickCounter;
}

/**
 * @brief Delay
 */
void HAL_Delay(uint32_t Delay)
{
    uint32_t tickstart = HAL_GetTick();
    while ((HAL_GetTick() - tickstart) < Delay) {
    }
}

/**
 * @brief Suspend tick
 */
HAL_StatusTypeDef HAL_SuspendTick(void)
{
    return HAL_OK;
}

/**
 * @brief Resume tick
 */
HAL_StatusTypeDef HAL_ResumeTick(void)
{
    return HAL_OK;
}

/**
 * @brief Get HAL version
 */
uint32_t HAL_GetHalVersion(void)
{
    return 0x01000000U;  /* 1.0.0 */
}

/**
 * @brief Get revision ID
 */
uint32_t HAL_GetREVID(void)
{
    return 0x20000000U;
}

/**
 * @brief Get device ID
 */
uint32_t HAL_GetDEVID(void)
{
    return 0x0400U;
}

/**
 * @brief Get UID word 0
 */
uint32_t HAL_GetUIDw0(void)
{
    return 0x12345678U;
}

/**
 * @brief Get UID word 1
 */
uint32_t HAL_GetUIDw1(void)
{
    return 0xABCDEF00U;
}

/**
 * @brief Get UID word 2
 */
uint32_t HAL_GetUIDw2(void)
{
    return 0x87654321U;
}