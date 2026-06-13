/**
 * @file stm32f4xx_hal_dma.c
 * @brief DMA HAL driver source
 */
#include "stm32f4xx_hal_dma.h"

/* Private variables ---------------------------------------------------------*/
static DMA_HandleTypeDef dma_handle_instance;

/**
 * @brief Initialize DMA
 */
HAL_StatusTypeDef HAL_DMA_Init(DMA_HandleTypeDef *hdma)
{
    (void)hdma;
    return HAL_OK;
}

/**
 * @brief Deinitialize DMA
 */
HAL_StatusTypeDef HAL_DMA_DeInit(DMA_HandleTypeDef *hdma)
{
    (void)hdma;
    return HAL_OK;
}

/**
 * @brief Start DMA
 */
HAL_StatusTypeDef HAL_DMA_Start(DMA_HandleTypeDef *hdma, uint32_t SrcAddress,
                                 uint32_t DstAddress, uint32_t DataLength)
{
    (void)hdma;
    (void)SrcAddress;
    (void)DstAddress;
    (void)DataLength;
    return HAL_OK;
}

/**
 * @brief Start DMA with interrupt
 */
HAL_StatusTypeDef HAL_DMA_Start_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress,
                                    uint32_t DstAddress, uint32_t DataLength)
{
    (void)hdma;
    (void)SrcAddress;
    (void)DstAddress;
    (void)DataLength;
    return HAL_OK;
}

/**
 * @brief Abort DMA
 */
HAL_StatusTypeDef HAL_DMA_Abort(DMA_HandleTypeDef *hdma)
{
    (void)hdma;
    return HAL_OK;
}

/**
 * @brief Abort DMA with interrupt
 */
HAL_StatusTypeDef HAL_DMA_Abort_IT(DMA_HandleTypeDef *hdma)
{
    (void)hdma;
    return HAL_OK;
}

/**
 * @brief DMA IRQ handler
 */
void HAL_DMA_IRQHandler(DMA_HandleTypeDef *hdma)
{
    (void)hdma;
}

/**
 * @brief Poll for transfer complete
 */
HAL_StatusTypeDef HAL_DMA_PollForTransfer(DMA_HandleTypeDef *hdma,
                                           uint32_t CompleteLevel, uint32_t Timeout)
{
    (void)hdma;
    (void)CompleteLevel;
    (void)Timeout;
    return HAL_OK;
}

/**
 * @brief Register DMA callback
 */
void HAL_DMA_RegisterCallback(DMA_HandleTypeDef *hdma, uint32_t CallbackID,
                               void (*pCallback)(DMA_HandleTypeDef *_hdma))
{
    (void)hdma;
    (void)CallbackID;
    (void)pCallback;
}

/**
 * @brief Unregister DMA callback
 */
void HAL_DMA_UnRegisterCallback(DMA_HandleTypeDef *hdma, uint32_t CallbackID)
{
    (void)hdma;
    (void)CallbackID;
}