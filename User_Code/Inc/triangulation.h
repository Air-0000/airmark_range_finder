/**
 * @file triangulation.h
 * @brief Triangulation Module - Pinhole camera model depth calculation
 * @author Development Team
 */
#ifndef __TRIANGULATION_H
#define __TRIANGULATION_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "subpixel_center.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Triangulation status
 */
typedef enum {
    TRI_STATUS_OK = 0,
    TRI_STATUS_ERROR,
    TRI_STATUS_INVALID_PARAMS,
    TRI_STATUS_CAMERA_NOT_CALIBRATED
} Triangulation_StatusTypeDef;

/**
 * @brief 3D position structure
 */
typedef struct {
    float x;   /**< X coordinate in meters */
    float y;   /**< Y coordinate in meters */
    float z;   /**< Z (depth) coordinate in meters */
} Triangulation_Point3DTypeDef;

/**
 * @brief Triangulation result structure
 */
typedef struct {
    Triangulation_Point3DTypeDef positions[MARKER_COUNT];
    float distances[MARKER_COUNT];    /**< Distance to each marker */
    uint8_t valid_count;
    uint8_t is_valid;
} Triangulation_ResultTypeDef;

/**
 * @brief Camera intrinsic parameters
 */
typedef struct {
    float focal_length_x;     /**< Focal length in pixels (X direction) */
    float focal_length_y;     /**< Focal length in pixels (Y direction) */
    float optical_center_x;   /**< Optical center X (principal point) */
    float optical_center_y;   /**< Optical center Y (principal point) */
    float distortion_k1;      /**< Radial distortion coefficient */
    float distortion_k2;      /**< Radial distortion coefficient */
} Triangulation_CameraParamsTypeDef;

/* Exported constants --------------------------------------------------------*/
#define TRI_DEFAULT_FOCAL_LENGTH   320.0f   /**< Default focal length in pixels */
#define TRI_CENTER_X               160.0f  /**< Image center X (320/2) */
#define TRI_CENTER_Y               120.0f  /**< Image center Y (240/2) */
#define TRI_MAX_DISTANCE           5.0f    /**< Maximum measurable distance in meters */
#define TRI_MIN_DISTANCE           0.1f    /**< Minimum measurable distance in meters */

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialize triangulation module
 * @retval None
 */
void Triangulation_Init(void);

/**
 * @brief Set marker physical size
 * @param size_m Marker size in meters (e.g., 0.05 for 5cm)
 * @retval None
 */
void Triangulation_SetMarkerSize(float size_m);

/**
 * @brief Calculate depth from marker size in image
 * @param marker_image_size_pixels Marker size measured in image (pixels)
 * @retval Depth in meters, or 0 if invalid
 */
float Triangulation_CalcDepth(float marker_image_size_pixels);

/**
 * @brief Calculate 3D position from subpixel coordinates
 * @param subpixel_result Subpixel detection results
 * @param result Triangulation result structure
 * @retval TRI_STATUS_OK if calculation successful
 */
Triangulation_StatusTypeDef Triangulation_Calc3DPosition(
    const Subpixel_ResultTypeDef *subpixel_result,
    Triangulation_ResultTypeDef *result);

/**
 * @brief Get camera intrinsic parameters
 * @param params Pointer to camera params structure
 * @retval None
 */
void Triangulation_GetCameraParams(Triangulation_CameraParamsTypeDef *params);

/**
 * @brief Set camera intrinsic parameters
 * @param params Pointer to camera params structure
 * @retval None
 */
void Triangulation_SetCameraParams(const Triangulation_CameraParamsTypeDef *params);

#ifdef __cplusplus
}
#endif

#endif /* __TRIANGULATION_H */