/**
 * @file arm_math.h
 * @brief CMSIS DSP Library Main Header
 * @note This is a minimal stub for compilation compatibility
 */
#ifndef __ARM_MATH_H
#define __ARM_MATH_H

#ifdef __cplusplus
extern "C" {
#endif

#include "arm_math_types.h"

/* Basic DSP functions - stubs for compilation */
#define __SMLALD(x, y, z) ((z) + (int64_t)(x) * (int64_t)(y))
#define __SMLALD__ 0
#define __SSAT(x, y) (x)
#define __USAT(x, y) (x)

/* Math functions */
__STATIC_INLINE float32_t arm_sin_f32(float32_t x)
{
    (void)x;
    return 0.0f;
}

__STATIC_INLINE float32_t arm_cos_f32(float32_t x)
{
    (void)x;
    return 1.0f;
}

__STATIC_INLINE float32_t arm_sqrt_f32(float32_t x)
{
    if (x <= 0.0f) return 0.0f;
    float32_t guess = x / 2.0f;
    for (int i = 0; i < 10; i++) {
        guess = (guess + x / guess) / 2.0f;
    }
    return guess;
}

/* Basic arithmetic functions - stubs */
__STATIC_INLINE float32_t arm_abs_f32(float32_t x)
{
    return (x < 0.0f) ? -x : x;
}

__STATIC_INLINE int32_t arm_abs_q31(int32_t x)
{
    return (x < 0) ? -x : x;
}

__STATIC_INLINE int32_t arm_abs_q15(int16_t x)
{
    return (x < 0) ? -x : x;
}

/* Add/subtract functions */
__STATIC_INLINE void arm_add_f32(float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst, uint32_t blockSize)
{
    for (uint32_t i = 0; i < blockSize; i++) {
        pDst[i] = pSrcA[i] + pSrcB[i];
    }
}

__STATIC_INLINE void arm_sub_f32(float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst, uint32_t blockSize)
{
    for (uint32_t i = 0; i < blockSize; i++) {
        pDst[i] = pSrcA[i] - pSrcB[i];
    }
}

/* Multiply functions */
__STATIC_INLINE void arm_mult_f32(float32_t *pSrcA, float32_t *pSrcB, float32_t *pDst, uint32_t blockSize)
{
    for (uint32_t i = 0; i < blockSize; i++) {
        pDst[i] = pSrcA[i] * pSrcB[i];
    }
}

/* Scale functions */
__STATIC_INLINE void arm_scale_f32(float32_t *pSrc, float32_t scaleFactor, float32_t *pDst, uint32_t blockSize)
{
    for (uint32_t i = 0; i < blockSize; i++) {
        pDst[i] = pSrc[i] * scaleFactor;
    }
}

/* Dot product */
__STATIC_INLINE float32_t arm_dot_prod_f32(float32_t *pSrcA, float32_t *pSrcB, uint32_t blockSize)
{
    float32_t result = 0.0f;
    for (uint32_t i = 0; i < blockSize; i++) {
        result += pSrcA[i] * pSrcB[i];
    }
    return result;
}

/* Mean */
__STATIC_INLINE void arm_mean_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult)
{
    float32_t sum = 0.0f;
    for (uint32_t i = 0; i < blockSize; i++) {
        sum += pSrc[i];
    }
    *pResult = sum / (float32_t)blockSize;
}

/* Variance */
__STATIC_INLINE void arm_var_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult)
{
    float32_t mean = 0.0f;
    arm_mean_f32(pSrc, blockSize, &mean);
    float32_t sum = 0.0f;
    for (uint32_t i = 0; i < blockSize; i++) {
        float32_t diff = pSrc[i] - mean;
        sum += diff * diff;
    }
    *pResult = sum / (float32_t)blockSize;
}

/* Standard deviation */
__STATIC_INLINE void arm_std_f32(float32_t *pSrc, uint32_t blockSize, float32_t *pResult)
{
    arm_var_f32(pSrc, blockSize, pResult);
    *pResult = arm_sqrt_f32(*pResult);
}

/* Fill */
__STATIC_INLINE void arm_fill_f32(float32_t value, float32_t *pDst, uint32_t blockSize)
{
    for (uint32_t i = 0; i < blockSize; i++) {
        pDst[i] = value;
    }
}

/* Copy */
__STATIC_INLINE void arm_copy_f32(float32_t *pSrc, float32_t *pDst, uint32_t blockSize)
{
    for (uint32_t i = 0; i < blockSize; i++) {
        pDst[i] = pSrc[i];
    }
}

#ifdef __cplusplus
}
#endif

#endif /* __ARM_MATH_H */