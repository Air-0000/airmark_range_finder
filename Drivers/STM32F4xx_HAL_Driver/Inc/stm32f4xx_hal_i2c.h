/**
 * @file stm32f4xx_hal_i2c.h
 * @brief I2C HAL driver
 */
#ifndef __STM32F4xx_HAL_I2C_H
#define __STM32F4xx_HAL_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
#include "stm32f407xx.h"

/* I2C Timing register values (100kHz Standard @ 42MHz APB1) */
#define I2C_TIMING_100KHZ  0x00303D5BU
#define I2C_TIMING_400KHZ  0x0010020BU

/* I2C memory address size */
#define I2C_MEMADD_SIZE_8BIT     0x00000001U
#define I2C_MEMADD_SIZE_16BIT    0x00000002U

/* I2C Init structure definition */
typedef struct {
    uint32_t ClockSpeed;         /*!< I2C clock speed in Hz */
    uint32_t DutyCycle;          /*!< Fast mode duty cycle (2:1 or 16:9) */
    uint32_t OwnAddress1;        /*!< Own address 1 (7-bit or 10-bit) */
    uint32_t AddressingMode;     /*!< Addressing mode: 7-bit or 10-bit */
    uint32_t DualAddressMode;    /*!< Dual addressing mode */
    uint32_t OwnAddress2;        /*!< Own address 2 (7-bit only) */
    uint32_t OwnAddress2Masks;   /*!< Own address 2 masks */
    uint32_t GeneralCallMode;    /*!< General call mode */
    uint32_t NoStretchMode;      /*!< No clock stretching mode */
} I2C_InitTypeDef;

/* I2C handle structure */
typedef struct {
    I2C_TypeDef *Instance;
    I2C_InitTypeDef Init;
    uint8_t *pBuffPtr;
    uint16_t XferSize;
    uint32_t XferCount;
    uint32_t XferOptions;
    uint32_t PreviousState;
    HAL_LockTypeDef Lock;
    uint32_t State;
    uint32_t ErrorCode;
} I2C_HandleTypeDef;

/* Exported functions */
HAL_StatusTypeDef HAL_I2C_Init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_DeInit(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef HAL_I2C_Master_Transmit(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Master_Receive(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Slave_Transmit(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Slave_Receive(I2C_HandleTypeDef *hi2c, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Mem_Write(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
HAL_StatusTypeDef HAL_I2C_Mem_Read(I2C_HandleTypeDef *hi2c, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
void HAL_I2C_EV_IRQHandler(I2C_HandleTypeDef *hi2c);
void HAL_I2C_ER_IRQHandler(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c);
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_I2C_H */