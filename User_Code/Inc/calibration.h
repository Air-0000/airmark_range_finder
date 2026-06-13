/**
 * @file calibration.h
 * @brief Calibration Module - System calibration and parameter storage
 * @author Development Team
 */
#ifndef __CALIBRATION_H
#define __CALIBRATION_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "triangulation.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Calibration status
 */
typedef enum {
    CALIB_STATUS_OK = 0,
    CALIB_STATUS_ERROR,
    CALIB_STATUS_NOT_CALIBRATED,
    CALIB_STATUS_INVALID_DATA
} Calib_StatusTypeDef;

/**
 * @brief Calibration parameters structure
 */
typedef struct {
    float focal_length;            /**< Calibrated focal length in pixels */
    float marker_size;             /**< Marker physical size in meters */
    float optical_center_x;        /**< Calibrated optical center X */
    float optical_center_y;        /**< Calibrated optical center Y */
    float distortion_k1;           /**< Radial distortion coefficient */
    float distortion_k2;           /**< Radial distortion coefficient */
    float reference_distance;      /**< Reference distance used for calibration */
    uint32_t calibration_version;  /**< Calibration data version */
} Calib_ParamsTypeDef;

/**
 * @brief Calibration data type
 */
typedef enum {
    CALIB_DATA_FOCAL = 0,      /**< Focal length calibration */
    CALIB_DATA_OPTICAL,        /**< Optical center calibration */
    CALIB_DATA_DISTORTION,     /**< Distortion calibration */
    CALIB_DATA_FULL            /**< Full calibration */
} Calib_DataTypeDef;

/* Exported constants --------------------------------------------------------*/
#define CALIB_MAGIC_NUMBER    0xCAFEBABEU   /**< Flash magic number */
#define CALIB_VERSION         0x00010000U   /**< Version 1.0.0 */
#define CALIB_MAX_MARKERS     MARKER_COUNT

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialize calibration module
 * @note  Loads calibration data from flash or initializes defaults
 * @retval CALIB_STATUS_OK on success
 */
Calib_StatusTypeDef Calib_Init(void);

/**
 * @brief Run quick calibration sequence
 * @note  Performs simplified calibration at known distance
 * @param reference_distance Known reference distance in meters
 * @retval CALIB_STATUS_OK on success
 */
Calib_StatusTypeDef Calib_RunQuickCalibration(float reference_distance);

/**
 * @brief Get current calibration parameters
 * @param params Pointer to calibration params structure
 * @retval None
 */
void Calib_GetParams(Calib_ParamsTypeDef *params);

/**
 * @brief Save calibration parameters to flash
 * @retval CALIB_STATUS_OK on success
 */
Calib_StatusTypeDef Calib_SaveParams(void);

/**
 * @brief Check if system is calibrated
 * @retval 1 if calibrated, 0 otherwise
 */
uint8_t Calib_IsCalibrated(void);

/**
 * @brief Reset calibration to defaults
 * @retval None
 */
void Calib_ResetToDefaults(void);

/**
 * @brief Validate calibration data integrity
 * @retval 1 if valid, 0 if corrupted
 */
uint8_t Calib_ValidateData(void);

#ifdef __cplusplus
}
#endif

#endif /* __CALIBRATION_H */