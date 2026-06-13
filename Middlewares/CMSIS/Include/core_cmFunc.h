/**
 * @file core_cmFunc.h
 * @brief CMSIS Cortex-M Core Function Access Header
 */
#ifndef __CMSIS_CMFUNC_H
#define __CMSIS_CMFUNC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Core function access - mostly intrinsics */
static inline void __NOP(void)
{
    __asm volatile ("nop");
}

static inline void __WFI(void)
{
    __asm volatile ("wfi");
}

static inline void __WFE(void)
{
    __asm volatile ("wfe");
}

static inline void __SEV(void)
{
    __asm volatile ("sev");
}

static inline void __ISB(void)
{
    __asm volatile ("isb 0xF":::"memory");
}

static inline void __DSB(void)
{
    __asm volatile ("dsb 0xF":::"memory");
}

static inline void __DMB(void)
{
    __asm volatile ("dmb 0xF":::"memory");
}

static inline uint32_t __get_CONTROL(void)
{
    uint32_t result;
    __asm volatile ("MRS %0, control" : "=r" (result));
    return result;
}

static inline void __set_CONTROL(uint32_t control)
{
    __asm volatile ("MSR control, %0" : : "r" (control));
}

static inline uint32_t __get_IPSR(void)
{
    uint32_t result;
    __asm volatile ("MRS %0, ipsr" : "=r" (result));
    return result;
}

static inline uint32_t __get_APSR(void)
{
    uint32_t result;
    __asm volatile ("MRS %0, apsr" : "=r" (result));
    return result;
}

static inline uint32_t __get_xPSR(void)
{
    uint32_t result;
    __asm volatile ("MRS %0, xpsr" : "=r" (result));
    return result;
}

static inline uint32_t __get_PSP(void)
{
    uint32_t result;
    __asm volatile ("MRS %0, psp" : "=r" (result));
    return result;
}

static inline void __set_PSP(uint32_t topOfProcStack)
{
    __asm volatile ("MSR psp, %0" : : "r" (topOfProcStack));
}

static inline uint32_t __get_MSP(void)
{
    uint32_t result;
    __asm volatile ("MRS %0, msp" : "=r" (result));
    return result;
}

static inline void __set_MSP(uint32_t topOfMainStack)
{
    __asm volatile ("MSR msp, %0" : : "r" (topOfMainStack));
}

static inline uint32_t __get_PRIMASK(void)
{
    uint32_t result;
    __asm volatile ("MRS %0, primask" : "=r" (result));
    return result;
}

static inline void __set_PRIMASK(uint32_t priMask)
{
    __asm volatile ("MSR primask, %0" : : "r" (priMask));
}

#ifdef __cplusplus
}
#endif

#endif /* __CMSIS_CMFUNC_H */