/**
 * @file camera_driver.h
 * @brief Camera Driver Module - OV2640 8-bit grayscale 320x240
 * @author Development Team
 *
 * This module extends the original camera driver with:
 * - P1: Hardware synchronization trigger support (camera only captures when LED is on)
 * - P1: External trigger mode for LED-camera coordination
 * - P2: Trigger polarity and delay configuration
 *
 * Hardware: STM32F407, OV2640 camera
 */
#ifndef __CAMERA_DRIVER_H
#define __CAMERA_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "optical_config.h"  /* Include optical config for trigger types */

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Camera operation status
 */
typedef enum {
    CAM_STATUS_OK = 0,
    CAM_STATUS_ERROR,
    CAM_STATUS_BUSY,
    CAM_STATUS_TIMEOUT,
    CAM_STATUS_NOT_INIT
} Camera_StatusTypeDef;

/**
 * @brief Camera exposure level
 */
typedef enum {
    CAM_EXPOSURE_LOW = 0,
    CAM_EXPOSURE_MEDIUM,
    CAM_EXPOSURE_HIGH,
    CAM_EXPOSURE_AUTO
} Camera_ExposureTypeDef;

/**
 * @brief Camera frame structure
 * @note  Extended to support 16-bit grayscale for NIR enhanced mode
 */
typedef struct {
    void *frame_buffer;          /**< Frame buffer pointer (uint8_t* or uint16_t*) */
    uint16_t width;
    uint16_t height;
    uint32_t timestamp;
    uint8_t is_ready;
    uint8_t data_type;           /**< 0=8bit, 1=16bit */
} Camera_FrameTypeDef;

/**
 * @brief Camera NIR enhancement mode
 * @note  Sony IMX290LLR style NIR optimization for 850nm LEDs
 */
typedef enum {
    CAM_NIR_MODE_OFF = 0,       /**< Standard mode */
    CAM_NIR_MODE_850NM,          /**< 850nm NIR enhanced (IMX290LLR style) */
    CAM_NIR_MODE_HIGH_SENS       /**< High sensitivity mode (2-frame average) */
} Camera_NIRModeTypeDef;

/**
 * @brief Camera event callback type
 */
typedef void (*Camera_CallbackTypeDef)(Camera_FrameTypeDef *frame);

/* ============================================================================
 * P1: EXTERNAL TRIGGER/SYNCHRONIZATION TYPES
 * ============================================================================*/

/**
 * @brief Camera trigger mode type
 */
typedef enum {
    CAM_TRIG_MODE_SOFTWARE = 0,  /**< Software trigger (default) */
    CAM_TRIG_MODE_EXTERNAL,       /**< External hardware trigger */
    CAM_TRIG_MODE_PWM,           /**< PWM input triggered mode */
    CAM_TRIG_MODE_COUNT
} Camera_TriggerModeTypeDef;

/**
 * @brief Camera trigger event callback (P1)
 * @note  Called when external trigger is detected
 */
typedef void (*Camera_TriggerCallbackTypeDef)(void);

/**
 * @brief Camera synchronization status
 */
typedef struct {
    uint8_t sync_enabled;                 /**< Sync mode enabled flag */
    Camera_TriggerModeTypeDef mode;       /**< Current trigger mode */
    Optical_TriggerPolarityTypeDef polarity; /**< Trigger polarity */
    uint16_t trigger_delay_us;           /**< Delay from trigger to exposure */
    uint16_t exposure_width_us;           /**< LED exposure pulse width */
    uint32_t last_trigger_timestamp;      /**< Timestamp of last trigger */
    uint32_t trigger_count;               /**< Total trigger count */
} Camera_SyncStatusTypeDef;

/* Exported constants --------------------------------------------------------*/
#define CAM_FRAME_WIDTH     320U
#define CAM_FRAME_HEIGHT    240U
#define CAM_FRAME_SIZE      (CAM_FRAME_WIDTH * CAM_FRAME_HEIGHT)  /* 76800 bytes */

/* Trigger timing limits */
#define CAM_TRIGGER_DELAY_MIN_US   1U     /**< Minimum trigger delay (us) */
#define CAM_TRIGGER_DELAY_MAX_US   1000U  /**< Maximum trigger delay (us) */
#define CAM_TRIGGER_WIDTH_MIN_US   10U   /**< Minimum exposure width (us) */
#define CAM_TRIGGER_WIDTH_MAX_US   10000U /**< Maximum exposure width (us) */

/* External trigger GPIO (configurable - example uses PA6) */
#define CAM_EXT_TRIGGER_PORT GPIOA
#define CAM_EXT_TRIGGER_PIN  GPIO_PIN_6

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialize camera driver
 * @note  Configures OV2640 in 8-bit grayscale 320x240 mode via DCMI/DMA
 * @retval CAM_STATUS_OK on success
 */
Camera_StatusTypeDef Camera_Init(void);

/**
 * @brief Deinitialize camera driver
 * @retval CAM_STATUS_OK on success
 */
Camera_StatusTypeDef Camera_DeInit(void);

/* ----- Basic Camera Control ----- */
/**
 * @brief Set camera exposure level
 * @param exposure Exposure level enum
 * @retval None
 */
void Camera_SetExposure(Camera_ExposureTypeDef exposure);

/**
 * @brief Start single frame capture
 * @retval CAM_STATUS_OK if capture started successfully
 */
Camera_StatusTypeDef Camera_StartCapture(void);

/**
 * @brief Get the most recent frame
 * @param frame Pointer to frame structure to fill
 * @retval CAM_STATUS_OK if frame available
 */
Camera_StatusTypeDef Camera_GetFrame(Camera_FrameTypeDef *frame);

/**
 * @brief Register callback for frame ready events
 * @param callback Function pointer to call when frame is ready
 * @retval None
 */
void Camera_RegisterCallback(Camera_CallbackTypeDef callback);

/**
 * @brief Check if camera is currently capturing
 * @retval 1 if capturing, 0 otherwise
 */
uint8_t Camera_IsCapturing(void);

/* ----- NIR Mode (original API, preserved) ----- */
/**
 * @brief Extended camera initialization with NIR mode
 * @param nir_mode NIR enhancement mode
 * @param use_16bit Use 16-bit frame buffer (for NIR high-res mode)
 * @retval CAM_STATUS_OK on success
 */
Camera_StatusTypeDef Camera_InitEx(Camera_NIRModeTypeDef nir_mode, uint8_t use_16bit);

/**
 * @brief Set NIR enhancement mode
 * @param mode NIR mode to set
 * @retval None
 */
void Camera_SetNIRMode(Camera_NIRModeTypeDef mode);

/**
 * @brief Get current NIR mode
 * @retval Current NIR mode
 */
Camera_NIRModeTypeDef Camera_GetNIRMode(void);

/**
 * @brief Get 16-bit frame data (for NIR mode)
 * @param frame Pointer to frame structure to fill
 * @param buffer_16b 16-bit buffer to store frame data
 * @retval CAM_STATUS_OK if frame available
 */
Camera_StatusTypeDef Camera_GetFrame16b(Camera_FrameTypeDef *frame, uint16_t *buffer_16b);

/* ----- P1: Synchronization/Trigger Control ----- */
/**
 * @brief Configure external trigger mode (P1)
 * @param mode Trigger mode (software/external/PWM)
 * @param polarity Trigger polarity (active high/low)
 * @retval None
 * @note  In external trigger mode, camera only captures when LED trigger is active
 */
void Camera_SetTriggerMode(Camera_TriggerModeTypeDef mode,
                           Optical_TriggerPolarityTypeDef polarity);

/**
 * @brief Get current trigger configuration (P1)
 * @param mode Pointer to store trigger mode
 * @param polarity Pointer to store trigger polarity
 * @retval None
 */
void Camera_GetTriggerConfig(Camera_TriggerModeTypeDef *mode,
                             Optical_TriggerPolarityTypeDef *polarity);

/**
 * @brief Enable or disable hardware synchronization (P1)
 * @param enable 1 to enable sync, 0 to disable
 * @retval None
 * @note  When enabled, camera only exposes during LED on-time (LED-camera sync)
 */
void Camera_SetSyncEnable(uint8_t enable);

/**
 * @brief Get synchronization enable state (P1)
 * @retval 1 if sync enabled, 0 if disabled
 */
uint8_t Camera_GetSyncEnable(void);

/**
 * @brief Set trigger delay (P1)
 * @param delay_us Delay in microseconds from trigger to exposure start
 * @retval None
 */
void Camera_SetTriggerDelay(uint16_t delay_us);

/**
 * @brief Set exposure width for sync mode (P1)
 * @param width_us Exposure pulse width in microseconds
 * @retval None
 */
void Camera_SetExposureWidth(uint16_t width_us);

/**
 * @brief Register callback for trigger events (P1)
 * @param callback Function pointer to call when external trigger is detected
 * @retval None
 */
void Camera_RegisterTriggerCallback(Camera_TriggerCallbackTypeDef callback);

/**
 * @brief Get camera synchronization status (P1)
 * @param status Pointer to sync status structure
 * @retval None
 */
void Camera_GetSyncStatus(Camera_SyncStatusTypeDef *status);

/**
 * @brief Reset trigger counter (P1)
 * @retval None
 */
void Camera_ResetTriggerCount(void);

/**
 * @brief Wait for external trigger (blocking) (P1)
 * @param timeout_ms Timeout in milliseconds
 * @retval 0 on trigger received, -1 on timeout
 */
int8_t Camera_WaitForTrigger(uint32_t timeout_ms);

/* ----- Debug ----- */
/**
 * @brief Get camera status string for debugging
 * @param status_str Buffer to fill with status string
 * @param buf_len Buffer length
 * @retval None
 */
void Camera_DebugStatus(char *status_str, uint32_t buf_len);

#ifdef __cplusplus
}
#endif

#endif /* __CAMERA_DRIVER_H */