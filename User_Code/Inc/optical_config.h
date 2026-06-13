/**
 * @file optical_config.h
 * @brief Optical Configuration Module - Dual-wavelength NIR with narrowband modulation
 * @author Development Team
 *
 * This module provides:
 * - P0: Dual wavelength switching (850nm/940nm) for spectral matching
 * - P0: High-frequency PWM modulation (10kHz-1MHz, pulse width <= 100us)
 * - P1: Hardware synchronization trigger for LED-camera coordination
 * - P2: Narrowband bandpass filter configuration (FWHM <= 20nm)
 *
 * Hardware: STM32F407, OV2640 camera, 5x 850nm IR LEDs (pentagon arrangement)
 */
#ifndef __OPTICAL_CONFIG_H
#define __OPTICAL_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* ============================================================================
 * EXPORTED TYPES
 * ============================================================================*/

/**
 * @brief LED wavelength selection type
 * @note  Currently hardware supports 850nm; 940nm requires hardware swap
 *        The software architecture supports dual-wavelength for future expansion
 */
typedef enum {
    WAVELENGTH_850NM = 0,    /**< 850nm IR LED (default, current hardware) */
    WAVELENGTH_940NM,        /**< 940nm IR LED (requires hardware change) */
    WAVELENGTH_COUNT
} Optical_WavelengthTypeDef;

/**
 * @brief LED modulation mode type
 */
typedef enum {
    MODULATION_NONE = 0,     /**< No modulation (constant on/off) */
    MODULATION_PWM,          /**< PWM modulation for high-frequency pulsing */
    MODULATION_CODE,         /**< Coding modulation (on-off keying pattern) */
    MODULATION_COUNT
} Optical_ModulationModeTypeDef;

/**
 * @brief External trigger synchronization mode
 */
typedef enum {
    TRIG_MODE_SOFTWARE = 0,  /**< Software trigger (free-running) */
    TRIG_MODE_EXTERNAL,      /**< External hardware trigger (LED sync) */
    TRIG_MODE_PWM_INPUT,     /**< PWM input triggered mode */
    TRIG_MODE_COUNT
} Optical_TriggerModeTypeDef;

/**
 * @brief Trigger polarity for external synchronization
 */
typedef enum {
    TRIG_POLARITY_ACTIVE_HIGH = 0,  /**< Trigger on rising edge/high level */
    TRIG_POLARITY_ACTIVE_LOW        /**< Trigger on falling edge/low level */
} Optical_TriggerPolarityTypeDef;

/**
 * @brief Filter type enumeration
 */
typedef enum {
    FILTER_NONE = 0,
    FILTER_850NM_BANDPASS,          /**< 850nm IR bandpass filter */
    FILTER_940NM_BANDPASS,           /**< 940nm IR bandpass filter */
    FILTER_850NM_LONGPASS,          /**< 850nm IR longpass filter */
    FILTER_NIR_ENHANCE,             /**< NIR enhancement filter */
    FILTER_NARROWBAND_850NM,        /**< Narrowband 850nm (FWHM <= 20nm) */
    FILTER_NARROWBAND_940NM,         /**< Narrowband 940nm (FWHM <= 20nm) */
    FILTER_COUNT
} Optical_FilterTypeDef;

/**
 * @brief PWM modulation parameters structure
 * @note  Frequency range: 10kHz - 1MHz
 *        Pulse width: <= 100us
 */
typedef struct {
    uint32_t frequency_hz;           /**< Modulation frequency in Hz (10kHz-1MHz) */
    uint8_t duty_cycle_percent;     /**< Duty cycle 0-100% */
    uint16_t pulse_width_us;         /**< Pulse width in microseconds (<=100us) */
    uint8_t enabled;                /**< Modulation enable flag */
} Optical_ModulationParamsTypeDef;

/**
 * @brief Wavelength configuration structure
 */
typedef struct {
    Optical_WavelengthTypeDef current_wavelength;  /**< Currently selected wavelength */
    Optical_WavelengthTypeDef supported_wavelengths[WAVELENGTH_COUNT]; /**< Supported wavelengths */
    float wavelengths_nm[WAVELENGTH_COUNT];        /**< Wavelength values in nm */
    uint8_t wavelength_switch_enabled;            /**< Wavelength switching enable */
} Optical_WavelengthConfigTypeDef;

/**
 * @brief External trigger configuration structure
 */
typedef struct {
    Optical_TriggerModeTypeDef mode;              /**< Trigger mode */
    Optical_TriggerPolarityTypeDef polarity;      /**< Trigger polarity */
    uint16_t trigger_delay_us;                   /**< Delay from trigger to exposure start (us) */
    uint16_t exposure_width_us;                  /**< LED exposure pulse width (us) */
    uint8_t sync_enabled;                        /**< Sync enable flag */
    uint8_t trigger_filter_samples;             /**< Debounce filter samples */
} Optical_TriggerConfigTypeDef;

/**
 * @brief Filter parameters structure
 * @note  FWHM (Full Width Half Maximum) <= 20nm for narrowband requirement
 */
typedef struct {
    Optical_FilterTypeDef type;
    float center_wavelength_nm;             /**< Center wavelength in nm */
    float fwhm_nm;                           /**< Full Width Half Maximum in nm (<=20nm) */
    float bandwidth_nm;                      /**< Bandwidth in nm */
    float peak_transmission;                 /**< Peak transmission ratio (0.0-1.0) */
    float cut_on_wavelength_nm;              /**< Cut-on wavelength in nm */
    float cut_off_wavelength_nm;             /**< Cut-off wavelength in nm */
    float blocking_od;                      /**< Out-of-band blocking (optical density) */
} Optical_FilterParamsTypeDef;

/**
 * @brief NIR enhancement configuration
 */
typedef struct {
    uint8_t nir_enhance_enabled;              /**< NIR enhancement enable flag */
    float gain_factor;                        /**< Signal gain factor */
    float offset_adjust;                      /**< DC offset adjustment */
    uint8_t ambient_suppression;             /**< Ambient light suppression level */
} Optical_NIRConfigTypeDef;

/**
 * @brief Complete optical system configuration
 */
typedef struct {
    Optical_WavelengthConfigTypeDef wavelength;
    Optical_ModulationParamsTypeDef modulation;
    Optical_TriggerConfigTypeDef trigger;
    Optical_FilterParamsTypeDef filter;
    Optical_NIRConfigTypeDef nir;
} Optical_SystemConfigTypeDef;

/* ============================================================================
 * EXPORTED CONSTANTS
 * ============================================================================*/

/* Wavelength constants */
#define WAVELENGTH_850NM_VALUE    850.0f    /**< 850nm wavelength in nm */
#define WAVELENGTH_940NM_VALUE    940.0f    /**< 940nm wavelength in nm */

/* Modulation frequency limits */
#define MODULATION_FREQ_MIN_HZ     10000U    /**< Minimum frequency 10kHz */
#define MODULATION_FREQ_MAX_HZ     1000000U  /**< Maximum frequency 1MHz */
#define MODULATION_PULSE_WIDTH_MAX_US  100U  /**< Maximum pulse width 100us */

/* Default modulation parameters */
#define MODULATION_DEFAULT_FREQ_HZ     20000U   /**< Default 20kHz */
#define MODULATION_DEFAULT_DUTY_PCT    50U      /**< Default 50% duty cycle */
#define MODULATION_DEFAULT_PULSE_US    50U      /**< Default 50us pulse width */

/* Trigger configuration defaults */
#define TRIGGER_DEFAULT_DELAY_US   10U       /**< Default 10us trigger delay */
#define TRIGGER_DEFAULT_WIDTH_US   100U      /**< Default 100us exposure width */
#define TRIGGER_DEFAULT_POLARITY   TRIG_POLARITY_ACTIVE_HIGH
#define TRIGGER_FILTER_SAMPLES     3U        /**< Default debounce samples */

/* Filter constants - Narrowband FWHM <= 20nm */
#define FILTER_FWHM_MAX_NM         20.0f     /**< Maximum FWHM for narrowband */
#define FILTER_850NM_CENTER       850.0f    /**< 850nm center wavelength */
#define FILTER_940NM_CENTER       940.0f    /**< 940nm center wavelength */
#define FILTER_850NM_BANDWIDTH    20.0f     /**< 20nm bandwidth for 850nm */
#define FILTER_940NM_BANDWIDTH    20.0f     /**< 20nm bandwidth for 940nm */
#define FILTER_850NM_FWHM         18.0f     /**< 18nm FWHM for 850nm narrowband */
#define FILTER_940NM_FWHM         18.0f     /**< 18nm FWHM for 940nm narrowband */
#define FILTER_PEAK_TRANSMISSION   0.85f     /**< 85% peak transmission */
#define FILTER_BLOCKING_OD         4.0f      /**< OD4 out-of-band blocking */

/* NIR enhancement defaults */
#define NIR_DEFAULT_GAIN           1.5f      /**< Default NIR gain factor */
#define NIR_AMBIENT_SUPPRESSION    0.7f      /**< 70% ambient suppression */

/* ============================================================================
 * EXPORTED FUNCTIONS - INITIALIZATION
 * ============================================================================*/

/**
 * @brief Initialize optical configuration module
 * @note  Sets up default filter parameters, wavelength, modulation, and trigger config
 * @retval None
 */
void Optical_Init(void);

/**
 * @brief Get the complete optical system configuration
 * @param config Pointer to configuration structure to fill
 * @retval None
 */
void Optical_GetSystemConfig(Optical_SystemConfigTypeDef *config);

/* ============================================================================
 * EXPORTED FUNCTIONS - WAVELENGTH CONTROL (P0)
 * ============================================================================*/

/**
 * @brief Set the active wavelength
 * @param wavelength Target wavelength type
 * @retval 0 on success, -1 on error (unsupported wavelength)
 */
int8_t Optical_SetWavelength(Optical_WavelengthTypeDef wavelength);

/**
 * @brief Get the current wavelength
 * @param wavelength Pointer to store current wavelength
 * @retval None
 */
void Optical_GetWavelength(Optical_WavelengthTypeDef *wavelength);

/**
 * @brief Check if wavelength switching is supported
 * @param wavelength Wavelength to check
 * @retval 1 if supported, 0 if not
 */
uint8_t Optical_IsWavelengthSupported(Optical_WavelengthTypeDef wavelength);

/* ============================================================================
 * EXPORTED FUNCTIONS - MODULATION CONTROL (P0)
 * ============================================================================*/

/**
 * @brief Set PWM modulation parameters
 * @param frequency_hz Modulation frequency (10kHz-1MHz)
 * @param duty_cycle_percent Duty cycle percentage (0-100)
 * @param pulse_width_us Pulse width in microseconds (<=100us)
 * @retval 0 on success, -1 on invalid parameters
 * @note  pulse_width_us must be <= 100us and compatible with frequency
 */
int8_t Optical_SetModulationParams(uint32_t frequency_hz,
                                     uint8_t duty_cycle_percent,
                                     uint16_t pulse_width_us);

/**
 * @brief Get current modulation parameters
 * @param params Pointer to modulation params structure
 * @retval None
 */
void Optical_GetModulationParams(Optical_ModulationParamsTypeDef *params);

/**
 * @brief Enable or disable modulation
 * @param enable 1 to enable, 0 to disable
 * @retval None
 */
void Optical_SetModulationEnable(uint8_t enable);

/**
 * @brief Get modulation enable state
 * @retval 1 if enabled, 0 if disabled
 */
uint8_t Optical_GetModulationEnable(void);

/**
 * @brief Validate modulation parameters
 * @param frequency_hz Frequency to validate
 * @param pulse_width_us Pulse width to validate
 * @retval 1 if valid, 0 if invalid
 */
uint8_t Optical_ValidateModulationParams(uint32_t frequency_hz, uint16_t pulse_width_us);

/* ============================================================================
 * EXPORTED FUNCTIONS - TRIGGER/SYNCHRONIZATION (P1)
 * ============================================================================*/

/**
 * @brief Configure external trigger synchronization
 * @param mode Trigger mode (software/external/PWM input)
 * @param polarity Trigger polarity (active high/low)
 * @retval None
 */
void Optical_SetTriggerMode(Optical_TriggerModeTypeDef mode,
                            Optical_TriggerPolarityTypeDef polarity);

/**
 * @brief Get current trigger configuration
 * @param config Pointer to trigger config structure
 * @retval None
 */
void Optical_GetTriggerConfig(Optical_TriggerConfigTypeDef *config);

/**
 * @brief Set trigger delay (time from trigger to exposure start)
 * @param delay_us Delay in microseconds
 * @retval None
 */
void Optical_SetTriggerDelay(uint16_t delay_us);

/**
 * @brief Set LED exposure pulse width
 * @param width_us Pulse width in microseconds
 * @retval None
 */
void Optical_SetExposureWidth(uint16_t width_us);

/**
 * @brief Enable or disable hardware synchronization
 * @param enable 1 to enable sync, 0 to disable
 * @retval None
 */
void Optical_SetSyncEnable(uint8_t enable);

/**
 * @brief Get sync enable state
 * @retval 1 if sync enabled, 0 if disabled
 */
uint8_t Optical_GetSyncEnable(void);

/**
 * @brief Wait for external trigger (blocking)
 * @param timeout_ms Timeout in milliseconds
 * @retval 0 on trigger received, -1 on timeout
 */
int8_t Optical_WaitForTrigger(uint32_t timeout_ms);

/* ============================================================================
 * EXPORTED FUNCTIONS - FILTER CONFIGURATION (P2)
 * ============================================================================*/

/**
 * @brief Set filter type and update parameters accordingly
 * @param filter_type Filter type to set
 * @retval None
 * @note  For narrowband filters (FWHM <= 20nm), use FILTER_NARROWBAND_850NM/940NM
 */
void Optical_SetFilterType(Optical_FilterTypeDef filter_type);

/**
 * @brief Get current filter parameters
 * @param params Pointer to filter params structure
 * @retval None
 */
void Optical_GetFilterParams(Optical_FilterParamsTypeDef *params);

/**
 * @brief Set custom narrowband filter parameters
 * @param center_nm Center wavelength in nm
 * @param fwhm_nm Full Width Half Maximum in nm (should be <= 20nm)
 * @param peak_transmission Peak transmission (0.0-1.0)
 * @retval 0 on success, -1 on invalid parameters
 */
int8_t Optical_SetCustomNarrowbandFilter(float center_nm,
                                          float fwhm_nm,
                                          float peak_transmission);

/**
 * @brief Check if current filter is narrowband (FWHM <= 20nm)
 * @retval 1 if narrowband, 0 if not
 */
uint8_t Optical_IsFilterNarrowband(void);

/* ============================================================================
 * EXPORTED FUNCTIONS - NIR ENHANCEMENT
 * ============================================================================*/

/**
 * @brief Get current NIR enhancement configuration
 * @param config Pointer to NIR config structure
 * @retval None
 */
void Optical_GetNIRConfig(Optical_NIRConfigTypeDef *config);

/**
 * @brief Enable or disable NIR enhancement
 * @param enable 1 to enable, 0 to disable
 * @retval None
 */
void Optical_SetNIREnhance(uint8_t enable);

/**
 * @brief Set NIR gain factor
 * @param gain Gain factor (> 0.0)
 * @retval None
 */
void Optical_SetNIRGain(float gain);

/* ============================================================================
 * EXPORTED FUNCTIONS - UTILITY
 * ============================================================================*/

/**
 * @brief Reset all optical configuration to defaults
 * @retval None
 */
void Optical_ResetToDefaults(void);

/**
 * @brief Get optical system status string for debugging
 * @param status_str Buffer to fill with status string
 * @param buf_len Buffer length
 * @retval None
 */
void Optical_DebugStatus(char *status_str, uint32_t buf_len);

/**
 * @brief Calculate modulation timing from frequency
 * @param frequency_hz Frequency in Hz
 * @param duty_cycle_percent Duty cycle percentage
 * @param period_us Pointer to store period in microseconds
 * @param pulse_width_us Pointer to store pulse width in microseconds
 * @retval None
 */
void Optical_CalcModulationTiming(uint32_t frequency_hz,
                                  uint8_t duty_cycle_percent,
                                  uint32_t *period_us,
                                  uint32_t *pulse_width_us);

#ifdef __cplusplus
}
#endif

#endif /* __OPTICAL_CONFIG_H */