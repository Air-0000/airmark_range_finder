/**
 * @file mark_detection.c
 * @brief Mark Detection Implementation - Binarization and connected component analysis
 * @author Development Team
 *
 * Features:
 * - Fixed threshold binarization (default 25) with Otsu option
 * - Packed binary output (4 pixels per byte)
 * - Two-pass connected component analysis with Union-Find
 * - Area filtering: 10-50000 pixels
 */
#include "mark_detection.h"
#include <string.h>

/* Private variables ---------------------------------------------------------*/
static uint8_t g_binarize_threshold = BIN_THRESHOLD_DEFAULT;
static uint8_t g_use_two_pass = 1;

/* Labeling buffer for connected components */
static uint16_t g_label_buffer[FRAME_HEIGHT][FRAME_WIDTH];

/* Union-Find parent array for label equivalence resolution */
static uint16_t g_label_parent[256];
static uint16_t g_label_rank[256];

/* Result buffer for component statistics */
static uint32_t g_area_buf[256];
static uint32_t g_x_sum_buf[256];
static uint32_t g_y_sum_buf[256];
static uint16_t g_x_min_buf[256];
static uint16_t g_x_max_buf[256];
static uint16_t g_y_min_buf[256];
static uint16_t g_y_max_buf[256];

/**
 * @brief Initialize mark detection module
 */
void ImgProc_Init(void)
{
    g_binarize_threshold = BIN_THRESHOLD_DEFAULT;
    g_use_two_pass = 1;

    /* Clear label buffer */
    (void)memset(g_label_buffer, 0, sizeof(g_label_buffer));

    /* Initialize Union-Find structures */
    (void)memset(g_label_parent, 0, sizeof(g_label_parent));
    (void)memset(g_label_rank, 0, sizeof(g_label_rank));
    (void)memset(g_area_buf, 0, sizeof(g_area_buf));
}

/**
 * @brief Find root with path compression for Union-Find
 */
static uint16_t Find_Set(uint16_t label)
{
    /* Path compression */
    if (g_label_parent[label] != label) {
        g_label_parent[label] = Find_Set(g_label_parent[label]);
    }
    return g_label_parent[label];
}

/**
 * @brief Union two labels by rank
 */
static void Union_Set(uint16_t label1, uint16_t label2)
{
    uint16_t root1 = Find_Set(label1);
    uint16_t root2 = Find_Set(label2);

    if (root1 == root2) {
        return;  /* Already in same set */
    }

    /* Union by rank - attach smaller rank to larger rank */
    if (g_label_rank[root1] < g_label_rank[root2]) {
        g_label_parent[root1] = root2;
    } else if (g_label_rank[root1] > g_label_rank[root2]) {
        g_label_parent[root2] = root1;
    } else {
        /* Same rank - choose one and increment rank */
        g_label_parent[root2] = root1;
        g_label_rank[root1]++;
    }
}

/**
 * @brief Binarize input image with fixed threshold
 * @param src Source image buffer (grayscale 8-bit)
 * @param dst Destination buffer (binary, 1-bit per pixel, 4 pixels packed into 1 byte)
 * @param width Image width
 * @param height Image height
 * @param threshold Binarization threshold (20-30)
 * @retval None
 *
 * @note Output format: 4 pixels per byte, bit 0 = leftmost pixel
 *       Pixel at (x,y) -> byte at (y * (width/4) + x/4), bit at (x%4)*2
 */
void ImgProc_Binarize(const uint8_t *src, uint8_t *dst,
                      uint16_t width, uint16_t height,
                      uint8_t threshold)
{
    /* Clamp threshold to valid range */
    if (threshold < BIN_THRESHOLD_MIN) {
        threshold = BIN_THRESHOLD_MIN;
    }
    if (threshold > BIN_THRESHOLD_MAX) {
        threshold = BIN_THRESHOLD_MAX;
    }
    g_binarize_threshold = threshold;

    /* Calculate packed width (each byte holds 4 pixels) */
    uint16_t packed_width = (width + 3U) / 4U;

    /* Clear output buffer first */
    (void)memset(dst, 0, (size_t)(packed_width * height));

    /* Binarize and pack */
    for (uint16_t y = 0U; y < height; y++) {
        for (uint16_t x = 0U; x < width; x++) {
            uint32_t src_idx = (uint32_t)y * width + x;
            uint32_t dst_idx = (uint32_t)y * packed_width + (x / 4U);
            uint8_t pixel_bit = (x % 4U) * 2U;  /* 0, 2, 4, 6 for bits */

            /* Set bit if pixel exceeds threshold */
            if (src[src_idx] > threshold) {
                dst[dst_idx] |= (1U << pixel_bit);
            }
        }
    }
}

/**
 * @brief Unpack a binary image pixel (for component analysis)
 * @param binary Packed binary image
 * @param width Image width
 * @param x Pixel x coordinate
 * @param y Pixel y coordinate
 * @retval 1 if pixel is foreground, 0 otherwise
 */
static uint8_t UnpackPixel(const uint8_t *binary, uint16_t width, uint16_t x, uint16_t y)
{
    uint16_t packed_width = (width + 3U) / 4U;
    uint32_t byte_idx = (uint32_t)y * packed_width + (x / 4U);
    uint8_t pixel_bit = (x % 4U) * 2U;  /* 0, 2, 4, 6 for bits */
    return (binary[byte_idx] >> pixel_bit) & 1U;
}

/**
 * @brief Find connected components in binary image using two-pass algorithm
 * @param binary Binary image buffer (packed 4 pixels per byte)
 * @param width Image width
 * @param height Image height
 * @param result Detection result structure
 * @retval MARKDET_STATUS_OK if marks found
 */
MarkDetect_StatusTypeDef ImgProc_FindConnectedComponents(
    const uint8_t *binary,
    uint16_t width, uint16_t height,
    MarkDetect_ResultTypeDef *result)
{
    if (binary == NULL || result == NULL) {
        return MARKDET_STATUS_ERROR;
    }

    /* Reset result */
    (void)memset(result, 0, sizeof(MarkDetect_ResultTypeDef));

    /* Clear label buffer and init Union-Find */
    (void)memset(g_label_buffer, 0, sizeof(g_label_buffer));
    for (uint16_t i = 0U; i < 256U; i++) {
        g_label_parent[i] = i;
        g_label_rank[i] = 0U;
    }

    /* Clear statistics buffers */
    (void)memset(g_area_buf, 0, sizeof(g_area_buf));
    (void)memset(g_x_sum_buf, 0, sizeof(g_x_sum_buf));
    (void)memset(g_y_sum_buf, 0, sizeof(g_y_sum_buf));

    /* Initialize bounds to max values */
    for (uint16_t i = 0U; i < 256U; i++) {
        g_x_min_buf[i] = width - 1U;
        g_y_min_buf[i] = height - 1U;
        g_x_max_buf[i] = 0U;
        g_y_max_buf[i] = 0U;
    }

    uint16_t next_label = 1U;  /* Labels start from 1 */

    /* ===== PASS 1: Assign provisional labels and track equivalences ===== */
    for (uint16_t y = 0U; y < height; y++) {
        for (uint16_t x = 0U; x < width; x++) {
            /* Check if foreground */
            if (!UnpackPixel(binary, width, x, y)) {
                continue;
            }

            /* Get neighbor labels (8-connectivity) */
            uint16_t neighbors[4] = {0U, 0U, 0U, 0U};  /* top, left, top-left, top-right */
            uint8_t neighbor_count = 0U;

            /* Top neighbor */
            if (y > 0U && UnpackPixel(binary, width, x, (uint16_t)(y - 1U))) {
                neighbors[neighbor_count++] = g_label_buffer[y - 1U][x];
            }
            /* Left neighbor */
            if (x > 0U && UnpackPixel(binary, width, (uint16_t)(x - 1U), y)) {
                neighbors[neighbor_count++] = g_label_buffer[y][x - 1U];
            }
            /* Top-left neighbor */
            if (y > 0U && x > 0U && UnpackPixel(binary, width, (uint16_t)(x - 1U), (uint16_t)(y - 1U))) {
                neighbors[neighbor_count++] = g_label_buffer[y - 1U][x - 1U];
            }
            /* Top-right neighbor */
            if (y > 0U && x < (width - 1U) && UnpackPixel(binary, width, (uint16_t)(x + 1U), (uint16_t)(y - 1U))) {
                neighbors[neighbor_count++] = g_label_buffer[y - 1U][x + 1U];
            }

            if (neighbor_count == 0U) {
                /* No neighbors - assign new label */
                if (next_label < 255U) {
                    g_label_buffer[y][x] = next_label;
                    next_label++;
                }
            } else {
                /* Find minimum neighbor label */
                uint16_t min_label = neighbors[0];
                for (uint8_t n = 1U; n < neighbor_count; n++) {
                    if (neighbors[n] < min_label) {
                        min_label = neighbors[n];
                    }
                }

                /* Assign minimum label */
                g_label_buffer[y][x] = min_label;

                /* Union all distinct neighbor labels with the minimum */
                for (uint8_t n = 0U; n < neighbor_count; n++) {
                    if (neighbors[n] != min_label) {
                        Union_Set(min_label, neighbors[n]);
                    }
                }
            }
        }
    }

    /* ===== PASS 2: Resolve equivalences, collect statistics ===== */

    /* Map provisional labels to canonical labels */
    uint16_t label_map[256] = {0U};

    /* Find canonical root for each label */
    for (uint16_t label = 1U; label < next_label && label < 256U; label++) {
        label_map[label] = Find_Set(label);
    }

    /* Accumulate statistics using canonical labels */
    for (uint16_t y = 0U; y < height; y++) {
        for (uint16_t x = 0U; x < width; x++) {
            uint16_t provisional = g_label_buffer[y][x];
            if (provisional == 0U) {
                continue;
            }

            uint16_t canonical = label_map[provisional];

            /* Accumulate statistics */
            g_area_buf[canonical]++;
            g_x_sum_buf[canonical] += x;
            g_y_sum_buf[canonical] += y;

            /* Update bounds */
            if (x < g_x_min_buf[canonical]) g_x_min_buf[canonical] = x;
            if (x > g_x_max_buf[canonical]) g_x_max_buf[canonical] = x;
            if (y < g_y_min_buf[canonical]) g_y_min_buf[canonical] = y;
            if (y > g_y_max_buf[canonical]) g_y_max_buf[canonical] = y;
        }
    }

    /* ===== Extract valid marks within area constraints (10-50000 pixels) ===== */
    uint8_t mark_idx = 0U;

    for (uint16_t label = 1U; label < 256U && mark_idx < MARKER_COUNT; label++) {
        uint32_t area = g_area_buf[label];

        /* Filter by area: 10 <= area <= 50000 */
        if (area < MARK_MIN_AREA || area > 50000U) {
            continue;
        }

        /* Valid mark found */
        result->marks[mark_idx].label = label;
        result->marks[mark_idx].area = area;
        result->marks[mark_idx].bbox.x_min = g_x_min_buf[label];
        result->marks[mark_idx].bbox.y_min = g_y_min_buf[label];
        result->marks[mark_idx].bbox.x_max = g_x_max_buf[label];
        result->marks[mark_idx].bbox.y_max = g_y_max_buf[label];

        /* Integer centroid */
        result->marks[mark_idx].centroid_x = (uint16_t)(g_x_sum_buf[label] / area);
        result->marks[mark_idx].centroid_y = (uint16_t)(g_y_sum_buf[label] / area);
        result->marks[mark_idx].is_valid = 1U;

        mark_idx++;
    }

    result->mark_count = mark_idx;

    /* Determine validity */
    if (mark_idx > 0U && mark_idx <= MARKER_COUNT) {
        result->is_valid = 1U;
        return MARKDET_STATUS_OK;
    } else if (mark_idx == 0U) {
        return MARKDET_STATUS_NO_MARKS;
    } else {
        return MARKDET_STATUS_TOO_MANY_MARKS;
    }
}

/**
 * @brief Validate detected marks against expected count and size
 * @param result Detection result to validate
 * @retval 1 if valid, 0 otherwise
 */
uint8_t ImgProc_ValidateResult(const MarkDetect_ResultTypeDef *result)
{
    if (result == NULL) {
        return 0U;
    }

    /* Must have at least 1 mark and at most MARKER_COUNT */
    if (result->mark_count == 0U || result->mark_count > MARKER_COUNT) {
        return 0U;
    }

    /* All marks must be marked as valid */
    for (uint8_t i = 0U; i < result->mark_count; i++) {
        if (!result->marks[i].is_valid) {
            return 0U;
        }

        /* Check area constraints: 10 <= area <= 50000 */
        if (result->marks[i].area < MARK_MIN_AREA || result->marks[i].area > 50000U) {
            return 0U;
        }
    }

    return 1U;
}

/**
 * @brief Set connected component algorithm
 */
void ImgProc_SetAlgorithm(uint8_t use_two_pass)
{
    g_use_two_pass = use_two_pass ? 1U : 0U;
}