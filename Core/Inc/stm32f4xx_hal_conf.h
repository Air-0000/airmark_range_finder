/**
 * @file stm32f4xx_hal_conf.h
 * @brief HAL configuration header for STM32F407ZGT6
 */
#ifndef __STM32F4xx_HAL_CONF_H
#define __STM32F4xx_HAL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
#include "system_stm32f4xx.h"

/* Module selection ----------------------------------------------------------*/
/* Only enable modules that have corresponding source files */
#define HAL_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED   /* NVIC, SysTick */
#define HAL_CRC_MODULE_ENABLED      /* CRC calculation unit */
#define HAL_DCMI_MODULE_ENABLED     /* Camera interface */
#define HAL_DMA_MODULE_ENABLED      /* DMA controller */
#define HAL_GPIO_MODULE_ENABLED     /* GPIO */
#define HAL_I2C_MODULE_ENABLED      /* I2C - OV2640 configuration */
#define HAL_RCC_MODULE_ENABLED      /* Reset and clock control */
#define HAL_TIM_MODULE_ENABLED      /* Timer - PWM for LED driver */

/* NOTE: HAL_ADC_MODULE_ENABLED and HAL_UART_MODULE_ENABLED removed - no source files */

/* Exported constants --------------------------------------------------------*/

/* Clock values (Hz) */
#define HSI_VALUE                16000000U
#define HSE_VALUE                8000000U
#define HSE_STARTUP_TIMEOUT      100U
#define LSI_VALUE                32000U
#define LSE_VALUE                32768U
#define LSE_STARTUP_TIMEOUT      5000U

/* IRQ priority configuration */
#ifndef VECT_TAB_OFFSET
#define VECT_TAB_OFFSET  0x00000000U  /* Vector Table base offset (FLASH at 0x08000000) */
#endif

/* Tick configuration */
#define HAL_TICK_FREQ_10HZ         100U
#define HAL_TICK_FREQ_100HZ        10U
#define HAL_TICK_FREQ_1KHZ         1U
#define HAL_TICK_FREQ_DEFAULT      HAL_TICK_FREQ_1KHZ

/* DMA configuration */
#define DMAx                      DMA2
#define DMA_CHANNELx              DMA_CHANNEL_1

/* Exported macros -----------------------------------------------------------*/

/* AHB1 bus clock enable */
#define __HAL_RCC_GPIOA_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN); \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOAEN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_GPIOB_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN); \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOBEN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_GPIOC_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN); \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOCEN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_GPIOD_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN); \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_GPIOE_CLK_ENABLE()   do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN); \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIOEEN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

/* AHB2 bus clock enable */
#define __HAL_RCC_DCMI_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DCMIEN); \
                                        tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_DCMIEN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_CRC_CLK_ENABLE()     do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB2ENR, RCC_AHB2ENR_CRCEN); \
                                        tmpreg = READ_BIT(RCC->AHB2ENR, RCC_AHB2ENR_CRCEN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

/* DMA */
#define __HAL_RCC_DMA2_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA2EN); \
                                        tmpreg = READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_DMA2EN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

/* APB1 clock enable (max 42MHz) */
#define __HAL_RCC_TIM2_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN); \
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_TIM3_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN); \
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_TIM4_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM4EN); \
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM4EN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_TIM5_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM5EN); \
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM5EN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_I2C1_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN); \
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C1EN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_I2C2_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C2EN); \
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_I2C2EN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_USART2_CLK_ENABLE()  do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN); \
                                        tmpreg = READ_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

/* APB2 clock enable (max 84MHz) */
#define __HAL_RCC_TIM1_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN); \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_TIM8_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM8EN); \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM8EN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_USART1_CLK_ENABLE()  do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN); \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_USART6_CLK_ENABLE()  do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART6EN); \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_USART6EN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_SPI1_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN); \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_SPI4_CLK_ENABLE()    do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI4EN); \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI4EN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

#define __HAL_RCC_SYSCFG_CLK_ENABLE()  do { \
                                        __IO uint32_t tmpreg; \
                                        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN); \
                                        tmpreg = READ_BIT(RCC->APB2ENR, RCC_APB2ENR_SYSCFGEN); \
                                        UNUSED(tmpreg); \
                                      } while(0U)

/* EXTI interrupt clear */
#define __HAL_GPIO_EXTI_CLEAR_IT(__EXTI_LINE__)  do { \
                                                    EXTI->PR = (__EXTI_LINE__); \
                                                  } while(0U)

/* Unused parameter - already defined in stm32f4xx_hal_def.h */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_CONF_H */
