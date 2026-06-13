/**
 * @file subpixel_center.h
 * @brief Subpixel Center Module - Gray moment based subpixel localization
 * @author Development Team
 * @note  DSP-accelerated version using ARM CMSIS DSP library
 */
#ifndef __SUBPIXEL_CENTER_H
#define __SUBPIXEL_CENTER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "mark_detection.h"

/* ARM CMSIS DSP Library */
#include "arm_math.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Subpixel position structure
 * @note  Extended with DSP performance metrics
 */
typedef struct {
    float x;              /**< Subpixel X coordinate */
    float y;              /**< Subpixel Y coordinate */
    float weight;         /**< Confidence weight (0.0-1.0) */
    float marker_image_size; /**< Marker image size in pixels (for depth calculation) */
    uint32_t process_cycles; /**< Debug: Processing cycles used */
    uint8_t dsp_used;      /**< Debug: DSP acceleration used (1=yes) */
} Subpixel_PointTypeDef;

/**
 * @brief Subpixel result structure
 */
typedef struct {
    Subpixel_PointTypeDef points[MARKER_COUNT];
    uint8_t valid_count;
    uint8_t is_valid;
} Subpixel_ResultTypeDef;

/* Exported constants --------------------------------------------------------*/
#define SUBPIXEL_WINDOW_SIZE   5U    /**< Window size for moment calculation */
#define SUBPIXEL_MIN_WEIGHT    0.5f  /**< Minimum weight for valid point */
#define SUBPIXEL_USE_DSP       1U   /**< DSP acceleration enabled */
#define SUBPIXEL_DSP_BLOCK_SIZE 8U  /**< Block size for DSP vectorization */

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Calculate subpixel center using gray moment method
 * @param grayscale_img Source grayscale image
 * @param width Image width
 * @param height Image height
 * @param component Connected component to process
 * @param result Subpixel result structure
 * @retval None
 */
void ImgProc_SubpixelGrayMoment(const uint8_t *grayscale_img,
                                 uint16_t width, uint16_t height,
                                 const MarkDetect_ComponentTypeDef *component,
                                 Subpixel_PointTypeDef *result);

/**
 * @brief Process all detected marks to get subpixel positions
 * @param grayscale_img Source grayscale image
 * @param detection_result Detection result from mark detection
 * @param subpixel_result Output subpixel result
 * @retval None
 */
void Subpixel_ProcessAll(const uint8_t *grayscale_img,
                         const MarkDetect_ResultTypeDef *detection_result,
                         Subpixel_ResultTypeDef *subpixel_result);

/**
 * @brief Calculate weighted centroid from gray moments
 * @param img Source image
 * @param width Image width for proper pixel indexing
 * @param x_min Left boundary
 * @param y_min Top boundary
 * @param x_max Right boundary
 * @param y_max Bottom boundary
 * @retval Subpixel_PointTypeDef with subpixel coordinates
 */
Subpixel_PointTypeDef Subpixel_CalculateWeightedCentroid(
    const uint8_t *img,
    uint16_t width,
    uint16_t x_min, uint16_t y_min,
    uint16_t x_max, uint16_t y_max);

/* DSP-accelerated functions ------------------------------------------------*/
/**
 * @brief Calculate subpixel center using DSP-accelerated gray moment
 * @param grayscale_img_16b Source grayscale image (16-bit for NIR mode)
 * @param width Image width
 * @param height Image height
 * @param component Connected component to process
 * @param result Subpixel result structure
 * @retval None
 * @note  Uses ARM CMSIS DSP for parallel moment calculation
 */
void ImgProc_SubpixelGrayMoment_DSP(const uint16_t *grayscale_img_16b,
                                     uint16_t width, uint16_t height,
                                     const MarkDetect_ComponentTypeDef *component,
                                     Subpixel_PointTypeDef *result);

/**
 * @brief Process all detected marks with DSP acceleration
 * @param grayscale_img_16b Source grayscale image (16-bit)
 * @param detection_result Detection result from mark detection
 * @param subpixel_result Output subpixel result
 * @retval None
 */
void Subpixel_ProcessAll_DSP(const uint16_t *grayscale_img_16b,
                             const MarkDetect_ResultTypeDef *detection_result,
                             Subpixel_ResultTypeDef *subpixel_result);

/**
 * @brief Get last processing cycle count
 * @retval Cycle count of last DSP operation
 */
uint32_t Subpixel_GetLastProcessCycles(void);

/**
 * @brief Check if DSP is available
 * @retval 1 if DSP is available, 0 otherwise
 */
uint8_t Subpixel_IsDSPAvailable(void);

#ifdef __cplusplus
}
#endif

#endif /* __SUBPIXEL_CENTER_H */