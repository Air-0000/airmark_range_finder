/**
 * @file cmsis_armcc.h
 * @brief ARM Compiler 5/6 abstractions
 */
#ifndef __CMSIS_ARMCC_H
#define __CMSIS_ARMCC_H

#ifdef __cplusplus
extern "C" {
#endif

/* ARMCC-specific macros */
#define __ASM __asm
#define __INLINE __inline
#define __STATIC_INLINE static __inline
#define __STATIC_FORCEINLINE static __forceinline
#define __NO_RETURN __declspec(noreturn)
#define __USED __attribute__((used))
#define __WEAK __attribute__((weak))
#define __PACKED __attribute__((packed))
#define __ALIGNED(n) __attribute__((aligned(n)))
#define __PACKED_STRUCT __packed struct
#define __PACKED_UNION __packed union

/* ARMCC does not support __builtin clz/clz for ARMv6M */
#if defined(__TARGET_ARCH_6M) || defined(__TARGET_ARCH_6SM)
  __STATIC_INLINE uint32_t __CLZ(uint32_t data)
  {
    uint32_t count;
    __ASM volatile ("clz %0, %1" : "=r" (count) : "r" (data));
    return count;
  }
#endif

#ifdef __cplusplus
}
#endif

#endif /* __CMSIS_ARMCC_H */