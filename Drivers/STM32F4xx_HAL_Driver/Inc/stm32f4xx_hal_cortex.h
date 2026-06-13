/**
 * @file stm32f4xx_hal_cortex.h
 * @brief CORTEX HAL driver header
 */
#ifndef __STM32F4xx_HAL_CORTEX_H
#define __STM32F4xx_HAL_CORTEX_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_def.h"
#include "stm32f407xx.h"

/* Exported types ------------------------------------------------------------*/
typedef struct {
    uint32_t NVIC_IRQChannel;
    uint32_t NVIC_IRQChannelPreemptionPriority;
    uint32_t NVIC_IRQChannelSubPriority;
    FunctionalState NVIC_IRQChannelCmd;
} NVIC_InitTypeDef;

/* Exported constants --------------------------------------------------------*/
/* SysTick Control / Status Register bits */
#define SYSTICK_CTRL_ENABLE          0x00000001U
#define SYSTICK_CTRL_TICKINT         0x00000002U
#define SYSTICK_CTRL_CLKSOURCE       0x00000004U
#define SYSTICK_CTRL_COUNTFLAG       0x00010000U

/* SysTick Reload Register mask */
#define SYSTICK_LOAD_RELOAD_Msk      0x00FFFFFFU

/* SysTick control register bits */
#define SYSTICK_CTRL_ENABLE          0x00000001U
#define SYSTICK_CTRL_TICKINT         0x00000002U
#define SYSTICK_CTRL_CLKSOURCE       0x00000004U
#define SYSTICK_CTRL_COUNTFLAG       0x00010000U

/* SysTick clock source selection */
#define SYSTICK_CLKSOURCE_HCLK       0x00000004U  /* HCLK as SysTick clock */
#define SYSTICK_CLKSOURCE_HCLK_DIV8  0x00000000U  /* HCLK/8 as SysTick clock */

/* Priority group */
#define NVIC_PRIORITYGROUP_0         0x00000007U
#define NVIC_PRIORITYGROUP_1         0x0000000FU
#define NVIC_PRIORITYGROUP_2         0x0000003FU
#define NVIC_PRIORITYGROUP_3         0x000000FFU
#define NVIC_PRIORITYGROUP_4         0x00000FFFU

/* __NVIC_PRIO_BITS is defined in core_cm4.h (4 for STM32F4) */
#ifndef __NVIC_PRIO_BITS
#define __NVIC_PRIO_BITS              4U
#endif

/* Exported macros -----------------------------------------------------------*/
#define __NVIC_SetPriority(IRQn, Priority) \
    (((0UL + ((__NVIC_PRIO_BITS) - 1UL)) \
      < (31UL - (IRQn))) ? \
     (~(((1UL << ((__NVIC_PRIO_BITS) - 1UL)) - 1UL) << (1UL + ((IRQn) & (1UL << 1UL)))) \
      & ((7UL) << ((IRQn) & (1UL << 1UL)))) \
     : \
     ((7UL) << ((IRQn) & 0xFUL)))

#define __NVIC_GetPriority(IRQn) \
    (((IRQn) < 0UL) ? \
     ((uint32_t)(__NVIC_SetPriority(IRQn, 0UL)) >> (1UL + ((__NVIC_PRIO_BITS) - 1UL))) : \
     ((uint32_t)(__NVIC_SetPriority(IRQn, 0UL)) >> (1UL + ((__NVIC_PRIO_BITS) - 1UL))))

/* Function-like macros */
#define NVIC_SetPriority(IRQn, priority)   __NVIC_SetPriority(IRQn, priority)
#define NVIC_GetPriority(IRQn)            __NVIC_GetPriority(IRQn)

#define NVIC_EnableIRQ(IRQn)               do { \
    __NVIC_EnableIRQ((IRQn)); \
} while(0U)

#define NVIC_DisableIRQ(IRQn)              do { \
    __NVIC_DisableIRQ((IRQn)); \
} while(0U)

#define __NVIC_EnableIRQ(IRQn) \
    do { \
        uint32_t tmpreg = (uint32_t)(IRQn); \
        NVIC->ISER[(uint32_t)(IRQn) >> 5UL] = (uint32_t)(1UL << ((uint32_t)(IRQn) & (uint32_t)0x1FUL)); \
        (void)tmpreg; \
    } while(0U)

#define __NVIC_DisableIRQ(IRQn) \
    do { \
        uint32_t tmpreg = (uint32_t)(IRQn); \
        NVIC->ICER[(uint32_t)(IRQn) >> 5UL] = (uint32_t)(1UL << ((uint32_t)(IRQn) & (uint32_t)0x1FUL)); \
        (void)tmpreg; \
    } while(0U)

/* Exported functions --------------------------------------------------------*/
void HAL_NVIC_SetPriorityGrouping(uint32_t PriorityGroup);
void HAL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t PreemptPriority, uint32_t SubPriority);
void HAL_NVIC_EnableIRQ(IRQn_Type IRQn);
void HAL_NVIC_DisableIRQ(IRQn_Type IRQn);
void HAL_NVIC_SystemReset(void);
void HAL_SYSTICK_CLKSourceConfig(uint32_t CLKSource);
uint32_t HAL_SYSTICK_Config(uint32_t Ticks);
void HAL_SYSTICK_PriorityConfig(uint32_t Priority);
void HAL_SYSTICK_INTConfig(FunctionalState State);
void HAL_SYSTICK_IRQHandler(void);

#define __HAL_CORTEX_HAL_SYSTICK_CLK_SOURCE          SYSTICK_CLKSOURCE_HCLK
#define __HAL_SYSTICK_CLK_SOURCE_CONFIG(__CLK__)     HAL_SYSTICK_CLKSourceConfig(__CLK__)
#define __HAL_SYSTICK_ENABLE()                       do { SysTick->CTRL |= SYSTICK_CTRL_ENABLE; } while(0U)
#define __HAL_SYSTICK_DISABLE()                      do { SysTick->CTRL &= ~SYSTICK_CTRL_ENABLE; } while(0U)
#define __HAL_SYSTICK_INT_ENABLE()                   do { SysTick->CTRL |= SYSTICK_CTRL_TICKINT; } while(0U)
#define __HAL_SYSTICK_INT_DISABLE()                  do { SysTick->CTRL &= ~SYSTICK_CTRL_TICKINT; } while(0U)

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_CORTEX_H */
