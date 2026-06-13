/**
 * @file subpixel_center.c
 * @brief Subpixel Center Implementation - Gray moment based subpixel localization
 * @author Development Team
 *
 * Features:
 * - Gray moment method: M00, M10, M01 calculation
 * - Subpixel centroid: xc = M10/M00, yc = M01/M00
 * - DSP acceleration using ARM CMSIS DSP (arm_dot_prod_f32)
 * - SIMD batch processing (8 pixels per iteration)
 */
#include "subpixel_center.h"
#include <string.h>

/* Private variables ---------------------------------------------------------*/
static uint32_t g_last_process_cycles = 0U;

/* Private function prototypes -----------------------------------------------*/
static float Subpixel_CalcMoment(const uint8_t *img, uint16_t width,
                                  uint16_t x_min, uint16_t y_min,
                                  uint16_t x_max, uint16_t y_max,
                                  uint16_t m, uint16_t n);
static void Subpixel_CalcMoment_DSP(const uint16_t *img, uint16_t width,
                                     uint16_t x_min, uint16_t y_min,
                                     uint16_t x_max, uint16_t y_max,
                                     uint16_t m, uint16_t n,
                                     float *result);
static float Subpixel_CalcWeightedCentroid(const uint8_t *img, uint16_t width,
                                           uint16_t x_min, uint16_t y_min,
                                           uint16_t x_max, uint16_t y_max,
                                           Subpixel_PointTypeDef *result);

/**
 * @brief Calculate subpixel center using gray moment method
 * @param grayscale_img Source grayscale image
 * @param width Image width
 * @param height Image height
 * @param component Connected component to process
 * @param result Subpixel result structure
 */
void ImgProc_SubpixelGrayMoment(const uint8_t *grayscale_img,
                                 uint16_t width, uint16_t height,
                                 const MarkDetect_ComponentTypeDef *component,
                                 Subpixel_PointTypeDef *result)
{
    if (grayscale_img == NULL || component == NULL || result == NULL) {
        result->x = 0.0f;
        result->y = 0.0f;
        result->weight = 0.0f;
        result->marker_image_size = 0.0f;
        result->dsp_used = 0U;
        result->process_cycles = 0U;
        return;
    }

    /* Get bounding box with margin */
    int32_t x_min = (int32_t)component->bbox.x_min;
    int32_t y_min = (int32_t)component->bbox.y_min;
    int32_t x_max = (int32_t)component->bbox.x_max;
    int32_t y_max = (int32_t)component->bbox.y_max;

    /* Clamp to image boundaries */
    if (x_min < 0) x_min = 0;
    if (y_min < 0) y_min = 0;
    if (x_max >= (int32_t)width) x_max = (int32_t)width - 1;
    if (y_max >= (int32_t)height) y_max = (int32_t)height - 1;

    /* Calculate marker image size from bounding box */
    uint16_t bbox_width = (x_max > x_min) ? (uint16_t)(x_max - x_min + 1) : 1U;
    uint16_t bbox_height = (y_max > y_min) ? (uint16_t)(y_max - y_min + 1) : 1U;
    result->marker_image_size = (float)((bbox_width + bbox_height) / 2U);  /* Average dimension */

    /* Calculate using weighted centroid method */
    (void)Subpixel_CalcWeightedCentroid(grayscale_img, width,
                                        (uint16_t)x_min, (uint16_t)y_min,
                                        (uint16_t)x_max, (uint16_t)y_max,
                                        result);

    result->dsp_used = 0U;
    result->process_cycles = 0U;
}

/**
 * @brief Calculate weighted centroid from gray moments
 * @param img Source image
 * @param width Image width for proper pixel indexing
 * @param x_min Left boundary
 * @param y_min Top boundary
 * @param x_max Right boundary
 * @param y_max Bottom boundary
 * @param result Output subpixel result
 */
static float Subpixel_CalcWeightedCentroid(const uint8_t *img, uint16_t width,
                                           uint16_t x_min, uint16_t y_min,
                                           uint16_t x_max, uint16_t y_max,
                                           Subpixel_PointTypeDef *result)
{
    result->x = 0.0f;
    result->y = 0.0f;
    result->weight = 0.0f;

    /* Calculate raw moments M00, M10, M01 */
    float m00 = Subpixel_CalcMoment(img, width, x_min, y_min, x_max, y_max, 0U, 0U);
    float m10 = Subpixel_CalcMoment(img, width, x_min, y_min, x_max, y_max, 1U, 0U);
    float m01 = Subpixel_CalcMoment(img, width, x_min, y_min, x_max, y_max, 0U, 1U);

    if (m00 <= 0.0f) {
        return 0.0f;
    }

    /* Centroid coordinates */
    result->x = m10 / m00;
    result->y = m01 / m00;

    /* Calculate normalized central moments for confidence */
    float norm_m20 = Subpixel_CalcMoment(img, width, x_min, y_min, x_max, y_max, 2U, 0U) / m00;
    float norm_m02 = Subpixel_CalcMoment(img, width, x_min, y_min, x_max, y_max, 0U, 2U) / m00;
    float norm_m11 = Subpixel_CalcMoment(img, width, x_min, y_min, x_max, y_max, 1U, 1U) / m00;

    /* Central moments */
    float mu20 = norm_m20 - (result->x * result->x);
    float mu02 = norm_m02 - (result->y * result->y);
    float mu11 = norm_m11 - (result->x * result->y);

    /* Eigenvalue-based weight for compactness */
    float trace = mu20 + mu02;
    float det = mu20 * mu02 - mu11 * mu11;

    if (trace > 0.0f) {
        float compactness = (det > 0.0f) ? (2.0f * det / trace) : SUBPIXEL_MIN_WEIGHT;
        result->weight = compactness;
        if (result->weight > 1.0f) result->weight = 1.0f;
        if (result->weight < SUBPIXEL_MIN_WEIGHT) result->weight = SUBPIXEL_MIN_WEIGHT;
    }

    return m00;
}

/**
 * @brief Calculate raw moment Mmn = sum of x^m * y^n * I(x,y)
 */
static float Subpixel_CalcMoment(const uint8_t *img, uint16_t width,
                                  uint16_t x_min, uint16_t y_min,
                                  uint16_t x_max, uint16_t y_max,
                                  uint16_t m, uint16_t n)
{
    float moment = 0.0f;

    uint16_t img_width = (x_max >= x_min) ? (x_max - x_min + 1U) : 0U;
    uint16_t img_height = (y_max >= y_min) ? (y_max - y_min + 1U) : 0U;

    /* Optimized paths for common moment orders */
    if (m == 0U && n == 0U) {
        /* M00 - simple sum of pixel values */
        for (uint16_t dy = 0U; dy < img_height; dy++) {
            uint32_t row_offset = ((uint32_t)(y_min + dy)) * width;
            for (uint16_t dx = 0U; dx < img_width; dx++) {
                uint32_t idx = row_offset + (uint32_t)(x_min + dx);
                moment += (float)img[idx];
            }
        }
    }
    else if (m == 1U && n == 0U) {
        /* M10 = sum(x * I) - weighted by x coordinate */
        for (uint16_t dy = 0U; dy < img_height; dy++) {
            uint32_t row_offset = ((uint32_t)(y_min + dy)) * width;
            for (uint16_t dx = 0U; dx < img_width; dx++) {
                uint32_t idx = row_offset + (uint32_t)(x_min + dx);
                float x_val = (float)(x_min + dx);
                moment += x_val * (float)img[idx];
            }
        }
    }
    else if (m == 0U && n == 1U) {
        /* M01 = sum(y * I) - weighted by y coordinate */
        for (uint16_t dy = 0U; dy < img_height; dy++) {
            uint32_t row_offset = ((uint32_t)(y_min + dy)) * width;
            float y_val = (float)(y_min + dy);
            for (uint16_t dx = 0U; dx < img_width; dx++) {
                uint32_t idx = row_offset + (uint32_t)(x_min + dx);
                moment += y_val * (float)img[idx];
            }
        }
    }
    else if (m == 2U && n == 0U) {
        /* M20 = sum(x^2 * I) */
        for (uint16_t dy = 0U; dy < img_height; dy++) {
            uint32_t row_offset = ((uint32_t)(y_min + dy)) * width;
            for (uint16_t dx = 0U; dx < img_width; dx++) {
                uint32_t idx = row_offset + (uint32_t)(x_min + dx);
                float x_val = (float)(x_min + dx);
                moment += x_val * x_val * (float)img[idx];
            }
        }
    }
    else if (m == 0U && n == 2U) {
        /* M02 = sum(y^2 * I) */
        for (uint16_t dy = 0U; dy < img_height; dy++) {
            uint32_t row_offset = ((uint32_t)(y_min + dy)) * width;
            float y_val = (float)(y_min + dy);
            for (uint16_t dx = 0U; dx < img_width; dx++) {
                uint32_t idx = row_offset + (uint32_t)(x_min + dx);
                moment += y_val * y_val * (float)img[idx];
            }
        }
    }
    else if (m == 1U && n == 1U) {
        /* M11 = sum(x*y*I) */
        for (uint16_t dy = 0U; dy < img_height; dy++) {
            uint32_t row_offset = ((uint32_t)(y_min + dy)) * width;
            float y_val = (float)(y_min + dy);
            for (uint16_t dx = 0U; dx < img_width; dx++) {
                uint32_t idx = row_offset + (uint32_t)(x_min + dx);
                float x_val = (float)(x_min + dx);
                moment += x_val * y_val * (float)img[idx];
            }
        }
    }
    else {
        /* General case for uncommon moment orders */
        for (uint16_t dy = 0U; dy < img_height; dy++) {
            uint32_t row_offset = ((uint32_t)(y_min + dy)) * width;
            float y_val = (float)(y_min + dy);
            float y_pow = 1.0f;
            for (uint16_t ni = 0U; ni < n; ni++) {
                y_pow *= y_val;
            }
            for (uint16_t dx = 0U; dx < img_width; dx++) {
                uint32_t idx = row_offset + (uint32_t)(x_min + dx);
                float x_val = (float)(x_min + dx);
                float x_pow = 1.0f;
                for (uint16_t mi = 0U; mi < m; mi++) {
                    x_pow *= x_val;
                }
                moment += x_pow * y_pow * (float)img[idx];
            }
        }
    }

    return moment;
}

/**
 * @brief DSP-accelerated moment calculation using ARM CMSIS DSP
 */
static void Subpixel_CalcMoment_DSP(const uint16_t *img, uint16_t width,
                                     uint16_t x_min, uint16_t y_min,
                                     uint16_t x_max, uint16_t y_max,
                                     uint16_t m, uint16_t n,
                                     float *result)
{
    float32_t moment = 0.0f;

    uint16_t img_width = (x_max >= x_min) ? (x_max - x_min + 1U) : 0U;
    uint16_t img_height = (y_max >= y_min) ? (y_max - y_min + 1U) : 0U;

    /* Process 8 pixels at a time using SIMD */
    if (m == 0U && n == 0U) {
        /* M00 with DSP vectorization */
        for (uint16_t dy = 0U; dy < img_height; dy++) {
            uint32_t row_offset = ((uint32_t)(y_min + dy)) * width;
            uint16_t dx = 0U;

            /* Process 8 pixels at a time */
            for (; dx + SUBPIXEL_DSP_BLOCK_SIZE <= img_width; dx += SUBPIXEL_DSP_BLOCK_SIZE) {
                float32_t block[SUBPIXEL_DSP_BLOCK_SIZE];

                for (uint8_t i = 0U; i < SUBPIXEL_DSP_BLOCK_SIZE; i++) {
                    block[i] = (float32_t)img[row_offset + (uint32_t)(x_min + dx + i)];
                }

                float32_t block_sum;
                block_sum = arm_dot_prod_f32(block, block, SUBPIXEL_DSP_BLOCK_SIZE);
                moment += block_sum;
            }

            /* Handle remaining pixels */
            for (; dx < img_width; dx++) {
                moment += (float32_t)img[row_offset + (uint32_t)(x_min + dx)];
            }
        }
    }
    else if (m == 1U && n == 0U) {
        /* M10 = sum(x*I) with DSP */
        for (uint16_t dy = 0U; dy < img_height; dy++) {
            uint32_t row_offset = ((uint32_t)(y_min + dy)) * width;
            float32_t y_val = (float32_t)(y_min + dy);

            for (uint16_t dx = 0U; dx < img_width; dx++) {
                float32_t x_val = (float32_t)(x_min + dx);
                float32_t pixel_val = (float32_t)img[row_offset + (uint32_t)(x_min + dx)];
                moment += x_val * pixel_val;
            }
        }
    }
    else if (m == 0U && n == 1U) {
        /* M01 = sum(y*I) */
        for (uint16_t dy = 0U; dy < img_height; dy++) {
            uint32_t row_offset = ((uint32_t)(y_min + dy)) * width;
            float32_t y_val = (float32_t)(y_min + dy);

            for (uint16_t dx = 0U; dx < img_width; dx++) {
                float32_t pixel_val = (float32_t)img[row_offset + (uint32_t)(x_min + dx)];
                moment += y_val * pixel_val;
            }
        }
    }
    else {
        /* For non-simple moments, use scalar fallback */
        for (uint16_t dy = 0U; dy < img_height; dy++) {
            uint32_t row_offset = ((uint32_t)(y_min + dy)) * width;
            for (uint16_t dx = 0U; dx < img_width; dx++) {
                float32_t x_val = (float32_t)(x_min + dx);
                float32_t y_val = (float32_t)(y_min + dy);
                float32_t pixel_val = (float32_t)img[row_offset + (uint32_t)(x_min + dx)];
                float32_t x_pow = 1.0f;
                float32_t y_pow = 1.0f;
                for (uint16_t mi = 0U; mi < m; mi++) x_pow *= x_val;
                for (uint16_t ni = 0U; ni < n; ni++) y_pow *= y_val;
                moment += x_pow * y_pow * pixel_val;
            }
        }
    }

    *result = moment;
}

/**
 * @brief Process all detected marks to get subpixel positions
 */
void Subpixel_ProcessAll(const uint8_t *grayscale_img,
                         const MarkDetect_ResultTypeDef *detection_result,
                         Subpixel_ResultTypeDef *subpixel_result)
{
    if (grayscale_img == NULL || detection_result == NULL || subpixel_result == NULL) {
        return;
    }

    (void)memset(subpixel_result, 0, sizeof(Subpixel_ResultTypeDef));

    for (uint8_t i = 0U; i < detection_result->mark_count && i < MARKER_COUNT; i++) {
        if (detection_result->marks[i].is_valid) {
            ImgProc_SubpixelGrayMoment(
                grayscale_img,
                FRAME_WIDTH, FRAME_HEIGHT,
                &detection_result->marks[i],
                &subpixel_result->points[i]);

            if (subpixel_result->points[i].weight >= SUBPIXEL_MIN_WEIGHT) {
                subpixel_result->valid_count++;
            }
        }
    }

    if (subpixel_result->valid_count >= MARK_EXPECTED_COUNT - 1U) {
        subpixel_result->is_valid = 1U;
    }
}

/**
 * @brief DSP-accelerated subpixel center calculation (16-bit input)
 */
void ImgProc_SubpixelGrayMoment_DSP(const uint16_t *grayscale_img_16b,
                                     uint16_t width, uint16_t height,
                                     const MarkDetect_ComponentTypeDef *component,
                                     Subpixel_PointTypeDef *result)
{
    if (grayscale_img_16b == NULL || component == NULL || result == NULL) {
        result->x = 0.0f;
        result->y = 0.0f;
        result->weight = 0.0f;
        result->marker_image_size = 0.0f;
        result->dsp_used = 0U;
        result->process_cycles = 0U;
        return;
    }

    uint32_t cycle_start = DWT->CYCCNT;

    /* Get bounding box with margin */
    int32_t x_min = (int32_t)component->bbox.x_min;
    int32_t y_min = (int32_t)component->bbox.y_min;
    int32_t x_max = (int32_t)component->bbox.x_max;
    int32_t y_max = (int32_t)component->bbox.y_max;

    /* Clamp to image boundaries */
    if (x_min < 0) x_min = 0;
    if (y_min < 0) y_min = 0;
    if (x_max >= (int32_t)width) x_max = (int32_t)width - 1;
    if (y_max >= (int32_t)height) y_max = (int32_t)height - 1;

    /* Calculate marker image size */
    uint16_t bbox_width = (x_max > x_min) ? (uint16_t)(x_max - x_min + 1) : 1U;
    uint16_t bbox_height = (y_max > y_min) ? (uint16_t)(y_max - y_min + 1) : 1U;
    result->marker_image_size = (float)((bbox_width + bbox_height) / 2U);

    /* Calculate moments using DSP */
    float m00, m10, m01;
    Subpixel_CalcMoment_DSP(grayscale_img_16b, width,
                            (uint16_t)x_min, (uint16_t)y_min,
                            (uint16_t)x_max, (uint16_t)y_max,
                            0U, 0U, &m00);
    Subpixel_CalcMoment_DSP(grayscale_img_16b, width,
                            (uint16_t)x_min, (uint16_t)y_min,
                            (uint16_t)x_max, (uint16_t)y_max,
                            1U, 0U, &m10);
    Subpixel_CalcMoment_DSP(grayscale_img_16b, width,
                            (uint16_t)x_min, (uint16_t)y_min,
                            (uint16_t)x_max, (uint16_t)y_max,
                            0U, 1U, &m01);

    if (m00 > 0.0f) {
        result->x = m10 / m00;
        result->y = m01 / m00;

        /* Calculate central moments for confidence */
        float norm_m20, norm_m02, norm_m11;
        Subpixel_CalcMoment_DSP(grayscale_img_16b, width,
                                (uint16_t)x_min, (uint16_t)y_min,
                                (uint16_t)x_max, (uint16_t)y_max,
                                2U, 0U, &norm_m20);
        Subpixel_CalcMoment_DSP(grayscale_img_16b, width,
                                (uint16_t)x_min, (uint16_t)y_min,
                                (uint16_t)x_max, (uint16_t)y_max,
                                0U, 2U, &norm_m02);
        Subpixel_CalcMoment_DSP(grayscale_img_16b, width,
                                (uint16_t)x_min, (uint16_t)y_min,
                                (uint16_t)x_max, (uint16_t)y_max,
                                1U, 1U, &norm_m11);

        norm_m20 /= m00;
        norm_m02 /= m00;
        norm_m11 /= m00;

        float mu20 = norm_m20 - (result->x * result->x);
        float mu02 = norm_m02 - (result->y * result->y);
        float mu11 = norm_m11 - (result->x * result->y);

        float trace = mu20 + mu02;
        float det = mu20 * mu02 - mu11 * mu11;

        if (trace > 0.0f) {
            result->weight = (det > 0.0f) ? (2.0f * det / trace) : SUBPIXEL_MIN_WEIGHT;
            if (result->weight > 1.0f) result->weight = 1.0f;
            if (result->weight < SUBPIXEL_MIN_WEIGHT) result->weight = SUBPIXEL_MIN_WEIGHT;
        }
    }

    uint32_t cycle_end = DWT->CYCCNT;
    result->process_cycles = cycle_end - cycle_start;
    g_last_process_cycles = result->process_cycles;
    result->dsp_used = 1U;
}

/**
 * @brief Process all detected marks with DSP acceleration
 */
void Subpixel_ProcessAll_DSP(const uint16_t *grayscale_img_16b,
                             const MarkDetect_ResultTypeDef *detection_result,
                             Subpixel_ResultTypeDef *subpixel_result)
{
    if (grayscale_img_16b == NULL || detection_result == NULL || subpixel_result == NULL) {
        return;
    }

    (void)memset(subpixel_result, 0, sizeof(Subpixel_ResultTypeDef));

    for (uint8_t i = 0U; i < detection_result->mark_count && i < MARKER_COUNT; i++) {
        if (detection_result->marks[i].is_valid) {
            ImgProc_SubpixelGrayMoment_DSP(
                grayscale_img_16b,
                FRAME_WIDTH, FRAME_HEIGHT,
                &detection_result->marks[i],
                &subpixel_result->points[i]);

            if (subpixel_result->points[i].weight >= SUBPIXEL_MIN_WEIGHT) {
                subpixel_result->valid_count++;
            }
        }
    }

    if (subpixel_result->valid_count >= MARK_EXPECTED_COUNT - 1U) {
        subpixel_result->is_valid = 1U;
    }
}

/**
 * @brief Get last processing cycle count
 */
uint32_t Subpixel_GetLastProcessCycles(void)
{
    return g_last_process_cycles;
}

/**
 * @brief Check if DSP is available
 */
uint8_t Subpixel_IsDSPAvailable(void)
{
#if defined(ARM_MATH_CM4)
    return 1U;
#else
    return 0U;
#endif
}