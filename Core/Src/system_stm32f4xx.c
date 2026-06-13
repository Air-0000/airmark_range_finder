/**
 * @file system_stm32f4xx.c
 * @brief STM32F4xx system source file
 * @note This file provides two functions:
 *       SystemInit()  - setup the PLL to run at 168MHz using HSE
 *       SystemCoreClockUpdate() - update SystemCoreClock variable
 *
 * Target: STM32F407, HSE=8MHz, PLLCLK=168MHz
 */
#include "stm32f4xx_hal.h"
#include "system_stm32f4xx.h"

/**
 * @brief  Setup the microcontroller system
 *         Initialize the FPU setting, vector table configuration
 *         and PLL configuration to run at 168MHz.
 */
void SystemInit(void)
{
    /* FPU settings ------------------------------------------------------------*/
    #if (__FPU_PRESENT == 1U) && (__FPU_USED == 1U)
        SCB->CPACR |= ((3UL << 10*2U) | (3UL << 11*2U));  /* set CP10 and CP11 Full Access */
    #endif

    /* Reset the RCC clock configuration to the default reset state --------*/
    /* Set HSION bit */
    RCC->CR |= (uint32_t)0x00000001U;

    /* Reset CFGR register */
    RCC->CFGR = 0x00000000U;

    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t)0xFEF6FFFFU;

    /* Reset PLLCFGR register to default value */
    RCC->PLLCFGR = 0x24003010U;

    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t)0xFFFBFFFFU;

    /* Disable all interrupts */
    RCC->CIR = 0x00000000U;

    /* Configure the Vector Table location add offset address ------------------*/
    #ifdef VECT_TAB_SRAM
        SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET;  /* Vector Table Relocation in Internal SRAM */
    #else
        SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */
    #endif

    /* Configure Clock with HSE=8MHz -> PLL -> 168MHz ------------------------*/
    /* Enable HSE */
    RCC->CR |= ((uint32_t)RCC_CR_HSEON);

    /* Wait until HSE is ready */
    while ((RCC->CR & RCC_CR_HSERDY) == 0U) { __NOP(); }

    /* Configure PLL: HSE=8MHz, PLLM=8, PLLN=336, PLLP=2 -> PLLCLK=168MHz ----*/
    /* PLLCFGR: PLLM=8, PLLSRC=HSE, PLLP=2 (00b), PLLN=336 -------------------*/
    RCC->PLLCFGR = (8U << RCC_PLLCFGR_PLLM_Pos)   |
                   RCC_PLLCFGR_PLLSRC_HSE          |
                   (2U << RCC_PLLCFGR_PLLP_Pos)    |   /* PLLP=2  -> PLLCLK = 168MHz */
                   (336U << RCC_PLLCFGR_PLLN_Pos)   |
                   (0U << RCC_PLLCFGR_PLLQ_Pos);        /* PLLQ=2  -> 48MHz for USB/SDIO */

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait until PLL is ready */
    while ((RCC->CR & RCC_CR_PLLRDY) == 0U) { __NOP(); }

    /* Configure Flash prefetch, Instruction cache, Data cache and wait state ---*/
    FLASH->ACR = (uint32_t)(FLASH_ACR_ICEN |    /* Instruction cache enable */
                            FLASH_ACR_DCEN |     /* Data cache enable */
                            FLASH_ACR_LATENCY_5WS);  /* 5 CPU cycles latency (168MHz) */

    /* Select PLL as system clock source --------------------------------------*/
    /* AHB prescaler: /1  (HCLK=168MHz) */
    /* APB1 prescaler: /4  (PCLK1=42MHz, max 42MHz) */
    /* APB2 prescaler: /2  (PCLK2=84MHz, max 84MHz) */
    RCC->CFGR = (0U << RCC_CFGR_HPRE_Pos)   |   /* AHB  prescaler = /1  -> 168MHz */
                (5U << RCC_CFGR_PPRE1_Pos)  |   /* APB1 prescaler = /4  -> 42MHz  */
                (4U << RCC_CFGR_PPRE2_Pos)   |   /* APB2 prescaler = /2  -> 84MHz  */
                RCC_CFGR_SW_PLL;                  /* System clock switch = PLL */

    /* Wait until PLL is used as system clock */
    while ((RCC->CFGR & RCC_CFGR_SWS) != (RCC_CFGR_SWS_PLL)) { __NOP(); }

    /* Update SystemCoreClock variable ----------------------------------------*/
    SystemCoreClock = 168000000U;
}

/**
 * @brief  Update SystemCoreClock variable
 *         Must be called whenever the clock changes.
 */
void SystemCoreClockUpdate(void)
{
    uint32_t tmp = 0U, pllvco = 0U, pllp = 2U, pllm = 2U;

    /* Get SYSCLK source -------------------------------------------------------*/
    tmp = RCC->CFGR & RCC_CFGR_SWS;

    switch (tmp) {
        case 0x00U:  /* HSI used as system clock */
            SystemCoreClock = HSI_VALUE;
            break;
        case 0x04U:  /* HSE used as system clock */
            SystemCoreClock = HSE_VALUE;
            break;
        case 0x08U:  /* PLL used as system clock */
            /* Get PLL clock source and multiplication factor --------------------*/
            pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;
            if (((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> RCC_PLLCFGR_PLLSRC_Pos) == 0U) {
                /* HSI oscillator clock selected as PLL clock source */
                pllvco = (HSI_VALUE * (RCC->PLLCFGR & RCC_PLLCFGR_PLLN)) / pllm;
            } else {
                /* HSE oscillator clock selected as PLL clock source */
                pllvco = (HSE_VALUE * (RCC->PLLCFGR & RCC_PLLCFGR_PLLN)) / pllm;
            }

            /* Get PLL division factor ------------------------------------------*/
            pllp = ((((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> RCC_PLLCFGR_PLLP_Pos) + 1U) * 2U);

            SystemCoreClock = pllvco / pllp;
            break;
        default:
            SystemCoreClock = HSI_VALUE;
            break;
    }

    /* Compute HCLK clock frequency --------------------------------------------*/
    /* Get HCLK prescaler */
    tmp = (RCC->CFGR & RCC_CFGR_HPRE) >> RCC_CFGR_HPRE_Pos;
    if (tmp < 8U) {
        SystemCoreClock >>= tmp;
    } else {
        SystemCoreClock >>= (tmp - 6U);
    }
}

/**
 * System Clock (SystemCoreClock)
 * Defaulted to 168000000Hz after SystemInit()
 */
uint32_t SystemCoreClock = 168000000U;
