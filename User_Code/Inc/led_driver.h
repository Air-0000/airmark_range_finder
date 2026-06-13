/**
 * @file led_driver.h
 * @brief LED Driver Module - Controls 5x 850nm IR LEDs in pentagon arrangement
 * @author Development Team
 *
 * This module extends the original LED driver with:
 * - P0: Dual wavelength switching (850nm/940nm) support
 * - P0: High-frequency PWM modulation (10kHz-1MHz, pulse width <= 100us)
 * - P1: Hardware synchronization trigger support
 *
 * Hardware: STM32F407, 5x IR LEDs (pentagon arrangement)
 */
#ifndef __LED_DRIVER_H
#define __LED_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "optical_config.h"  /* Include optical config for modulation params */

/* Exported types ------------------------------------------------------------*/
/**
 * @brief LED operation status
 */
typedef enum {
    LED_STATUS_OK = 0,
    LED_STATUS_ERROR,
    LED_STATUS_BUSY,
    LED_STATUS_TIMEOUT
} LED_StatusTypeDef;

/**
 * @brief LED operation mode
 */
typedef enum {
    LED_MODE_OFF = 0,        /**< All LEDs off */
    LED_MODE_CONSTANT,       /**< Constant illumination mode */
    LED_MODE_DARK_FLASH,     /**< Dark flash sync mode (active low) */
    LED_MODE_BREATHING,      /**< Breathing effect mode */
    LED_MODE_ALERT,          /**< Alert mode (red LED warning) */
    LED_MODE_MODULATED,      /**< P0: High-frequency modulated mode (PWM 10kHz-1MHz) */
    LED_MODE_SYNC_TRIGGER,   /**< P1: Synchronized trigger mode (hardware sync) */
    LED_MODE_COUNT
} LED_ModeTypeDef;

/**
 * @brief LED enable word type (5-bit mask for 5 LEDs)
 */
typedef uint8_t LED_EnableWordType;

/**
 * @brief LED status structure
 */
typedef struct {
    LED_ModeTypeDef mode;
    LED_EnableWordType enabled_mask;
    uint32_t flash_period_ms;
    uint8_t flash_duty_cycle;
    Optical_ModulationParamsTypeDef modulation_params;  /**< P0: Modulation parameters */
    Optical_WavelengthTypeDef wavelength;               /**< P0: Current wavelength */
} LED_StatusTypedef;

/* Exported constants --------------------------------------------------------*/
#define LED_MASK_ALL    0x1FU   /**< Bit 0-4 for LED0-LED4 */
#define LED_MASK_NONE   0x00U

/* LED bit positions */
#define LED_BIT_0       0x01U
#define LED_BIT_1       0x02U
#define LED_BIT_2       0x04U
#define LED_BIT_3       0x08U
#define LED_BIT_4       0x10U

/* Timer definitions for PWM modulation */
#define LED_PWM_TIMER_INSTANCE       TIM2        /**< Timer instance for PWM */
#define LED_PWM_TIMER_CLOCK           84000000U   /**< 84MHz timer clock */
#define LED_PWM_MAX_FREQUENCY_HZ      1000000U    /**< 1MHz max PWM frequency */
#define LED_PWM_MIN_FREQUENCY_HZ      10000U      /**< 10kHz min PWM frequency */

/* Exported functions --------------------------------------------------------*/

/* ----- Basic Initialization ----- */
/**
 * @brief Initialize LED driver
 * @note  Configures GPIO pins for 5 IR LEDs in pentagon arrangement
 *        Also configures PWM timer for modulated mode
 * @retval LED_STATUS_OK on success
 */
LED_StatusTypeDef LED_Init(void);

/**
 * @brief Deinitialize LED driver
 * @note  Disables all LEDs and stops PWM timer
 * @retval LED_STATUS_OK on success
 */
LED_StatusTypeDef LED_DeInit(void);

/* ----- LED Control ----- */
/**
 * @brief Set LED enable word
 * @param enable_word 5-bit mask indicating which LEDs to enable
 * @retval None
 */
void LED_SetEnableWord(LED_EnableWordType enable_word);

/**
 * @brief Get LED enable word
 * @retval Current enable word (5-bit mask)
 */
LED_EnableWordType LED_GetEnableWord(void);

/**
 * @brief Set LED operation mode
 * @param mode LED mode (off, constant, dark_flash, breathing, alert, modulated, sync_trigger)
 * @retval None
 */
void LED_SetMode(LED_ModeTypeDef mode);

/**
 * @brief Get current LED mode
 * @retval Current LED mode
 */
LED_ModeTypeDef LED_GetMode(void);

/**
 * @brief Get current LED status
 * @param status Pointer to LED status structure
 * @retval None
 */
void LED_GetStatus(LED_StatusTypedef *status);

/**
 * @brief Set flash parameters for dark flash sync mode
 * @param period_ms Flash period in milliseconds
 * @param duty_cycle Duty cycle percentage (0-100)
 * @retval None
 */
void LED_SetFlashParams(uint32_t period_ms, uint8_t duty_cycle);

/* ----- P0: Modulation Control ----- */
/**
 * @brief Set PWM modulation parameters (P0)
 * @param frequency_hz Modulation frequency (10kHz-1MHz)
 * @param duty_cycle_percent Duty cycle percentage (0-100)
 * @param pulse_width_us Pulse width in microseconds (<=100us)
 * @retval LED_STATUS_OK on success, LED_STATUS_ERROR on invalid params
 */
LED_StatusTypeDef LED_SetModulation(uint32_t frequency_hz,
                                      uint8_t duty_cycle_percent,
                                      uint16_t pulse_width_us);

/**
 * @brief Get current modulation parameters (P0)
 * @param params Pointer to modulation params structure
 * @retval None
 */
void LED_GetModulationParams(Optical_ModulationParamsTypeDef *params);

/**
 * @brief Enable or disable modulated mode (P0)
 * @param enable 1 to enable, 0 to disable
 * @retval None
 */
void LED_SetModulationEnable(uint8_t enable);

/**
 * @brief Get modulation enable state (P0)
 * @retval 1 if enabled, 0 if disabled
 */
uint8_t LED_GetModulationEnable(void);

/**
 * @brief Start modulated LED output (P0)
 * @note  Begins high-frequency PWM output on all enabled LEDs
 * @retval None
 */
void LED_StartModulated(void);

/**
 * @brief Stop modulated LED output (P0)
 * @note  Stops PWM output and turns off LEDs
 * @retval None
 */
void LED_StopModulated(void);

/* ----- P0: Wavelength Control ----- */
/**
 * @brief Set LED wavelength (P0)
 * @param wavelength Target wavelength (WAVELENGTH_850NM or WAVELENGTH_940NM)
 * @note  Currently hardware supports 850nm; 940nm requires hardware swap
 *        This function stores the software selection for future expansion
 * @retval LED_STATUS_OK on success, LED_STATUS_ERROR if unsupported
 */
LED_StatusTypeDef LED_SetWavelength(Optical_WavelengthTypeDef wavelength);

/**
 * @brief Get current LED wavelength (P0)
 * @param wavelength Pointer to store current wavelength
 * @retval None
 */
void LED_GetWavelength(Optical_WavelengthTypeDef *wavelength);

/**
 * @brief Check if wavelength is supported (P0)
 * @param wavelength Wavelength to check
 * @retval 1 if supported, 0 if not
 */
uint8_t LED_IsWavelengthSupported(Optical_WavelengthTypeDef wavelength);

/* ----- P1: Synchronization Control ----- */
/**
 * @brief Configure LED for external sync trigger mode (P1)
 * @param enable 1 to enable sync mode, 0 to disable
 * @param polarity Trigger polarity (active high/low)
 * @retval None
 * @note  In sync mode, LEDs only turn on when external trigger is active
 */
void LED_SetSyncMode(uint8_t enable, Optical_TriggerPolarityTypeDef polarity);

/**
 * @brief Set LED sync pulse parameters (P1)
 * @param pulse_width_us LED on pulse width in microseconds
 * @param delay_us Delay from trigger to LED on in microseconds
 * @retval None
 */
void LED_SetSyncPulseParams(uint16_t pulse_width_us, uint16_t delay_us);

/**
 * @brief Get sync mode status (P1)
 * @param polarity Pointer to store trigger polarity
 * @retval 1 if sync mode enabled, 0 if disabled
 */
uint8_t LED_GetSyncMode(Optical_TriggerPolarityTypeDef *polarity);

/* ----- PWM Timer Access ----- */
/**
 * @brief Get PWM timer handle for advanced control
 * @note  Use with caution - directly manipulating timer can affect modulation
 * @retval Pointer to TIM_HandleTypeDef
 */
TIM_HandleTypeDef* LED_GetPwmTimerHandle(void);

/**
 * @brief Update PWM timer dynamically
 * @param frequency_hz New PWM frequency in Hz
 * @retval LED_STATUS_OK on success, LED_STATUS_ERROR if out of range
 */
LED_StatusTypeDef LED_UpdatePwmFrequency(uint32_t frequency_hz);

/* ----- Debug ----- */
/**
 * @brief Get LED driver status string for debugging
 * @param status_str Buffer to fill with status string
 * @param buf_len Buffer length
 * @retval None
 */
void LED_DebugStatus(char *status_str, uint32_t buf_len);

#ifdef __cplusplus
}
#endif

#endif /* __LED_DRIVER_H */