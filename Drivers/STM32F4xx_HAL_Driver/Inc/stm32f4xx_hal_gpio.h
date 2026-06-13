/**
 * @file stm32f4xx_hal_gpio.h
 * @brief GPIO HAL driver
 */
#ifndef __STM32F4xx_HAL_GPIO_H
#define __STM32F4xx_HAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
#include "stm32f407xx.h"

/* GPIO Pin modes */
#define GPIO_MODE_INPUT         0x00000000U
#define GPIO_MODE_OUTPUT        0x00000001U
#define GPIO_MODE_AF            0x00000002U
#define GPIO_MODE_AF_PP         0x00000002U  /* Alternate function push-pull */
#define GPIO_MODE_ANALOG        0x00000003U
#define GPIO_MODE_IT_RISING     0x10110000U
#define GPIO_MODE_IT_FALLING    0x10120000U
#define GPIO_MODE_IT_RISING_FALLING 0x10130000U
#define GPIO_MODE_EVT_RISING    0x10140000U
#define GPIO_MODE_EVT_FALLING   0x10150000U

#define GPIO_NOPULL             0x00000000U
#define GPIO_PULLUP             0x00000001U
#define GPIO_PULLDOWN           0x00000002U

#define GPIO_SPEED_FREQ_LOW     0x00000000U
#define GPIO_SPEED_FREQ_MEDIUM   0x00000001U
#define GPIO_SPEED_FREQ_HIGH    0x00000002U
#define GPIO_SPEED_FREQ_VERY_HIGH 0x00000003U

/* GPIO alternate function numbers */
#define GPIO_AF0_RTC_50Hz      0U
#define GPIO_AF1_TIM1_2         1U
#define GPIO_AF2_TIM3_4_5       2U
#define GPIO_AF3_TIM8_9_10_11   3U
#define GPIO_AF4_I2C1_2_3       4U
#define GPIO_AF5_SPI1_2         5U
#define GPIO_AF6_SPI3           6U
#define GPIO_AF7_USART1_2_3     7U
#define GPIO_AF8_USART6         8U
#define GPIO_AF9_CAN1_2_TIM12_13_14  9U
#define GPIO_AF10_OTG_FS_HS     10U
#define GPIO_AF11_ETH           11U
#define GPIO_AF12_FSMC_SDIO     12U
#define GPIO_AF13_DCMI          13U
#define GPIO_AF14               14U
#define GPIO_AF15_EVENTOUT      15U

/* GPIO pin numbers */
#define GPIO_PIN_0  (0x0001U)
#define GPIO_PIN_1  (0x0002U)
#define GPIO_PIN_2  (0x0004U)
#define GPIO_PIN_3  (0x0008U)
#define GPIO_PIN_4  (0x0010U)
#define GPIO_PIN_5  (0x0020U)
#define GPIO_PIN_6  (0x0040U)
#define GPIO_PIN_7  (0x0080U)
#define GPIO_PIN_8  (0x0100U)
#define GPIO_PIN_9  (0x0200U)
#define GPIO_PIN_10 (0x0400U)
#define GPIO_PIN_11 (0x0800U)
#define GPIO_PIN_12 (0x1000U)
#define GPIO_PIN_13 (0x2000U)
#define GPIO_PIN_14 (0x4000U)
#define GPIO_PIN_15 (0x8000U)
#define GPIO_PIN_All (0xFFFFU)

/* GPIO Init structure definition */
typedef struct {
    uint32_t Pin;       /*!< GPIO pin number */
    uint32_t Mode;      /*!< Pin mode: GPIO_MODE_INPUT/OUTPUT/AF/ANALOG/IT/EVT */
    uint32_t Pull;      /*!< Pull: GPIO_NOPULL/PULLUP/PULLDOWN */
    uint32_t Speed;     /*!< Speed: GPIO_SPEED_FREQ_LOW/MEDIUM/HIGH/VERY_HIGH */
    uint32_t Alternate; /*!< Alternate function (AF0-15) */
} GPIO_InitTypeDef;

/* GPIO handle structure */
typedef struct {
    GPIO_TypeDef *Instance;
    GPIO_InitTypeDef Init;
    uint32_t Lock;
} GPIO_HandleTypeDef;

/* Exported functions --------------------------------------------------------*/
void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init);
void HAL_GPIO_DeInit(GPIO_TypeDef *GPIOx, uint32_t GPIO_Pin);
HAL_StatusTypeDef HAL_GPIO_LockPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, uint32_t PinState);
uint32_t HAL_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void HAL_GPIO_EXTI_IRQHandler(uint16_t GPIO_Pin);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_GPIO_H */