/**
 * @file arm_math_types.h
 * @brief CMSIS DSP Library Types Header
 */
#ifndef __ARM_MATH_TYPES_H
#define __ARM_MATH_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes */
#include <stdint.h>

/* Definition for __FPU_USED */
#ifndef __FPU_USED
#define __FPU_USED 1
#endif

/* Compile options check */
#if   defined ( __CC_ARM )
    #define __ASM __asm
    #define __INLINE __inline
    #define __STATIC_INLINE static __inline
    #define __STATIC_FORCEINLINE __attribute__((always_inline)) static __inline
    #define __NO_RETURN __declspec(noreturn)
    #define __USED __attribute__((used))
    #define __WEAK __attribute__((weak))
    #define __PACKED __attribute__((packed))
    #define __PACKED_STRUCT struct __attribute__((packed))
    #define __PACKED union union __attribute__((packed))
    #define __RESTRICT __restrict
#elif defined ( __ARMCC_VERSION ) && ( __ARMCC_VERSION >= 6010050 )
    #define __ASM __asm
    #define __INLINE __inline
    #define __STATIC_INLINE static __inline
    #define __STATIC_FORCEINLINE __attribute__((always_inline)) static __inline
    #define __NO_RETURN __declspec(noreturn)
    #define __USED __attribute__((used))
    #define __WEAK __attribute__((weak))
    #define __PACKED __attribute__((packed))
    #define __PACKED_STRUCT struct __attribute__((packed))
    #define __PACKED union union __attribute__((packed))
    #define __RESTRICT __restrict
#elif defined ( __GNUC__ )
    #define __ASM __asm
    #define __INLINE inline
    #define __STATIC_INLINE static inline
    #define __STATIC_FORCEINLINE __attribute__((always_inline)) static inline
    #define __NO_RETURN __attribute__((__noreturn__))
    #define __USED __attribute__((used))
    #define __WEAK __attribute__((weak))
    #define __PACKED __attribute__((packed))
    #define __PACKED_STRUCT struct __attribute__((packed)) __attribute__((packed))
    #define __PACKED union union __attribute__((packed))
    #define __RESTRICT __restrict
#else
    #error Unknown compiler.
#endif

/* Basic types */
typedef uint8_t     uint8_t;
typedef int8_t      int8_t;
typedef uint16_t    uint16_t;
typedef int16_t     int16_t;
typedef uint32_t    uint32_t;
typedef int32_t     int32_t;
typedef uint64_t    uint64_t;
typedef int64_t     int64_t;

/* Float types */
typedef float       float32_t;
typedef double      float64_t;

/* Boolean type - guard against stdbool.h */
#ifndef bool
typedef _Bool       bool;
#endif

#ifndef NULL
#define NULL ((void*) 0)
#endif

/* Status codes */
typedef enum {
    ARM_MATH_SUCCESS = 0,
    ARM_MATH_ARGUMENT_ERROR = -1,
    ARM_MATH_LENGTH_ERROR = -2,
    ARM_MATH_SIZE_MISMATCH = -3,
    ARM_MATH_NANINF = -4,
    ARM_MATH_SINGULAR = -5,
    ARM_MATH_TEST_FAILURE = -6
} arm_status;

/* Math functions */
#define ARM_MATH_PI 3.14159265358979f

/* DSP-related definitions */
#define FPU_MATH_TABLES

#ifdef __cplusplus
}
#endif

#endif /* __ARM_MATH_TYPES_H */