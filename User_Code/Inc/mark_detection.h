/**
 * @file mark_detection.h
 * @brief Mark Detection Module - Binarization and connected component analysis
 * @author Development Team
 */
#ifndef __MARK_DETECTION_H
#define __MARK_DETECTION_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Mark detection status
 */
typedef enum {
    MARKDET_STATUS_OK = 0,
    MARKDET_STATUS_ERROR,
    MARKDET_STATUS_NO_MARKS,
    MARKDET_STATUS_TOO_MANY_MARKS
} MarkDetect_StatusTypeDef;

/**
 * @brief Bounding box structure
 */
typedef struct {
    uint16_t x_min;
    uint16_t y_min;
    uint16_t x_max;
    uint16_t y_max;
} MarkDetect_BoundingBoxTypeDef;

/**
 * @brief Connected component structure
 */
typedef struct {
    uint16_t label;
    uint32_t area;                      /**< Pixel count */
    MarkDetect_BoundingBoxTypeDef bbox; /**< Bounding box */
    uint16_t centroid_x;                /**< Centroid X (integer) */
    uint16_t centroid_y;                /**< Centroid Y (integer) */
    uint8_t is_valid;                   /**< Valid mark flag */
} MarkDetect_ComponentTypeDef;

/**
 * @brief Detection result structure
 */
typedef struct {
    MarkDetect_ComponentTypeDef marks[MARKER_COUNT];
    uint8_t mark_count;
    uint8_t is_valid;
} MarkDetect_ResultTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Binarization threshold range */
#define BIN_THRESHOLD_MIN    20U
#define BIN_THRESHOLD_MAX    30U
#define BIN_THRESHOLD_DEFAULT 25U

/* Mark size constraints */
#define MARK_MIN_AREA        10U     /**< Minimum mark area in pixels */
#define MARK_MAX_AREA        20U     /**< Maximum mark area in pixels */
#define MARK_EXPECTED_COUNT  5U      /**< Expected number of markers */

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialize mark detection module
 * @retval None
 */
void ImgProc_Init(void);

/**
 * @brief Binarize input image with fixed threshold
 * @param src Source image buffer (grayscale 8-bit)
 * @param dst Destination buffer (binary, 1-bit per pixel packed)
 * @param width Image width
 * @param height Image height
 * @param threshold Binarization threshold (20-30)
 * @retval None
 */
void ImgProc_Binarize(const uint8_t *src, uint8_t *dst,
                      uint16_t width, uint16_t height,
                      uint8_t threshold);

/**
 * @brief Find connected components in binary image
 * @param binary Binary image buffer
 * @param width Image width
 * @param height Image height
 * @param result Detection result structure
 * @retval MARKDET_STATUS_OK if marks found
 */
MarkDetect_StatusTypeDef ImgProc_FindConnectedComponents(
    const uint8_t *binary,
    uint16_t width, uint16_t height,
    MarkDetect_ResultTypeDef *result);

/**
 * @brief Validate detected marks against expected count and size
 * @param result Detection result to validate
 * @retval 1 if valid, 0 otherwise
 */
uint8_t ImgProc_ValidateResult(const MarkDetect_ResultTypeDef *result);

/* Algorithm selection -------------------------------------------------------*/
/**
 * @brief Set connected component algorithm
 * @param use_two_pass 1 for two-pass, 0 for single-pass
 * @retval None
 */
void ImgProc_SetAlgorithm(uint8_t use_two_pass);

#ifdef __cplusplus
}
#endif

#endif /* __MARK_DETECTION_H */