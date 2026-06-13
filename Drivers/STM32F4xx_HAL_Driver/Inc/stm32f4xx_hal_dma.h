/**
 * @file stm32f4xx_hal_dma.h
 * @brief DMA HAL driver
 */
#ifndef __STM32F4xx_HAL_DMA_H
#define __STM32F4xx_HAL_DMA_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
#include "stm32f407xx.h"

/* DMA channel selection masks */
#define DMA_SxCR_CHSEL_0         0x00000001U
#define DMA_SxCR_CHSEL_1         0x00000002U
#define DMA_SxCR_CHSEL_2         0x00000004U
#define DMA_SxCR_CHSEL_3         0x00000008U

/* DMA Init structure definition */
typedef struct {
    uint32_t Channel;        /*!< DMA channel selection (0-7) */
    uint32_t Direction;      /*!< Memory-peripheral/peripheral-memory transfer */
    uint32_t PeriphInc;      /*!< Peripheral increment mode */
    uint32_t MemInc;         /*!< Memory increment mode */
    uint32_t PeriphDataAlignment; /*!< Peripheral data size */
    uint32_t MemDataAlignment;    /*!< Memory data size */
    uint32_t Mode;           /*!< Normal/Circular/Flow control mode */
    uint32_t Priority;       /*!< Priority level */
    uint32_t FIFOMode;       /*!< FIFO mode enable/disable */
    uint32_t FIFOThreshold;  /*!< FIFO threshold selection */
    uint32_t MemBurst;       /*!< Memory burst transfer configuration */
    uint32_t PeriphBurst;    /*!< Peripheral burst transfer configuration */
} DMA_InitTypeDef;

/* DMA handle structure */
typedef struct {
    DMA_Stream_TypeDef *Instance;
    DMA_InitTypeDef Init;
    HAL_LockTypeDef Lock;
    uint32_t State;
    void *Parent;
} DMA_HandleTypeDef;

/* DMA link to handle macro */
#define __HAL_LINKDMA(__HANDLE__, __PPP_DMA_FIELD__, __DMA_HANDLE__) \
    do { \
        (__HANDLE__)->__PPP_DMA_FIELD__ = &(__DMA_HANDLE__); \
        (__DMA_HANDLE__).Parent = (__HANDLE__); \
    } while(0U)

/* DMA stream/channel defines */
#define DMA_CHANNEL_0       0x00000000U
#define DMA_CHANNEL_1       0x00000001U
#define DMA_CHANNEL_2       0x00000002U
#define DMA_CHANNEL_3       0x00000003U
#define DMA_CHANNEL_4       0x00000004U
#define DMA_CHANNEL_5       0x00000005U
#define DMA_CHANNEL_6       0x00000006U
#define DMA_CHANNEL_7       0x00000007U

/* Exported functions */
HAL_StatusTypeDef HAL_DMA_Init(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_DeInit(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_Start(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
HAL_StatusTypeDef HAL_DMA_Start_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
HAL_StatusTypeDef HAL_DMA_Abort(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_Abort_IT(DMA_HandleTypeDef *hdma);
void HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma);
HAL_StatusTypeDef HAL_DMA_PollForTransfer(DMA_HandleTypeDef *hdma, uint32_t CompleteLevel, uint32_t Timeout);
void HAL_DMA_RegisterCallback(DMA_HandleTypeDef *hdma, uint32_t CallbackID, void (*pCallback)(DMA_HandleTypeDef *_hdma));
void HAL_DMA_UnRegisterCallback(DMA_HandleTypeDef *hdma, uint32_t CallbackID);

#define __HAL_DMA_GET_FLAG(__HANDLE__, __FLAG__) \
    (((__HANDLE__)->Instance->CR & (__FLAG__)) != 0)

#define __HAL_DMA_CLEAR_FLAG(__HANDLE__, __FLAG__) \
    ((__HANDLE__)->Instance->CR &= ~(__FLAG__))

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_DMA_H */