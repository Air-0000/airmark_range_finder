/**
 * @file stm32f4xx_hal_dcmi.h
 * @brief DCMI HAL driver
 */
#ifndef __STM32F4xx_HAL_DCMI_H
#define __STM32F4xx_HAL_DCMI_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
#include "stm32f407xx.h"

/* DCMI capture mode */
#define DCMI_MODE_SNAPSHOT      0x00000000U
#define DCMI_MODE_CONTINUOUS    0x00000001U

/* DCMI polarity */
#define DCMI_Polarity_AHB       0x00000000U
#define DCMI_Polarity_Falling   0x00020000U

#define DCMI_CaptureRate_1FRAME   0x00000000U
#define DCMI_CaptureRate_2FRAME   0x00010000U
#define DCMI_CaptureRate_4FRAME   0x00030000U

#define DCMI_VSYNC_Edge          0x00000000U
#define DCMI_VSYNC_ActiveHigh    0x00080000U

#define DCMI_HSYNC_InActiveLow   0x00000000U
#define DCMI_HSYNC_InActiveHigh  0x00040000U

/* DCMI Init structure definition */
typedef struct {
    uint32_t SynchroMode;    /*!< Synchronization mode */
    uint32_t JPEG;           /*!< JPEG mode */
    uint32_t CaptureRate;    /*!< Capture rate */
    uint32_t VSPolarity;    /*!< VSYNC polarity */
    uint32_t HSPolarity;    /*!< HSYNC polarity */
    uint32_t PCKPolarity;   /*!< Pixel clock polarity */
    uint32_t ExtendedDataMode; /*!< 8-bit or 16-bit data */
    uint32_t ByteSelectMode;  /*!< Byte select mode */
    uint32_t ByteSelectStart; /*!< Byte select start */
    uint32_t LineSelectMode;  /*!< Line select mode */
} DCMI_InitTypeDef;

/* DCMI handle structure */
typedef struct DCMI_HandleTypeDef_s {
    DCMI_TypeDef *Instance;
    DCMI_InitTypeDef Init;
    HAL_LockTypeDef Lock;
    uint32_t State;
    uint32_t XferSize;
    uint32_t XferCount;
    uint32_t ErrorCode;
    void *DMA_Handle;
    void (*FrameCallback)(struct DCMI_HandleTypeDef_s *hdcmi);
    void (*VsyncCallback)(struct DCMI_HandleTypeDef_s *hdcmi);
    void (*LineCallback)(struct DCMI_HandleTypeDef_s *hdcmi);
    void (*ErrorCallback)(struct DCMI_HandleTypeDef_s *hdcmi);
} DCMI_HandleTypeDef;

/* Exported functions */
HAL_StatusTypeDef HAL_DCMI_Init(DCMI_HandleTypeDef *hdcmi);
HAL_StatusTypeDef HAL_DCMI_DeInit(DCMI_HandleTypeDef *hdcmi);
HAL_StatusTypeDef HAL_DCMI_Start_DMA(DCMI_HandleTypeDef *hdcmi, uint32_t DstAddress, uint32_t BufferSize);
HAL_StatusTypeDef HAL_DCMI_Stop(DCMI_HandleTypeDef *hdcmi);
HAL_StatusTypeDef HAL_DCMI_Suspend(DCMI_HandleTypeDef *hdcmi);
HAL_StatusTypeDef HAL_DCMI_Resume(DCMI_HandleTypeDef *hdcmi);
HAL_StatusTypeDef HAL_DCMI_Grab(DCMI_HandleTypeDef *hdcmi, uint8_t *pData, uint32_t Timeout);
void HAL_DCMI_IRQHandler(DCMI_HandleTypeDef *hdcmi);
void HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi);
void HAL_DCMI_FrameCallback(DCMI_HandleTypeDef *hdcmi);
void HAL_DCMI_VsyncCallback(DCMI_HandleTypeDef *hdcmi);
void HAL_DCMI_LineCallback(DCMI_HandleTypeDef *hdcmi);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_DCMI_H */