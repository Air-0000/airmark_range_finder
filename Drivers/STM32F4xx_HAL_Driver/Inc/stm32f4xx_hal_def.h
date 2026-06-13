/**
 * @file stm32f4xx_hal_def.h
 * @brief HAL definitions for STM32F4xx
 */
#ifndef __STM32F4xx_HAL_DEF
#define __STM32F4xx_HAL_DEF

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* CMSIS compiler abstractions */
#if defined(__GNUC__)
  #define __WEAK __attribute__((weak))
  #define __ASM __asm
  #define __INLINE inline
  #define __STATIC_INLINE static inline
  #define __STATIC_FORCEINLINE static inline __attribute__((always_inline))
  #define __NO_RETURN __attribute__((noreturn))
  #define __USED __attribute__((used))
  #define __PACKED __attribute__((packed, aligned(1)))
  #define __ALIGNED(n) __attribute__((aligned(n)))
  #define __CLZ __builtin_clz
static inline __attribute__((always_inline)) void __NOP(void) { __asm__ volatile ("nop" ::: "memory"); }
#define __RBIT(X) __builtin_bitreverse(X)
#elif defined(__CC_ARM)
  #define __WEAK __attribute__((weak))
  #define __ASM __asm
  #define __INLINE __inline
  #define __STATIC_INLINE static __inline
  #define __STATIC_FORCEINLINE static __forceinline
  #define __NO_RETURN __declspec(noreturn)
  #define __USED __attribute__((used))
  #define __PACKED __attribute__((packed))
  #define __ALIGNED(n) __attribute__((aligned(n)))
#endif

/* Exported types ------------------------------------------------------------*/
typedef enum {
    HAL_OK = 0x00U,
    HAL_ERROR = 0x01U,
    HAL_BUSY = 0x02U,
    HAL_TIMEOUT = 0x03U
} HAL_StatusTypeDef;

typedef enum {
    HAL_UNLOCKED = 0x00U,
    HAL_LOCKED = 0x01U
} HAL_LockTypeDef;

typedef enum {
    DISABLE = 0U,
    ENABLE = !DISABLE
} FunctionalState;

typedef enum {
    RESET = 0U,
    SET = !RESET
} FlagStatus, ITStatus;

typedef enum {
    UNUSED = 0U,
    USED = !UNUSED
} PriorityStatus;

/* Exported macros -----------------------------------------------------------*/
#define UNUSED(X) (void)X
#define SET_BIT(REG, BIT) ((REG) |= (BIT))
#define CLEAR_BIT(REG, BIT) ((REG) &= ~(BIT))
#define READ_BIT(REG, BIT) ((REG) & (BIT))
#define CLEAR_REG(REG) ((REG) = 0x0U)
#define WRITE_REG(REG, VAL) ((REG) = (VAL))
#define READ_REG(REG) ((REG))
#define MODIFY_REG(REG, CLEARMASK, SETMASK) \
    WRITE_REG((REG), (((READ_REG(REG)) & (~(CLEARMASK))) | (SETMASK)))

#define POSITION_VAL(X) (__CLZ(__RBIT(X)))

/* Assert macros */
#define assert_param(expr) ((void)0U)

/* HAL delay max value */
#define HAL_MAX_DELAY  0xFFFFFFFFU

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4xx_HAL_DEF */