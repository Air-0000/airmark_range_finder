/**
 * @file stm32f4xx_hal_crc.h
 * @brief CRC HAL driver header
 */
#ifndef __STM32F4xx_HAL_CRC_H
#define __STM32F4xx_HAL_CRC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
#include "stm32f407xx.h"

/* CRC register bit definitions */
#define CRC_CR_RESET    0x00000001U

/* Exported types ------------------------------------------------------------*/
typedef struct {
    FunctionalState DefaultInitUse;
    uint32_t Init;
    uint32_t Polynomial;
} CRC_InitTypeDef;

typedef struct {
    uint32_t State;
    uint32_t ErrorCode;
    CRC_InitTypeDef *Init;
} CRC_HandleTypeDef;

/* HAL CRC state values */
#define HAL_CRC_STATE_RESET      0U
#define HAL_CRC_STATE_READY      1U
#define HAL_CRC_STATE_BUSY       2U
#define HAL_CRC_STATE_TIMEOUT    3U

/* Exported macros -----------------------------------------------------------*/
#define __HAL_CRC_UNLOCK(hcrc)         ((void)(hcrc))
#define __HAL_CRC_SET_INITIAL(hcrc)    ((void)(hcrc))
#define __HAL_CRC_SET_INITIAL_VALUE(hcrc, val)  ((void)(hcrc))

/* Exported functions --------------------------------------------------------*/
HAL_StatusTypeDef HAL_CRC_Init(CRC_HandleTypeDef *hcrc);
HAL_StatusTypeDef HAL_CRC_DeInit(CRC_HandleTypeDef *hcrc);
uint32_t HAL_CRC_Calc(CRC_HandleTypeDef *hcrc, uint32_t pBuffer[], uint32_t BufferLength);
uint32_t HAL_CRC_Calc_8bits(CRC_HandleTypeDef *hcrc, uint8_t pBuffer[], uint32_t BufferLength);
uint32_t HAL_CRC_Accumulate(CRC_HandleTypeDef *hcrc, uint32_t pBuffer[], uint32_t BufferLength);
uint32_t HAL_CRC_GetState(CRC_HandleTypeDef *hcrc);
uint32_t HAL_CRC_GetError(CRC_HandleTypeDef *hcrc);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_CRC_H */
