/**
 * @file stm32f4xx_hal_dcmi.c
 * @brief DCMI HAL driver source
 */
#include "stm32f4xx_hal_dcmi.h"

/**
 * @brief Initialize DCMI
 */
HAL_StatusTypeDef HAL_DCMI_Init(DCMI_HandleTypeDef *hdcmi)
{
    (void)hdcmi;
    return HAL_OK;
}

/**
 * @brief Deinitialize DCMI
 */
HAL_StatusTypeDef HAL_DCMI_DeInit(DCMI_HandleTypeDef *hdcmi)
{
    (void)hdcmi;
    return HAL_OK;
}

/**
 * @brief Start DMA capture
 */
HAL_StatusTypeDef HAL_DCMI_Start_DMA(DCMI_HandleTypeDef *hdcmi, uint32_t DstAddress,
                                      uint32_t BufferSize)
{
    (void)hdcmi;
    (void)DstAddress;
    (void)BufferSize;
    return HAL_OK;
}

/**
 * @brief Stop DCMI capture
 */
HAL_StatusTypeDef HAL_DCMI_Stop(DCMI_HandleTypeDef *hdcmi)
{
    (void)hdcmi;
    return HAL_OK;
}

/**
 * @brief Suspend DCMI
 */
HAL_StatusTypeDef HAL_DCMI_Suspend(DCMI_HandleTypeDef *hdcmi)
{
    (void)hdcmi;
    return HAL_OK;
}

/**
 * @brief Resume DCMI
 */
HAL_StatusTypeDef HAL_DCMI_Resume(DCMI_HandleTypeDef *hdcmi)
{
    (void)hdcmi;
    return HAL_OK;
}

/**
 * @brief Grab frame
 */
HAL_StatusTypeDef HAL_DCMI_Grab(DCMI_HandleTypeDef *hdcmi, uint8_t *pData, uint32_t Timeout)
{
    (void)hdcmi;
    (void)pData;
    (void)Timeout;
    return HAL_OK;
}

/**
 * @brief DCMI IRQ handler
 */
void HAL_DCMI_IRQHandler(DCMI_HandleTypeDef *hdcmi)
{
    (void)hdcmi;
}

/**
 * @brief DCMI error callback
 */
void __attribute__((weak)) HAL_DCMI_ErrorCallback(DCMI_HandleTypeDef *hdcmi)
{
    (void)hdcmi;
}

/**
 * @brief DCMI frame callback
 */
void __attribute__((weak)) HAL_DCMI_FrameCallback(DCMI_HandleTypeDef *hdcmi)
{
    (void)hdcmi;
}

/**
 * @brief DCMI vsync callback
 */
void __attribute__((weak)) HAL_DCMI_VsyncCallback(DCMI_HandleTypeDef *hdcmi)
{
    (void)hdcmi;
}

/**
 * @brief DCMI line callback
 */
void __attribute__((weak)) HAL_DCMI_LineCallback(DCMI_HandleTypeDef *hdcmi)
{
    (void)hdcmi;
}