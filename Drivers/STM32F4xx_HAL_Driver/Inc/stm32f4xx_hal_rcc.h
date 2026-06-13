/**
 * @file stm32f4xx_hal_rcc.h
 * @brief RCC HAL driver
 */
#ifndef __STM32F4xx_HAL_RCC_H
#define __STM32F4xx_HAL_RCC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
#include "stm32f407xx.h"

/* RCC Clock configuration structure */
typedef struct {
    uint32_t ClockType;
    uint32_t SYSCLKSource;
    uint32_t AHBCLKDivider;
    uint32_t APB1CLKDivider;
    uint32_t APB2CLKDivider;
} RCC_ClkInitTypeDef;

/* RCC Oscillator Init structure */
typedef struct {
    uint32_t OscillatorType;
    uint32_t HSEState;
    uint32_t HSEPredivValue;
    uint32_t HSIState;
    uint32_t HSICalibrationValue;
    uint32_t LSIState;
    uint32_t LSEState;
    struct {
        uint32_t PLLState;
        uint32_t PLLSource;
        uint32_t PLLM;
        uint32_t PLLN;
        uint32_t PLLP;
        uint32_t PLLQ;
    } PLL;
} RCC_OscInitTypeDef;

#define RCC_CLOCKTYPE_HCLK      0x00000001U
#define RCC_CLOCKTYPE_SYSCLK    0x00000002U
#define RCC_CLOCKTYPE_PCLK1     0x00000004U
#define RCC_CLOCKTYPE_PCLK2     0x00000008U

#define RCC_SYSCLKSOURCE_PLL    0x00000000U
#define RCC_SYSCLKSOURCE_HSI    0x00000001U
#define RCC_SYSCLKSOURCE_HSE    0x00000002U

#define RCC_PLLSOURCE_HSI       0x00000000U
#define RCC_PLLSOURCE_HSE       0x00000001U

/* Exported functions */
HAL_StatusTypeDef HAL_RCC_DeInit(void);
HAL_StatusTypeDef HAL_RCC_OscConfig(void *OscInitStruct);
HAL_StatusTypeDef HAL_RCC_ClockConfig(RCC_ClkInitTypeDef *ClkInitStruct, uint32_t FLatency);
void HAL_RCC_MCOConfig(uint32_t RCC_MCOx, uint32_t RCC_MCOSource, uint32_t RCC_MCODiv);
void HAL_RCC_EnableCSS(void);
void HAL_RCC_DisableCSS(void);
uint32_t HAL_RCC_GetSYSCLKSource(void);
uint32_t HAL_RCC_GetHCLKFreq(void);
uint32_t HAL_RCC_GetPCLK1Freq(void);
uint32_t HAL_RCC_GetPCLK2Freq(void);
void HAL_RCC_GetOscConfig(void *OscInitStruct);
void HAL_RCC_GetClockConfig(RCC_ClkInitTypeDef *ClkInitStruct, uint32_t *pFLatency);
void HAL_RCC_NMI_IRQHandler(void);
void HAL_RCC_CSSCallback(void);

/* Oscillator types */
#define RCC_OSCILLATORTYPE_NONE       0x00000000U
#define RCC_OSCILLATORTYPE_HSE        0x00000001U
#define RCC_OSCILLATORTYPE_HSI        0x00000002U
#define RCC_OSCILLATORTYPE_LSI        0x00000004U
#define RCC_OSCILLATORTYPE_LSE        0x00000008U

/* HSE states */
#define RCC_HSE_OFF                   0x00000000U
#define RCC_HSE_ON                    0x00000001U
#define RCC_HSE_BYPASS               0x00000002U

/* PLL states */
#define RCC_PLL_NONE                  0x00000000U
#define RCC_PLL_OFF                   0x00000000U
#define RCC_PLL_ON                    0x00000001U

/* HSE predivider */
#define RCC_HSE_PREDIV_DIV1           0x00000000U
#define RCC_HSE_PREDIV_DIV2           0x00000001U

/* System clock sources */
#define RCC_SYSCLKSOURCE_PLLCLK       0x00000002U

/* AHB/APB prescalers */
#define RCC_SYSCLK_DIV1   0x00000000U
#define RCC_SYSCLK_DIV2   0x00001000U
#define RCC_SYSCLK_DIV4   0x00001400U
#define RCC_HCLK_DIV1     0x00000000U
#define RCC_HCLK_DIV2     0x00001000U
#define RCC_HCLK_DIV4     0x00001400U
#define RCC_HCLK_DIV8     0x00001800U
#define RCC_HCLK_DIV16    0x00001C00U

/* PLL source */
#define RCC_PLLSOURCE_HSE  0x00000001U
#define RCC_PLLSOURCE_HSI  0x00000000U

/* Flash latency */
#define FLASH_LATENCY_0    0x00000000U
#define FLASH_LATENCY_1    0x00000001U
#define FLASH_LATENCY_2    0x00000002U
#define FLASH_LATENCY_3    0x00000003U
#define FLASH_LATENCY_4    0x00000004U
#define FLASH_LATENCY_5    0x00000005U

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_RCC_H */