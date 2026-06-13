/**
 * @file cmsis_gcc.h
 * @brief GCC compiler abstractions
 */
#ifndef __CMSIS_GCC_H
#define __CMSIS_GCC_H

#ifdef __cplusplus
extern "C" {
#endif

/* GCC-specific macros */
#define __ASM __asm
#define __INLINE inline
#define __STATIC_INLINE static inline
#define __STATIC_FORCEINLINE static inline __attribute__((always_inline))
#define __NO_RETURN __attribute__((noreturn))
#define __USED __attribute__((used))
#define __WEAK __attribute__((weak))
#define __PACKED __attribute__((packed, aligned(1)))
#define __ALIGNED(n) __attribute__((aligned(n)))
#define __PACKED_STRUCT struct __attribute__((packed, aligned(1)))
#define __PACKED_UNION union __attribute__((packed, aligned(1)))
#define __RESTRICT __restrict

/* GCC __builtin support */
#define __CLZ __builtin_clz
#define __CLZLL __builtin_clzll

#ifdef __cplusplus
}
#endif

#endif /* __CMSIS_GCC_H */