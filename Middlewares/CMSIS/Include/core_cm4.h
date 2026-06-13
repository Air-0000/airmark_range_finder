/**
 * @file core_cm4.h
 * @brief CMSIS Cortex-M4 Core Peripheral Access Layer Header
 */
#ifndef __CMSIS_CM4_H
#define __CMSIS_CM4_H

#ifdef __cplusplus
extern "C" {
#endif

/* Processor and Core Peripheral definitions */
#define __CM4_REV              0x0001U
#define __MPU_PRESENT          1U
#define __VTOR_PRESENT         1U
#define __NVIC_PRIO_BITS       4U
#define __Vendor_SysTickConfig 0U
#define __FPU_PRESENT          1U

/* Includes */
#include <stdint.h>

/* Legacy defines - avoid redefinition if already set by stm32f407xx.h */
#ifndef __I
#define __I  volatile const
#endif
#ifndef __O
#define __O  volatile
#endif
#ifndef __IO
#define __IO volatile
#endif

/* IRQ Numbers - use IRQn_Type from stm32f407xx.h if available */
#ifndef IRQn_Type
typedef enum {
    NonMaskableInt_IRQn = -14,
    MemoryManagement_IRQn = -12,
    BusFault_IRQn = -11,
    UsageFault_IRQn = -10,
    SVCall_IRQn = -5,
    DebugMonitor_IRQn = -4,
    PendSV_IRQn = -2,
    SysTick_IRQn = -1,
    WWDG_IRQn = 0,
    PVD_IRQn = 1,
    TAMPER_STAMP_IRQn = 2,
    RTC_WKUP_IRQn = 3,
    FLASH_IRQn = 4,
    RCC_IRQn = 5,
    EXTI0_IRQn = 6,
    EXTI1_IRQn = 7,
    EXTI2_IRQn = 8,
    EXTI3_IRQn = 9,
    EXTI4_IRQn = 10,
    DMA1_Stream0_IRQn = 11,
    DMA1_Stream1_IRQn = 12,
    DMA1_Stream2_IRQn = 13,
    DMA1_Stream3_IRQn = 14,
    DMA1_Stream4_IRQn = 15,
    DMA1_Stream5_IRQn = 16,
    DMA1_Stream6_IRQn = 17,
    ADC_IRQn = 18,
    CAN1_TX_IRQn = 19,
    CAN1_RX0_IRQn = 20,
    CAN1_RX1_IRQn = 21,
    CAN1_SCE_IRQn = 22,
    EXTI9_5_IRQn = 23,
    TIM1_BRK_TIM9_IRQn = 24,
    TIM1_UP_TIM10_IRQn = 25,
    TIM1_TRG_COM_TIM11_IRQn = 26,
    TIM1_CC_IRQn = 27,
    TIM2_IRQn = 28,
    TIM3_IRQn = 29,
    TIM4_IRQn = 30,
    I2C1_EV_IRQn = 31,
    I2C1_ER_IRQn = 32,
    I2C2_EV_IRQn = 33,
    I2C2_ER_IRQn = 34,
    SPI1_IRQn = 35,
    SPI2_IRQn = 36,
    USART1_IRQn = 37,
    USART2_IRQn = 38,
    USART3_IRQn = 39,
    EXTI15_10_IRQn = 40,
    RTC_Alarm_IRQn = 41,
    OTG_FS_WKUP_IRQn = 42,
    TIM8_BRK_TIM12_IRQn = 43,
    TIM8_UP_TIM13_IRQn = 44,
    TIM8_TRG_COM_TIM14_IRQn = 45,
    TIM8_CC_IRQn = 46,
    DMA1_Stream7_IRQn = 47,
    FSMC_IRQn = 48,
    SDIO_IRQn = 49,
    TIM5_IRQn = 50,
    SPI3_IRQn = 51,
    UART4_IRQn = 52,
    UART5_IRQn = 53,
    TIM6_DAC_IRQn = 54,
    TIM7_IRQn = 55,
    DMA2_Stream0_IRQn = 56,
    DMA2_Stream1_IRQn = 57,
    DMA2_Stream2_IRQn = 58,
    DMA2_Stream3_IRQn = 59,
    DMA2_Stream4_IRQn = 60,
    CAN2_TX_IRQn = 63,
    CAN2_RX0_IRQn = 64,
    CAN2_RX1_IRQn = 65,
    CAN2_SCE_IRQn = 66,
    OTG_FS_IRQn = 67,
    DMA2_Stream5_IRQn = 68,
    DMA2_Stream6_IRQn = 69,
    DMA2_Stream7_IRQn = 70,
    USART6_IRQn = 71,
    I2C3_EV_IRQn = 72,
    I2C3_ER_IRQn = 73,
    OTG_HS_EP1_OUT_IRQn = 74,
    OTG_HS_EP1_IN_IRQn = 75,
    OTG_HS_WKUP_IRQn = 76,
    OTG_HS_IRQn = 77,
    DCMI_IRQn = 78,
    CRYP_IRQn = 79,
    HASH_RNG_IRQn = 80
} IRQn_TypeDef;

/* SCB registers */
typedef struct {
    __I uint32_t CPUID;
    __IO uint32_t ICSR;
    __IO uint32_t VTOR;
    __IO uint32_t AIRCR;
    __IO uint32_t SCR;
    __IO uint32_t CCR;
    __IO uint8_t SHP[12];
    __IO uint32_t SHCSR;
    __IO uint32_t CFSR;
    __IO uint32_t HFSR;
    __IO uint32_t DFSR;
    __IO uint32_t MMFAR;
    __IO uint32_t BFAR;
    __IO uint32_t AFSR;
    __I uint32_t PFR[2];
    __I uint32_t DFR;
    __I uint32_t ADR;
    __I uint32_t MMFR[4];
    __I uint32_t ISAR[5];
    uint32_t RESERVED0[5];
    __IO uint32_t CPACR;
} SCB_TypeDef;

#define SCB ((SCB_TypeDef *) 0xE000ED00)

/* SysTick registers */
typedef struct {
    __IO uint32_t CTRL;
    __IO uint32_t LOAD;
    __IO uint32_t VAL;
    __I uint32_t CALIB;
} SysTick_TypeDef;

#define SysTick ((SysTick_TypeDef *) 0xE000E010)

/* NVIC registers */
typedef struct {
    __IO uint32_t ISER[8];
    uint32_t RESERVED0[24];
    __IO uint32_t ICER[8];
    uint32_t RESERVED1[24];
    __IO uint32_t ISPR[8];
    uint32_t RESERVED2[24];
    __IO uint32_t ICPR[8];
    uint32_t RESERVED3[24];
    __IO uint32_t IABR[8];
    uint32_t RESERVED4[56];
    __IO uint8_t IP[240];
    uint32_t RESERVED5[644];
    __O uint32_t STIR;
} NVIC_TypeDef;

#define NVIC ((NVIC_TypeDef *) 0xE000E100)

/* Interrupt enable macros */
#define __enable_irq()         __asm volatile ("cpsie i")
#define __disable_irq()        __asm volatile ("cpsid i")

/* Priority masking */
#define __get_xPSR()           __asm volatile ("mrs %0, psr" : "=r" ())
#define __set_PRIMASK(primask) __asm volatile ("msr primask, %0" : : "r" (primask))
#define __get_PRIMASK()        ({ uint32_t primask; __asm volatile ("mrs %0, primask" : "=r" (primask)); primask; })

/* SCB macros */
#define SCB_ACTLR_DEFAULT       0x00000000U
#define SCS_BASE                0xE000E000
#define SysTick_BASE            (SCS_BASE + 0x0010)
#define NVIC_BASE               (SCS_BASE + 0x0100)
#define SCB_BASE                (SCS_BASE + 0x0D00)

/* Core bit definitions */
#define SCB_AIRCR_VECTKEY_Pos      16
#define SCB_AIRCR_VECTKEY_Msk      (0xFFFFUL << 16)
#define SCB_AIRCR_PRIGROUP_Pos      8
#define SCB_AIRCR_PRIGROUP_Msk     (7UL << 8)

#ifdef __cplusplus
}
#endif

#endif /* __CMSIS_CM4_H */