/**
 * @file stm32f4xx_hal_tim.h
 * @brief TIM HAL driver
 */
#ifndef __STM32F4xx_HAL_TIM_H
#define __STM32F4xx_HAL_TIM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
#include "stm32f407xx.h"

/* TIM Init structure definition */
typedef struct {
    uint32_t Prescaler;       /*!< Prescaler value */
    uint32_t CounterMode;     /*!< Counter mode: Up/Down/CenterAligned */
    uint32_t Period;          /*!< Auto-reload value (ARR) */
    uint32_t ClockDivision;  /*!< Clock division (1/2/4) */
    uint32_t AutoReloadPreload; /*!< Auto-reload preload enable */
} TIM_InitTypeDef;

/* TIM handle structure */
typedef struct {
    TIM_TypeDef *Instance;
    TIM_InitTypeDef Init;
    uint32_t State;
} TIM_HandleTypeDef;

/* TIM channel defines */
#define TIM_CHANNEL_1         0x00000001U
#define TIM_CHANNEL_2         0x00000002U
#define TIM_CHANNEL_3         0x00000004U
#define TIM_CHANNEL_4         0x00000008U
#define TIM_CHANNEL_ALL       0x0000000FU

/* TIM clock division */
#define TIM_CLOCKDIVISION_DIV1  0x00000000U
#define TIM_CLOCKDIVISION_DIV2  0x00000100U
#define TIM_CLOCKDIVISION_DIV4  0x00000200U

/* TIM output compare mode */
#define TIM_OCMODE_TIMING       0x00000000U
#define TIM_OCMODE_ACTIVE       0x00000001U
#define TIM_OCMODE_INACTIVE      0x00000002U
#define TIM_OCMODE_TOGGLE        0x00000003U
#define TIM_OCMODE_PWM1          0x00000006U
#define TIM_OCMODE_PWM2          0x00000007U

/* TIM output polarity */
#define TIM_OCPOLARITY_HIGH      0x00000000U
#define TIM_OCPOLARITY_LOW        0x00000002U

/* TIM output fast enable */
#define TIM_OCFAST_DISABLE       0x00000000U
#define TIM_OCFAST_ENABLE        0x00000400U

/* TIM PWM output set/compare macros */
#define __HAL_TIM_SET_PRESCALER(__HANDLE__, __PRESC__) ((__HANDLE__)->Instance->PSC = (__PRESC__))
#define __HAL_TIM_SET_COMPARE(__HANDLE__, __CHANNEL__, __COMPARE__) \
    (((__CHANNEL__) == TIM_CHANNEL_1) ? ((__HANDLE__)->Instance->CCR[0] = (__COMPARE__)) : \
     ((__CHANNEL__) == TIM_CHANNEL_2) ? ((__HANDLE__)->Instance->CCR[1] = (__COMPARE__)) : \
     ((__CHANNEL__) == TIM_CHANNEL_3) ? ((__HANDLE__)->Instance->CCR[2] = (__COMPARE__)) : \
                                        ((__HANDLE__)->Instance->CCR[3] = (__COMPARE__)))

/* TIM OC init structure */
typedef struct {
    uint32_t OCMode;      /*!< TIM output compare mode */
    uint32_t Pulse;        /*!< PWM pulse width */
    uint32_t OCPolarity;  /*!< Output polarity */
    uint32_t OCFastMode;  /*!< Output compare fast mode */
} TIM_OC_InitTypeDef;

typedef enum {
    HAL_TIM_ACTIVE_CHANNEL_1 = 0x00U,
    HAL_TIM_ACTIVE_CHANNEL_2 = 0x01U,
    HAL_TIM_ACTIVE_CHANNEL_3 = 0x02U,
    HAL_TIM_ACTIVE_CHANNEL_4 = 0x03U,
    HAL_TIM_ACTIVE_CHANNEL_CLEARED = 0x04U
} HAL_TIM_ActiveChannel;

/* Exported functions */
HAL_StatusTypeDef HAL_TIM_Init(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_DeInit(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Init(TIM_InitTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_DeInit(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Start(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Stop(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Start_IT(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_Base_Stop_IT(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim);
HAL_StatusTypeDef HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim, void *sConfig, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
HAL_StatusTypeDef HAL_TIM_PWM_Stop(TIM_HandleTypeDef *htim, uint32_t Channel);
void HAL_TIM_IRQHandler(TIM_HandleTypeDef *htim);
uint32_t HAL_TIM_Base_GetState(TIM_HandleTypeDef *htim);

#define __HAL_TIM_SET_COUNTER(__HANDLE__, __COUNTER__) ((__HANDLE__)->Instance->CNT = (__COUNTER__))
#define __HAL_TIM_GET_COUNTER(__HANDLE__) ((__HANDLE__)->Instance->CNT)
#define __HAL_TIM_SET_AUTORELOAD(__HANDLE__, __AUTORELOAD__) ((__HANDLE__)->Instance->ARR = (__AUTORELOAD__))
#define __HAL_TIM_GET_AUTORELOAD(__HANDLE__) ((__HANDLE__)->Instance->ARR)

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_TIM_H */