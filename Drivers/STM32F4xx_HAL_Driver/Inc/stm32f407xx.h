/**
 * @file stm32f407xx.h
 * @brief STM32F407ZG device definitions
 */
#ifndef __STM32F407xx_H
#define __STM32F407xx_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Base addresses ------------------------------------------------------------*/
#define FLASH_BASE            0x08000000UL
#define SRAM1_BASE           0x20000000UL
#define SRAM2_BASE           0x2001C000UL
#define PERIPH_BASE          0x40000000UL
#define APB1PERIPH_BASE      PERIPH_BASE
#define APB2PERIPH_BASE      (PERIPH_BASE + 0x00010000UL)
#define AHB1PERIPH_BASE      (PERIPH_BASE + 0x00020000UL)
#define AHB2PERIPH_BASE      (PERIPH_BASE + 0x08000000UL)
#define FLASH_R_BASE         (AHB1PERIPH_BASE + 0x3C00UL)
#define EXTI_BASE            (APB2PERIPH_BASE + 0x3C00UL)
#define SYSCFG_BASE          (APB2PERIPH_BASE + 0x3800UL)
#define SCB_BASE             0xE000ED00UL
#define DWT_BASE             0xE0001000UL

/* GPIO bases */
#define GPIOA_BASE           (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE           (AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE           (AHB1PERIPH_BASE + 0x0800UL)
#define GPIOD_BASE           (AHB1PERIPH_BASE + 0x0C00UL)
#define GPIOE_BASE           (AHB1PERIPH_BASE + 0x1000UL)
#define GPIOH_BASE           (AHB1PERIPH_BASE + 0x1C00UL)

/* RCC base */
#define RCC_BASE             (AHB1PERIPH_BASE + 0x3800UL)

/* DCMI base */
#define DCMI_BASE            (APB2PERIPH_BASE + 0x5000UL)

/* DMA base */
#define DMA2_BASE            (AHB1PERIPH_BASE + 0x6400UL)

/* TIM bases */
#define TIM2_BASE            (APB1PERIPH_BASE + 0x0000UL)
#define TIM3_BASE            (APB1PERIPH_BASE + 0x0400UL)
#define TIM4_BASE            (APB1PERIPH_BASE + 0x0800UL)
#define TIM5_BASE            (APB1PERIPH_BASE + 0x0C00UL)

/* I2C bases */
#define I2C1_BASE            (APB1PERIPH_BASE + 0x5400UL)
#define I2C2_BASE            (APB1PERIPH_BASE + 0x5800UL)
#define I2C3_BASE            (APB1PERIPH_BASE + 0x5C00UL)

/* CRC base */
#define CRC_BASE             (AHB2PERIPH_BASE + 0x3000UL)

/* ============================================================
 * Interrupt Number Definition (IRQn_Type)
 * ============================================================*/
typedef enum {
    NonMaskableInt_IRQn         = -14,
    MemoryManagement_IRQn       = -12,
    BusFault_IRQn               = -11,
    UsageFault_IRQn             = -10,
    SVCall_IRQn                 = -5,
    DebugMonitor_IRQn            = -4,
    PendSV_IRQn                 = -2,
    SysTick_IRQn                = -1,

    WWDG_IRQn                   = 0,
    PVD_IRQn                    = 1,
    TAMP_STAMP_IRQn             = 2,
    RTC_WKUP_IRQn               = 3,
    FLASH_IRQn                   = 4,
    RCC_IRQn                    = 5,
    EXTI0_IRQn                  = 6,
    EXTI1_IRQn                  = 7,
    EXTI2_IRQn                  = 8,
    EXTI3_IRQn                  = 9,
    EXTI4_IRQn                  = 10,
    DMA1_Stream0_IRQn           = 11,
    DMA1_Stream1_IRQn           = 12,
    DMA1_Stream2_IRQn           = 13,
    DMA1_Stream3_IRQn           = 14,
    DMA1_Stream4_IRQn           = 15,
    DMA1_Stream5_IRQn           = 16,
    DMA1_Stream6_IRQn           = 17,
    ADC_IRQn                    = 18,
    CAN1_TX_IRQn                = 19,
    CAN1_RX0_IRQn               = 20,
    CAN1_RX1_IRQn               = 21,
    CAN1_SCE_IRQn               = 22,
    EXTI9_5_IRQn                = 23,
    TIM1_BRK_TIM9_IRQn          = 24,
    TIM1_UP_TIM10_IRQn          = 25,
    TIM1_CC_IRQn                = 26,
    TIM2_IRQn                   = 27,
    TIM3_IRQn                   = 28,
    TIM4_IRQn                   = 29,
    I2C1_EV_IRQn                = 31,
    I2C1_ER_IRQn                = 32,
    I2C2_EV_IRQn                = 33,
    I2C2_ER_IRQn                = 34,
    SPI1_IRQn                   = 35,
    SPI2_IRQn                   = 36,
    USART1_IRQn                 = 37,
    USART2_IRQn                 = 38,
    USART3_IRQn                 = 39,
    EXTI15_10_IRQn              = 40,
    RTC_Alarm_IRQn              = 41,
    OTG_FS_WKUP_IRQn           = 42,
    TIM8_BRK_TIM12_IRQn        = 43,
    TIM8_UP_TIM13_IRQn         = 44,
    TIM8_TRG_COM_TIM14_IRQn    = 45,
    TIM8_CC_IRQn                = 46,
    DMA1_Stream7_IRQn           = 47,
    SDIO_IRQn                   = 49,
    TIM5_IRQn                   = 50,
    SPI3_IRQn                   = 51,
    DMA2_Stream0_IRQn           = 56,
    DMA2_Stream1_IRQn           = 57,
    DMA2_Stream2_IRQn           = 58,
    DMA2_Stream3_IRQn           = 59,
    DMA2_Stream4_IRQn           = 60,
    CAN2_TX_IRQn                = 63,
    CAN2_RX0_IRQn               = 64,
    CAN2_RX1_IRQn               = 65,
    CAN2_SCE_IRQn               = 66,
    OTG_FS_IRQn                 = 67,
    DMA2_Stream5_IRQn           = 68,
    DMA2_Stream6_IRQn           = 69,
    DMA2_Stream7_IRQn           = 70,
    USART6_IRQn                 = 71,
    I2C3_EV_IRQn                = 72,
    I2C3_ER_IRQn                = 73,
    FPU_IRQn                    = 81,
    SPI4_IRQn                   = 84,
    SPI5_IRQn                   = 85,
    SPI6_IRQn                   = 86,
} IRQn_Type;

/* ============================================================
 * Core Registers
 * ============================================================*/
typedef struct {
    volatile uint32_t CPUID;
    volatile uint32_t ICSR;
    volatile uint32_t VTOR;
    volatile uint32_t AIRCR;
    volatile uint32_t SCR;
    volatile uint32_t CCR;
    volatile uint8_t  SHP[12];
    volatile uint32_t SHCSR;
    volatile uint32_t CFSR;
    volatile uint32_t HFSR;
    volatile uint32_t DFSR;
    volatile uint32_t MMFAR;
    volatile uint32_t BFAR;
    volatile uint32_t AFSR;
    volatile uint32_t PFR[2];
    volatile uint32_t DFR;
    volatile uint32_t AMR;
    volatile uint32_t MAR;
    volatile uint32_t PFAR[2];
} SCB_TypeDef;

#define SCB                 ((SCB_TypeDef *) SCB_BASE)

/* SysTick */
#define SYSTICK_BASE        0xE000E010UL
typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
    volatile uint32_t CALIB;
} SysTick_TypeDef;

#define SysTick             ((SysTick_TypeDef *) SYSTICK_BASE)

/* DWT (Data Watchpoint and Trace) - for cycle counting */
#define DWT                 ((DWT_TypeDef *) DWT_BASE)
typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t CYCCNT;
    volatile uint32_t CPICNT;
    volatile uint32_t EXCCNT;
    volatile uint32_t SLEEPCNT;
    volatile uint32_t LSUCNT;
    volatile uint32_t FOLDCNT;
    volatile uint32_t PCSR;
    volatile uint32_t COMP0;
    volatile uint32_t MASK0;
    volatile uint32_t FUNCTION0;
    volatile uint32_t COMP1;
    volatile uint32_t MASK1;
    volatile uint32_t FUNCTION1;
    volatile uint32_t COMP2;
    volatile uint32_t MASK2;
    volatile uint32_t FUNCTION2;
    volatile uint32_t COMP3;
    volatile uint32_t MASK3;
    volatile uint32_t FUNCTION3;
} DWT_TypeDef;

/* DWT CTRL bits */
#define DWT_CTRL_CYCCNTENA  0x00000001U

/* ============================================================
 * FLASH Registers
 * ============================================================*/
typedef struct {
    volatile uint32_t ACR;
    volatile uint32_t KEYR;
    volatile uint32_t OPTKEYR;
    volatile uint32_t SR;
    volatile uint32_t CR;
    volatile uint32_t OPTCR;
} FLASH_TypeDef;

#define FLASH              ((FLASH_TypeDef *) FLASH_R_BASE)

/* FLASH ACR bits */
#define FLASH_ACR_ICEN     0x00002000U
#define FLASH_ACR_DCEN     0x00001000U
#define FLASH_ACR_LATENCY_5WS  0x00000005U

/* ============================================================
 * EXTI Registers
 * ============================================================*/
typedef struct {
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
} EXTI_TypeDef;

#define EXTI               ((EXTI_TypeDef *) EXTI_BASE)

/* ============================================================
 * SYSCFG Registers
 * ============================================================*/
typedef struct {
    volatile uint32_t MEMRMP;
    volatile uint32_t PMC;
    volatile uint32_t EXTICR[4];
    volatile uint32_t CMPCR;
    volatile uint32_t CFGR;
} SYSCFG_TypeDef;

#define SYSCFG             ((SYSCFG_TypeDef *) SYSCFG_BASE)

/* ============================================================
 * GPIO Registers
 * ============================================================*/
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIO_TypeDef;

/* ============================================================
 * RCC Registers
 * ============================================================*/
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t RESERVED0;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    uint32_t RESERVED2;
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
    uint32_t RESERVED3[2];
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    uint32_t RESERVED4[2];
    volatile uint32_t SSCGR;
    volatile uint32_t DCKCFGR;
} RCC_TypeDef;

/* RCC CR bits */
#define RCC_CR_HSION       0x00000001U
#define RCC_CR_HSIRDY      0x00000002U
#define RCC_CR_HSEON       0x00010000U
#define RCC_CR_HSERDY      0x00020000U
#define RCC_CR_HSEBYP      0x00040000U
#define RCC_CR_PLLON       0x01000000U
#define RCC_CR_PLLRDY      0x02000000U

/* RCC PLLCFGR bits */
#define RCC_PLLCFGR_PLLM_Pos   0U
#define RCC_PLLCFGR_PLLN_Pos   6U
#define RCC_PLLCFGR_PLLP_Pos   16U
#define RCC_PLLCFGR_PLLQ_Pos   24U
#define RCC_PLLCFGR_PLLSRC_HSE (1U << 22U)
#define RCC_PLLCFGR_PLLSRC     (1U << 22U)
#define RCC_PLLCFGR_PLLSRC_Pos 22U
#define RCC_PLLCFGR_PLLM        (0x3FU << RCC_PLLCFGR_PLLM_Pos)
#define RCC_PLLCFGR_PLLN        (0x1FFU << RCC_PLLCFGR_PLLN_Pos)
#define RCC_PLLCFGR_PLLP        (3U << RCC_PLLCFGR_PLLP_Pos)
#define RCC_PLLCFGR_PLLQ        (0xFU << RCC_PLLCFGR_PLLQ_Pos)

/* RCC CFGR bits */
#define RCC_CFGR_SW_PLL         0x00000002U
#define RCC_CFGR_SWS_PLL        0x00000008U
#define RCC_CFGR_SWS            0x0000000CU
#define RCC_CFGR_HPRE_Pos       4U
#define RCC_CFGR_PPRE1_Pos      10U
#define RCC_CFGR_PPRE2_Pos      13U
#define RCC_CFGR_HPRE            (0xFU << RCC_CFGR_HPRE_Pos)
#define RCC_CFGR_PPRE1           (0x7U << RCC_CFGR_PPRE1_Pos)
#define RCC_CFGR_PPRE2           (0x7U << RCC_CFGR_PPRE2_Pos)

/* ============================================================
 * DMA Registers
 * ============================================================*/
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t NDTR;
    volatile uint32_t PAR;
    volatile uint32_t M0AR;
    volatile uint32_t M1AR;
    volatile uint32_t FCR;
} DMA_Stream_TypeDef;

typedef struct {
    volatile uint32_t LISR;
    volatile uint32_t HISR;
    volatile uint32_t LIFCR;
    volatile uint32_t HIFCR;
    DMA_Stream_TypeDef S[8];
} DMA_TypeDef;

/* DMA CR bits */
#define DMA_SxCR_CHSEL       0x06000000U
#define DMA_SxCR_MINC         0x00000080U
#define DMA_SxCR_PINC         0x00000040U
#define DMA_SxCR_MBURST_SINGLE  0x00000000U
#define DMA_SxCR_MBURST_INC4     0x00000800U
#define DMA_SxCR_PBURST_SINGLE   0x00000000U
#define DMA_SxCR_PBURST_INC4     0x00000400U

/* DMA FCR bits */
#define DMA_SxFCR_FTH         0x00000003U
#define DMA_SxFCR_FIFO_THRESHOLD_FULL  0x00000003U
#define DMA_FIFOMODE_ENABLE    0x00000004U

/* DMA stream selection */
#define DMA2_Stream1          ((DMA_Stream_TypeDef *) (DMA2_BASE + 0x0018UL))

/* ============================================================
 * TIM Registers
 * ============================================================*/
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR[4];
    volatile uint32_t BDTR;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
    volatile uint32_t OR;
} TIM_TypeDef;

/* ============================================================
 * DCMI Registers
 * ============================================================*/
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t SR;
    volatile uint32_t RISR;
    volatile uint32_t IER;
    volatile uint32_t MISR;
    volatile uint32_t ICR;
    volatile uint32_t ESCR;
    volatile uint32_t ESUR;
    volatile uint32_t CWSTRTR;
    volatile uint32_t CWSIZER;
    volatile uint32_t DR;
} DCMI_TypeDef;

/* DCMI CR bits */
#define DCMI_CR_ENABLE       0x00000001U
#define DCMI_CR_ALL_FRAME    0x00000000U
#define DCMI_CR_CAPCNT       0x00000002U
#define DCMI_CR_VSPOLARITY   0x00000080U
#define DCMI_CR_HSPOLARITY   0x00000040U
#define DCMI_CR_PCKPOLARITY  0x00000020U
#define DCMI_CR_EDM_8BIT     0x00000000U
#define DCMI_CR_EDM_16BIT    0x00000080U

/* DCMI CR ByteSelectMode */
#define DCMI_BSM_8BIT        0x00000000U
/* DCMI CR ByteSelectStart */
#define DCMI_OEBS_ODD        0x00000100U
/* DCMI CR LineSelectMode */
#define DCMI_LSM_ALL         0x00000000U

/* DCMI SR bits */
#define DCMI_SR_HSYNC        0x00000001U
#define DCMI_SR_VSYNC        0x00000002U
#define DCMI_SR_FNE          0x00000004U

/* ============================================================
 * I2C Registers
 * ============================================================*/
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t OAR1;
    volatile uint32_t OAR2;
    volatile uint32_t DR;
    volatile uint32_t SR1;
    volatile uint32_t SR2;
    volatile uint32_t CCR;
    volatile uint32_t TRISE;
    volatile uint32_t FLTR;
} I2C_TypeDef;

/* I2C CR1 bits */
#define I2C_CR1_START        0x00000001U
#define I2C_CR1_STOP        0x00000002U
#define I2C_CR1_PE          0x00000001U

/* I2C SR1 bits */
#define I2C_SR1_SB          0x00000001U
#define I2C_SR1_ADDR        0x00000002U
#define I2C_SR1_TXE         0x00000004U
#define I2C_SR1_RXNE        0x00000004U
#define I2C_SR1_BTF         0x00000004U

/* ============================================================
 * CRC Registers
 * ============================================================*/
typedef struct {
    volatile uint32_t DR;
    volatile uint32_t IDR;
    volatile uint32_t CR;
} CRC_TypeDef;

#define CRC                 ((CRC_TypeDef *) CRC_BASE)

/* ============================================================
 * Peripheral Instances
 * ============================================================*/
#define GPIOA               ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOB               ((GPIO_TypeDef *) GPIOB_BASE)
#define GPIOC               ((GPIO_TypeDef *) GPIOC_BASE)
#define GPIOD               ((GPIO_TypeDef *) GPIOD_BASE)
#define GPIOE               ((GPIO_TypeDef *) GPIOE_BASE)
#define GPIOH               ((GPIO_TypeDef *) GPIOH_BASE)
#define RCC                 ((RCC_TypeDef *) RCC_BASE)
#define DMA2                ((DMA_TypeDef *) DMA2_BASE)
#define TIM2                ((TIM_TypeDef *) TIM2_BASE)
#define TIM3                ((TIM_TypeDef *) TIM3_BASE)
#define TIM4                ((TIM_TypeDef *) TIM4_BASE)
#define TIM5                ((TIM_TypeDef *) TIM5_BASE)
#define DCMI                ((DCMI_TypeDef *) DCMI_BASE)
#define I2C1                ((I2C_TypeDef *) I2C1_BASE)
#define I2C2                ((I2C_TypeDef *) I2C2_BASE)
#define I2C3                ((I2C_TypeDef *) I2C3_BASE)

/* ============================================================
 * Standard Bit / Register Defines
 * ============================================================*/

/* Core */
#define NVIC_PRIORITYGROUP_4  0x00000003U
#define __IO volatile
#define __I volatile const
#define __O volatile

/* Bit operations */
#define SET_BIT(REG, BIT)     ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT)    ((REG) & (BIT))

/* GPIO */
#define GPIO_PIN_0          0x0001U
#define GPIO_PIN_1          0x0002U
#define GPIO_PIN_2          0x0004U
#define GPIO_PIN_3          0x0008U
#define GPIO_PIN_4          0x0010U
#define GPIO_PIN_5          0x0020U
#define GPIO_PIN_6          0x0040U
#define GPIO_PIN_7          0x0080U
#define GPIO_PIN_8          0x0100U
#define GPIO_PIN_9          0x0200U
#define GPIO_PIN_10         0x0400U
#define GPIO_PIN_11         0x0800U
#define GPIO_PIN_12         0x1000U
#define GPIO_PIN_13         0x2000U
#define GPIO_PIN_14         0x4000U
#define GPIO_PIN_15         0x8000U
#define GPIO_PIN_All        0xFFFFU
#define GPIO_PIN_RESET       0U
#define GPIO_PIN_SET         1U
#define GPIO_MODE_INPUT      0x00000000U
#define GPIO_MODE_OUTPUT_PP  0x00000001U
#define GPIO_MODE_OUTPUT_OD  0x00000011U
#define GPIO_MODE_AF_PP      0x00000002U
#define GPIO_MODE_AF_OD      0x00000012U
#define GPIO_MODE_ANALOG     0x00000003U
#define GPIO_MODE_IT_RISING  0x10110000U
#define GPIO_MODE_IT_FALLING 0x10210000U
#define GPIO_MODE_IT_RISING_FALLING 0x10310000U
#define GPIO_NOPULL          0x00000000U
#define GPIO_PULLUP          0x00000001U
#define GPIO_PULLDOWN        0x00000002U
#define GPIO_SPEED_FREQ_LOW         0x00000000U
#define GPIO_SPEED_FREQ_MEDIUM      0x00000001U
#define GPIO_SPEED_FREQ_HIGH        0x00000002U
#define GPIO_SPEED_FREQ_VERY_HIGH   0x00000003U

/* GPIO AF */
#define GPIO_AF13_DCMI      0x0DU
#define GPIO_AF4_I2C1       0x04U
#define GPIO_AF4_I2C2       0x04U
#define GPIO_AF4_I2C3       0x04U
#define GPIO_AF7_USART1     0x07U
#define GPIO_AF7_USART2     0x07U
#define GPIO_AF7_USART3     0x07U
#define GPIO_AF5_SPI1       0x05U
#define GPIO_AF5_SPI4       0x05U

/* RCC AHB1ENR */
#define RCC_AHB1ENR_GPIOAEN 0x00000001U
#define RCC_AHB1ENR_GPIOBEN 0x00000002U
#define RCC_AHB1ENR_GPIOCEN 0x00000004U
#define RCC_AHB1ENR_GPIODEN 0x00000008U
#define RCC_AHB1ENR_GPIOEEN 0x00000010U
#define RCC_AHB1ENR_GPIOHEN 0x00000020U
#define RCC_AHB1ENR_CRCEN   0x00001000U
#define RCC_AHB1ENR_DMA1EN  0x00200000U
#define RCC_AHB1ENR_DMA2EN  0x00400000U

/* RCC AHB2ENR */
#define RCC_AHB2ENR_DCMIEN  0x00000001U
#define RCC_AHB2ENR_CRCEN   0x00000010U

/* RCC APB1ENR */
#define RCC_APB1ENR_TIM2EN  0x00000001U
#define RCC_APB1ENR_TIM3EN  0x00000002U
#define RCC_APB1ENR_TIM4EN  0x00000004U
#define RCC_APB1ENR_TIM5EN  0x00000008U
#define RCC_APB1ENR_I2C1EN  0x00200000U
#define RCC_APB1ENR_I2C2EN  0x00400000U
#define RCC_APB1ENR_I2C3EN  0x00800000U
#define RCC_APB1ENR_USART2EN 0x00020000U

/* RCC APB2ENR */
#define RCC_APB2ENR_TIM1EN  0x00000001U
#define RCC_APB2ENR_TIM8EN  0x00000002U
#define RCC_APB2ENR_USART1EN 0x00000010U
#define RCC_APB2ENR_USART6EN 0x00000020U
#define RCC_APB2ENR_SPI1EN  0x00001000U
#define RCC_APB2ENR_SPI4EN  0x00002000U
#define RCC_APB2ENR_SYSCFGEN 0x00004000U
#define RCC_APB2ENR_DCMIEN  0x10000000U

/* DMA */
#define DMA_CHANNEL_0       0x00000000U
#define DMA_CHANNEL_1       0x02000000U
#define DMA_CHANNEL_2       0x04000000U
#define DMA_CHANNEL_3       0x06000000U
#define DMA_PERIPH_TO_MEMORY 0x00000000U
#define DMA_MEMORY_TO_PERIPH 0x00000040U
#define DMA_PINC_DISABLE    0x00000000U
#define DMA_PINC_ENABLE     0x00000040U
#define DMA_MINC_DISABLE    0x00000000U
#define DMA_MINC_ENABLE     0x00000080U
#define DMA_PDATAALIGN_BYTE      0x00000000U
#define DMA_PDATAALIGN_HALFWORD  0x00000100U
#define DMA_PDATAALIGN_WORD      0x00000200U
#define DMA_MDATAALIGN_BYTE      0x00000000U
#define DMA_MDATAALIGN_HALFWORD  0x00000400U
#define DMA_MDATAALIGN_WORD      0x00000800U
#define DMA_NORMAL          0x00000000U
#define DMA_CIRCULAR        0x00000020U
#define DMA_PRIORITY_LOW    0x00000000U
#define DMA_PRIORITY_MEDIUM 0x00001000U
#define DMA_PRIORITY_HIGH   0x00002000U
#define DMA_PRIORITY_VERY_HIGH 0x00003000U
#define DMA_FIFOMODE_DISABLE    0x00000000U
#define DMA_FIFOMODE_ENABLE     0x00000004U
#define DMA_FIFO_THRESHOLD_1QUARTER  0x00000000U
#define DMA_FIFO_THRESHOLD_HALF      0x00000001U
#define DMA_FIFO_THRESHOLD_3QUARTERS 0x00000002U
#define DMA_FIFO_THRESHOLD_FULL      0x00000003U
#define DMA_MBURST_SINGLE   0x00000000U
#define DMA_MBURST_INC4     0x00000800U
#define DMA_PBURST_SINGLE   0x00000000U
#define DMA_PBURST_INC4     0x00000400U
#define DMA_IT_TCIF         0x00000020U
#define DMA_IT_HTIF         0x00000010U
#define DMA_IT_TEIF          0x00000008U

/* DCMI */
#define DCMI_BSM_8BIT       0x00000000U
#define DCMI_OEBS_ODD       0x00000100U
#define DCMI_LSM_ALL        0x00000000U
#define DCMI_EDM_8BIT       0x00000000U
#define DCMI_EDM_16BIT      0x00000080U
#define DCMI_VSPOLARITY_HIGH  0x00000080U
#define DCMI_HSPOLARITY_HIGH  0x00000040U
#define DCMI_PCKPOLARITY_RISING 0x00000000U

/* TIM */
#define TIM_COUNTERMODE_UP         0x00000000U
#define TIM_AUTORELOAD_PRELOAD_ENABLE 0x00000001U
#define TIM_CR1_CEN                0x00000001U
#define TIM_DIER_UIE               0x00000001U

/* CRC */
#define CRC_CR_RESET   0x00000001U

/* DMA flags */
#define DMA_LISR_TCIF0    0x00000001U
#define DMA_LISR_HTIF0    0x00000002U
#define DMA_LISR_TEIF0    0x00000004U
#define DMA_LISR_DMEIF0   0x00000008U
#define DMA_LISR_FEIF0    0x00000010U

#ifdef __cplusplus
}
#endif

#endif /* __STM32F407xx_H */
