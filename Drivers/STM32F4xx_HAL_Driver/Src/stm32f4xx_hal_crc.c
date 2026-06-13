/**
 * @file stm32f4xx_hal_crc.c
 * @brief CRC HAL driver
 */
#include "stm32f4xx_hal.h"

/**
 * @brief  Initialize the CRC peripheral
 * @param  hcrc: pointer to a CRC_HandleTypeDef structure
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_CRC_Init(CRC_HandleTypeDef *hcrc)
{
    /* Check the parameters */
    if (hcrc == NULL) {
        return HAL_ERROR;
    }

    /* Unlock CRC */
    __HAL_CRC_UNLOCK(hcrc);

    /* Reset CRC */
    CRC->DR = 0xFFFFFFFFU;

    /* Configure CRC */
    hcrc->State = HAL_CRC_STATE_BUSY;

    (void)hcrc;  /* Suppress unused warning */

    hcrc->State = HAL_CRC_STATE_READY;

    return HAL_OK;
}

/**
 * @brief  DeInitialize the CRC peripheral
 * @param  hcrc: pointer to a CRC_HandleTypeDef structure
 * @retval HAL status
 */
HAL_StatusTypeDef HAL_CRC_DeInit(CRC_HandleTypeDef *hcrc)
{
    if (hcrc == NULL) {
        return HAL_ERROR;
    }

    /* Disable CRC */
    CRC->CR = 0U;

    hcrc->State = HAL_CRC_STATE_READY;

    return HAL_OK;
}

/**
 * @brief  Compute the CRC of 32-bit data
 * @param  hcrc: pointer to a CRC_HandleTypeDef structure
 * @param  pBuffer: pointer to the data buffer
 * @param  BufferLength: length of the buffer (in 32-bit words)
 * @retval uint32_t: computed CRC value
 */
uint32_t HAL_CRC_Calc(CRC_HandleTypeDef *hcrc, uint32_t pBuffer[], uint32_t BufferLength)
{
    uint32_t index;
    uint32_t crc = 0U;

    if (hcrc == NULL) {
        return 0xFFFFFFFFU;
    }

    /* Setup input data */
    hcrc->State = HAL_CRC_STATE_BUSY;

    /* Reset CRC generator */
    CRC->CR |= CRC_CR_RESET;

    /* Compute CRC */
    for (index = 0U; index < BufferLength; index++) {
        CRC->DR = pBuffer[index];
    }

    /* Get computed CRC value */
    crc = CRC->DR;

    hcrc->State = HAL_CRC_STATE_READY;

    return crc;
}

/**
 * @brief  Compute the CRC of 8-bit data
 * @param  hcrc: pointer to a CRC_HandleTypeDef structure
 * @param  pBuffer: pointer to the data buffer
 * @param  BufferLength: length of the buffer (in bytes)
 * @retval uint32_t: computed CRC value
 */
uint32_t HAL_CRC_Calc_8bits(CRC_HandleTypeDef *hcrc, uint8_t pBuffer[], uint32_t BufferLength)
{
    uint32_t index;
    uint32_t crc = 0U;

    if (hcrc == NULL) {
        return 0xFFFFFFFFU;
    }

    /* Set up input data */
    hcrc->State = HAL_CRC_STATE_BUSY;

    /* Reset CRC generator */
    CRC->CR |= CRC_CR_RESET;

    /* Compute CRC byte by byte */
    for (index = 0U; index < BufferLength; index++) {
        CRC->DR = (uint32_t)pBuffer[index];
    }

    /* Get computed CRC value */
    crc = CRC->DR;

    hcrc->State = HAL_CRC_STATE_READY;

    return crc;
}

/**
 * @brief  Accumulate CRC of 32-bit data (multiple calls)
 * @param  hcrc: pointer to a CRC_HandleTypeDef structure
 * @param  pBuffer: pointer to the data buffer
 * @param  BufferLength: length of the buffer (in 32-bit words)
 * @retval uint32_t: computed CRC value
 */
uint32_t HAL_CRC_Accumulate(CRC_HandleTypeDef *hcrc, uint32_t pBuffer[], uint32_t BufferLength)
{
    uint32_t index;
    uint32_t crc = 0U;

    if (hcrc == NULL) {
        return 0xFFFFFFFFU;
    }

    /* Compute CRC */
    for (index = 0U; index < BufferLength; index++) {
        CRC->DR = pBuffer[index];
    }

    /* Get computed CRC value */
    crc = CRC->DR;

    return crc;
}

/**
 * @brief  Get the current CRC value
 * @param  hcrc: pointer to a CRC_HandleTypeDef structure
 * @retval uint32_t: current CRC value
 */
uint32_t HAL_CRC_GetState(CRC_HandleTypeDef *hcrc)
{
    return CRC->DR;
}

/**
 * @brief  Get the CRC error code
 * @param  hcrc: pointer to a CRC_HandleTypeDef structure
 * @retval uint32_t: CRC error code
 */
uint32_t HAL_CRC_GetError(CRC_HandleTypeDef *hcrc)
{
    return hcrc->ErrorCode;
}
