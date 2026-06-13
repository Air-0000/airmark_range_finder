/**
 * @file stm32f4xx_hal_i2c.c
 * @brief I2C HAL driver source
 */
#include "stm32f4xx_hal_i2c.h"

/* Private variables ---------------------------------------------------------*/
static I2C_HandleTypeDef i2c_handle_instance;

/**
 * @brief Initialize I2C
 */
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c)
{
    (void)hi2c;
    return HAL_OK;
}

/**
 * @brief Deinitialize I2C
 */
HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c)
{
    (void)hi2c;
    return HAL_OK;
}

/**
 * @brief I2C Msp Init callback
 */
void __attribute__((weak)) HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
    (void)hi2c;
}

/**
 * @brief I2C Msp DeInit callback
 */
void __attribute__((weak)) HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
    (void)hi2c;
}

/**
 * @brief Master transmit
 */
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
                                           uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    (void)hi2c;
    (void)DevAddress;
    (void)pData;
    (void)Size;
    (void)Timeout;
    return HAL_OK;
}

/**
 * @brief Master receive
 */
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
                                          uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    (void)hi2c;
    (void)DevAddress;
    (void)pData;
    (void)Size;
    (void)Timeout;
    return HAL_OK;
}

/**
 * @brief Slave transmit
 */
HAL_StatusTypeDef HAL_I2C_Slave_Transmit(I2C_HandleTypeDef *hi2c, uint8_t *pData,
                                          uint16_t Size, uint32_t Timeout)
{
    (void)hi2c;
    (void)pData;
    (void)Size;
    (void)Timeout;
    return HAL_OK;
}

/**
 * @brief Slave receive
 */
HAL_StatusTypeDef HAL_I2C_Slave_Receive(I2C_HandleTypeDef *hi2c, uint8_t *pData,
                                         uint16_t Size, uint32_t Timeout)
{
    (void)hi2c;
    (void)pData;
    (void)Size;
    (void)Timeout;
    return HAL_OK;
}

/**
 * @brief Memory write
 */
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
                                     uint16_t MemAddress, uint16_t MemAddSize,
                                     uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    (void)hi2c;
    (void)DevAddress;
    (void)MemAddress;
    (void)MemAddSize;
    (void)pData;
    (void)Size;
    (void)Timeout;
    return HAL_OK;
}

/**
 * @brief Memory read
 */
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
                                    uint16_t MemAddress, uint16_t MemAddSize,
                                    uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    (void)hi2c;
    (void)DevAddress;
    (void)MemAddress;
    (void)MemAddSize;
    (void)pData;
    (void)Size;
    (void)Timeout;
    return HAL_OK;
}

/**
 * @brief I2C EV IRQ handler
 */
void HAL_I2C_EV_IRQHandler(I2C_HandleTypeDef *hi2c)
{
    (void)hi2c;
}

/**
 * @brief I2C ER IRQ handler
 */
void HAL_I2C_ER_IRQHandler(I2C_HandleTypeDef *hi2c)
{
    (void)hi2c;
}

/**
 * @brief Master Tx complete callback
 */
void __attribute__((weak)) HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    (void)hi2c;
}

/**
 * @brief Master Rx complete callback
 */
void __attribute__((weak)) HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    (void)hi2c;
}

/**
 * @brief Slave Tx complete callback
 */
void __attribute__((weak)) HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    (void)hi2c;
}

/**
 * @brief Slave Rx complete callback
 */
void __attribute__((weak)) HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    (void)hi2c;
}

/**
 * @brief Error callback
 */
void __attribute__((weak)) HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
    (void)hi2c;
}