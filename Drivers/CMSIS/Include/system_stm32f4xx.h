/**
 * @file system_stm32f4xx.h
 * @brief STM32F4xx system header
 */
#ifndef __SYSTEM_STM32F4XX_H
#define __SYSTEM_STM32F4XX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Exported constants --------------------------------------------------------*/
#define HSE_VALUE    8000000U
#define HSI_VALUE    16000000U

/* Exported variables --------------------------------------------------------*/
extern uint32_t SystemCoreClock;

/* Exported functions --------------------------------------------------------*/
void SystemInit(void);
void SystemCoreClockUpdate(void);

#ifdef __cplusplus
}
#endif

#endif /* __SYSTEM_STM32F4XX_H */