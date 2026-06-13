/**
 * @file stm32f4xx_hal.h
 * @brief STM32F4xx HAL driver header
 */
#ifndef __STM32F4xx_HAL_H
#define __STM32F4xx_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
#include "stm32f407xx.h"

/* HAL module enabled check */
#ifdef HAL_MODULE_ENABLED
#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_dcmi.h"
#include "stm32f4xx_hal_tim.h"
#include "stm32f4xx_hal_i2c.h"
#include "stm32f4xx_hal_crc.h"
#endif

/* Exported variables --------------------------------------------------------*/
extern uint32_t SystemCoreClock;
extern const uint32_t AHBPrescTable[16];
extern const uint32_t APBPrescTable[8];
extern const uint8_t PLLMulTable[16];

/* Exported functions --------------------------------------------------------*/
uint32_t HAL_Init(void);
uint32_t HAL_DeInit(void);
void HAL_MspInit(void);
void HAL_MspDeInit(void);
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority);
void HAL_IncTick(void);
uint32_t HAL_GetTick(void);
void HAL_Delay(uint32_t Delay);
HAL_StatusTypeDef HAL_SuspendTick(void);
HAL_StatusTypeDef HAL_ResumeTick(void);
uint32_t HAL_GetHalVersion(void);
uint32_t HAL_GetREVID(void);
uint32_t HAL_GetDEVID(void);
uint32_t HAL_GetUIDw0(void);
uint32_t HAL_GetUIDw1(void);
uint32_t HAL_GetUIDw2(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_H */