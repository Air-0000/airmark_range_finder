/**
 * @file main.h
 * @brief Main header file for AirMark Range Finder project
 * @author Development Team
 * @version 1.0
 */
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal_conf.h"
#include "stm32f4xx_hal.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief System status enumeration
 */
typedef enum {
    SYS_STATUS_OK = 0,
    SYS_STATUS_ERROR,
    SYS_STATUS_BUSY,
    SYS_STATUS_TIMEOUT,
    SYS_STATUS_NOT_INIT
} System_StatusTypeDef;

/**
 * @brief Error codes for system error log
 */
typedef enum {
    SYS_ERROR_NONE = 0,
    SYS_ERROR_LED_INIT,
    SYS_ERROR_CAMERA_INIT,
    SYS_ERROR_CAMERA_CAPTURE,
    SYS_ERROR_OPTICAL_INIT,
    SYS_ERROR_IMGPROC_INIT,
    SYS_ERROR_TRIANGULATION_INIT,
    SYS_ERROR_CALIB_INIT,
    SYS_ERROR_CALIB_DATA_INVALID,
    SYS_ERROR_DMA_TRANSFER,
    SYS_ERROR_UNKNOWN
} System_ErrorTypeDef;

/* Exported constants --------------------------------------------------------*/
#define FRAME_WIDTH      320U
#define FRAME_HEIGHT     240U
#define LED_COUNT        5U
#define MARKER_COUNT     5U
#define MARKER_SIDE_LEN  0.05f   /* 5cm in meters */
#define FOCAL_LENGTH     0.003f  /* 3mm approximated focal length */

/* Zone thresholds for layered illumination (in meters) */
#define ZONE_LOWER_MIN   0.5f
#define ZONE_LOWER_MAX   1.2f
#define ZONE_MID_MIN     1.2f
#define ZONE_MID_MAX     1.8f
#define ZONE_UPPER_MIN   1.8f
#define ZONE_UPPER_MAX   2.2f

/* Timing constants */
#define HAND_AWAY_DELAY_MS   3000U  /* 3 seconds hand-away timeout */

/* Exported functions --------------------------------------------------------*/
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */