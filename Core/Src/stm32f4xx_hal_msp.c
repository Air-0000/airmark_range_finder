/**
 * @file stm32f4xx_hal_msp.c
 * @brief HAL MSP (MCU Specific Package) initialization
 */
#include "main.h"

/**
 * @brief Initialize MSP (MCU Specific Package)
 * @note  This function is called from HAL_Init()
 */
void HAL_MspInit(void)
{
    /* Configure GPIO pins for LED driver
     * LED0-PB0, LED1-PB1, LED2-PB2, LED3-PB3, LED4-PB4
     * Active low, push-pull output
     */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Configure GPIO pins for camera control
     * PC6 - DCMI_D0, PC7 - DCMI_D1, PC8 - DCMI_D2, PC9 - DCMI_D3
     * PC11 - DCMI_D4, PD3 - DCMI_D5, PD6 - DCMI_D6, PD10 - DCMI_D7
     * PB7 - SCL, PB8 - SDA (I2C)
     * PC10 - XI (clock), PA4 - VSYNC
     */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /* Configure DMA2 for camera data transfer */
    __HAL_RCC_DMA2_CLK_ENABLE();

    /* Configure DCMI clock */
    __HAL_RCC_DCMI_CLK_ENABLE();

    /* Configure timers for LED PWM and camera trigger */
    __HAL_RCC_TIM2_CLK_ENABLE();  /* LED PWM */
    __HAL_RCC_TIM3_CLK_ENABLE();  /* Camera trigger */
}

/**
 * @brief Deinitialize MSP
 * @note  This function is called from HAL_DeInit()
 */
void HAL_MspDeInit(void)
{
    /* Reset GPIO pins to default state */
    /* Note: In production, would reset all used peripherals */
}

/**
 * @brief DCMI MSP initialization
 * @param hdcmi Pointer to DCMI handle
 */
void HAL_DCMI_MspInit(DCMI_HandleTypeDef *hdcmi)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIO clocks */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Configure DCMI data pins (D0-D7) */
    GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 |
                         GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* Configure remaining DCMI data pins */
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_6 | GPIO_PIN_10;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* Configure DCMI control pins */
    /* VSYNC on PA4, HREF on PA6, PIXCLK on PC10 */
    GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_6;
    GPIO_InitStruct.Alternate = GPIO_AF13_DCMI;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* Configure DMA2 Stream 1 for DCMI */
    /* Note: DMA configuration would be completed in camera_driver.c */
}

/**
 * @brief Timer MSP initialization
 * @param htim Pointer to TIM handle
 */
void HAL_TIM_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2) {
        /* TIM2 for LED PWM */
        __HAL_RCC_TIM2_CLK_ENABLE();
    } else if (htim->Instance == TIM3) {
        /* TIM3 for camera trigger */
        __HAL_RCC_TIM3_CLK_ENABLE();
    }
}