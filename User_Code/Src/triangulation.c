/**
 * @file triangulation.c
 * @brief Triangulation Implementation - Pinhole camera model depth calculation
 * @author Development Team
 *
 * Features:
 * - Pinhole model: Z = f * L / y (f=focal, L=baseline, y=image height)
 * - 3D position from subpixel coordinates
 * - Radial distortion correction (k1, k2)
 * - Camera intrinsic parameters
 */
#include "triangulation.h"

/* Private variables ---------------------------------------------------------*/
static float g_marker_size_m = MARKER_SIDE_LEN;  /* 0.05m baseline */
static float g_baseline_m = MARKER_SIDE_LEN;      /* Same as marker size for single camera */

static Triangulation_CameraParamsTypeDef g_cam_params = {
    .focal_length_x = TRI_DEFAULT_FOCAL_LENGTH,  /* 320 pixels */
    .focal_length_y = TRI_DEFAULT_FOCAL_LENGTH,
    .optical_center_x = TRI_CENTER_X,            /* 160 */
    .optical_center_y = TRI_CENTER_Y,            /* 120 */
    .distortion_k1 = 0.0f,
    .distortion_k2 = 0.0f
};

static uint8_t g_is_calibrated = 0U;

/* Private function prototypes -----------------------------------------------*/
static float Triangulation_CalcDepthByPinhole(float marker_image_size_px);
static void Triangulation_ApplyDistortion(float x, float y, float *dx, float *dy);

/**
 * @brief Initialize triangulation module
 */
void Triangulation_Init(void)
{
    g_marker_size_m = MARKER_SIDE_LEN;
    g_baseline_m = MARKER_SIDE_LEN;
    g_is_calibrated = 0U;

    g_cam_params.focal_length_x = TRI_DEFAULT_FOCAL_LENGTH;
    g_cam_params.focal_length_y = TRI_DEFAULT_FOCAL_LENGTH;
    g_cam_params.optical_center_x = TRI_CENTER_X;
    g_cam_params.optical_center_y = TRI_CENTER_Y;
    g_cam_params.distortion_k1 = 0.0f;
    g_cam_params.distortion_k2 = 0.0f;
}

/**
 * @brief Set marker physical size
 */
void Triangulation_SetMarkerSize(float size_m)
{
    if (size_m > 0.0f && size_m < 1.0f) {
        g_marker_size_m = size_m;
        g_baseline_m = size_m;  /* Baseline = marker size for reference */
    }
}

/**
 * @brief Calculate depth using pinhole model
 * @note Z = f * L / y
 *       f = focal length in pixels (default 320)
 *       L = baseline in meters (default 0.05m)
 *       y = marker image size in pixels
 * @param marker_image_size_pixels Marker size in image (pixels)
 * @retval Depth in meters
 */
float Triangulation_CalcDepth(float marker_image_size_pixels)
{
    return Triangulation_CalcDepthByPinhole(marker_image_size_pixels);
}

/**
 * @brief Internal depth calculation using pinhole model
 */
static float Triangulation_CalcDepthByPinhole(float marker_image_size_px)
{
    if (marker_image_size_px <= 0.0f) {
        return 0.0f;
    }

    /* Z = f * L / y */
    float depth = (g_cam_params.focal_length_x * g_baseline_m) / marker_image_size_px;

    /* Clamp to valid range */
    if (depth < TRI_MIN_DISTANCE) {
        depth = TRI_MIN_DISTANCE;
    } else if (depth > TRI_MAX_DISTANCE) {
        depth = TRI_MAX_DISTANCE;
    }

    return depth;
}

/**
 * @brief Apply lens distortion correction
 * @note Uses Brown-Conrady model:
 *       x_corrected = x + x * (k1 * r^2 + k2 * r^4)
 *       y_corrected = y + y * (k1 * r^2 + k2 * r^4)
 *       where r^2 = xn^2 + yn^2 (normalized coords from principal point)
 */
static void Triangulation_ApplyDistortion(float x, float y, float *dx, float *dy)
{
    if (dx == NULL || dy == NULL) {
        return;
    }

    *dx = 0.0f;
    *dy = 0.0f;

    /* Skip if no distortion */
    if (g_cam_params.distortion_k1 == 0.0f && g_cam_params.distortion_k2 == 0.0f) {
        return;
    }

    /* Normalized coordinates from optical center */
    float xn = (x - g_cam_params.optical_center_x) / g_cam_params.focal_length_x;
    float yn = (y - g_cam_params.optical_center_y) / g_cam_params.focal_length_y;

    float r2 = xn * xn + yn * yn;
    float r4 = r2 * r2;

    /* Radial distortion factor */
    float factor = 1.0f + g_cam_params.distortion_k1 * r2 + g_cam_params.distortion_k2 * r4;

    /* Calculate correction */
    *dx = (x - g_cam_params.optical_center_x) * (factor - 1.0f);
    *dy = (y - g_cam_params.optical_center_y) * (factor - 1.0f);
}

/**
 * @brief Calculate 3D position from subpixel coordinates
 */
Triangulation_StatusTypeDef Triangulation_Calc3DPosition(
    const Subpixel_ResultTypeDef *subpixel_result,
    Triangulation_ResultTypeDef *result)
{
    if (subpixel_result == NULL || result == NULL) {
        return TRI_STATUS_ERROR;
    }

    if (!subpixel_result->is_valid || subpixel_result->valid_count == 0U) {
        return TRI_STATUS_INVALID_PARAMS;
    }

    /* Reset result */
    for (uint8_t i = 0U; i < MARKER_COUNT; i++) {
        result->positions[i].x = 0.0f;
        result->positions[i].y = 0.0f;
        result->positions[i].z = 0.0f;
        result->distances[i] = 0.0f;
    }
    result->valid_count = 0U;
    result->is_valid = 0U;

    /* Process each valid subpixel point */
    for (uint8_t i = 0U; i < subpixel_result->valid_count && i < MARKER_COUNT; i++) {
        Subpixel_PointTypeDef pt = subpixel_result->points[i];

        /* Skip invalid points */
        if (pt.weight < SUBPIXEL_MIN_WEIGHT) {
            continue;
        }

        /* Apply lens distortion correction */
        float dx, dy;
        Triangulation_ApplyDistortion(pt.x, pt.y, &dx, &dy);
        float x_corrected = pt.x - dx;
        float y_corrected = pt.y - dy;

        /* Calculate depth using marker image size */
        float depth = Triangulation_CalcDepthByPinhole(pt.marker_image_size);

        if (depth <= 0.0f || depth > TRI_MAX_DISTANCE) {
            continue;
        }

        /* Convert to normalized camera coordinates
         * x_norm = (x_corrected - cx) / f
         * y_norm = (y_corrected - cy) / f
         */
        float x_norm = (x_corrected - g_cam_params.optical_center_x) / g_cam_params.focal_length_x;
        float y_norm = (y_corrected - g_cam_params.optical_center_y) / g_cam_params.focal_length_y;

        /* Scale normalized coordinates by depth to get 3D position
         * X = x_norm * depth
         * Y = y_norm * depth
         * Z = depth
         */
        result->positions[i].x = x_norm * depth;
        result->positions[i].y = y_norm * depth;
        result->positions[i].z = depth;
        result->distances[i] = depth;
        result->valid_count++;
    }

    /* Mark as valid if we have at least MARK_EXPECTED_COUNT - 1 valid points */
    if (result->valid_count >= MARK_EXPECTED_COUNT - 1U) {
        result->is_valid = 1U;
        return TRI_STATUS_OK;
    }

    return TRI_STATUS_ERROR;
}

/**
 * @brief Get camera intrinsic parameters
 */
void Triangulation_GetCameraParams(Triangulation_CameraParamsTypeDef *params)
{
    if (params != NULL) {
        params->focal_length_x = g_cam_params.focal_length_x;
        params->focal_length_y = g_cam_params.focal_length_y;
        params->optical_center_x = g_cam_params.optical_center_x;
        params->optical_center_y = g_cam_params.optical_center_y;
        params->distortion_k1 = g_cam_params.distortion_k1;
        params->distortion_k2 = g_cam_params.distortion_k2;
    }
}

/**
 * @brief Set camera intrinsic parameters
 */
void Triangulation_SetCameraParams(const Triangulation_CameraParamsTypeDef *params)
{
    if (params != NULL) {
        g_cam_params.focal_length_x = params->focal_length_x;
        g_cam_params.focal_length_y = params->focal_length_y;
        g_cam_params.optical_center_x = params->optical_center_x;
        g_cam_params.optical_center_y = params->optical_center_y;
        g_cam_params.distortion_k1 = params->distortion_k1;
        g_cam_params.distortion_k2 = params->distortion_k2;
        g_is_calibrated = 1U;
    }
}