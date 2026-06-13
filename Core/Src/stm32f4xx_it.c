/**
 * @file stm32f4xx_it.c
 * @brief Interrupt handling implementation
 */
#include "main.h"
#include "camera_driver.h"
#include "led_driver.h"

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_dcmi;
extern DCMI_HandleTypeDef hdcmi;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

/**
 * @brief This function handles Non maskable interrupt
 */
void NMI_Handler(void)
{
    while (1) {
    }
}

/**
 * @brief This function handles Hard fault interrupt
 */
void HardFault_Handler(void)
{
    while (1) {
    }
}

/**
 * @brief This function handles Memory management fault
 */
void MemManage_Handler(void)
{
    while (1) {
    }
}

/**
 * @brief This function handles Pre-fetch fault, memory access fault
 */
void BusFault_Handler(void)
{
    while (1) {
    }
}

/**
 * @brief This function handles Undefined instruction or illegal state
 */
void UsageFault_Handler(void)
{
    while (1) {
    }
}

/**
 * @brief This function handles System service call via SWI instruction
 */
void SVC_Handler(void)
{
}

/**
 * @brief This function handles Debug monitor
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief This function handles Pend pending request
 */
void PendSV_Handler(void)
{
}

/**
 * @brief This function handles System tick timer
 */
void SysTick_Handler(void)
{
    HAL_IncTick();
}

/**
 * @brief This function handles DCMI interrupt
 */
void DCMI_IRQHandler(void)
{
    HAL_DCMI_IRQHandler(&hdcmi);
}

/**
 * @brief This function handles DMA2 stream 1 interrupt (DCMI)
 */
void DMA2_Stream1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_dcmi);
}

/**
 * @brief This function handles TIM2 interrupt (LED PWM)
 */
void TIM2_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim2);
}

/**
 * @brief This function handles TIM3 interrupt (camera trigger)
 */
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim3);
}