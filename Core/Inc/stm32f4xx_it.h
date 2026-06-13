/**
 * @file stm32f4xx_it.h
 * @brief Interrupt handling header for STM32F407
 */
#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported constants --------------------------------------------------------*/
/* DCMI interrupt requests */
#define DCMI_IRQn                DCMI_IRQn
#define DCMI_IRQHandler          DCMI_IRQHandler

/* DMA2 stream 1 interrupt requests (DCMI) */
#define DMA2_Stream1_IRQn        DMA2_Stream1_IRQn
#define DMA2_Stream1_IRQHandler  DMA2_Stream1_IRQHandler

/* TIM2 interrupt requests (LED PWM) */
#define TIM2_IRQn                TIM2_IRQn
#define TIM2_IRQHandler          TIM2_IRQHandler

/* TIM3 interrupt requests (camera trigger) */
#define TIM3_IRQn                TIM3_IRQn
#define TIM3_IRQHandler          TIM3_IRQHandler

/* Exported functions --------------------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);
void DCMI_IRQHandler(void);
void DMA2_Stream1_IRQHandler(void);
void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_IT_H */