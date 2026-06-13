/**
 * @file optical_config.c
 * @brief Optical Configuration Implementation - Dual-wavelength NIR narrowband
 * @author Development Team
 *
 * Features:
 * - Dual wavelength (850nm/940nm) configuration
 * - PWM modulation (20kHz default, 10kHz-1MHz range)
 * - Hardware trigger synchronization
 * - Narrowband filter configuration (FWHM <= 20nm)
 */
#include "optical_config.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>

/* Private variables ---------------------------------------------------------*/
static Optical_WavelengthConfigTypeDef g_wavelength_config = {
    .current_wavelength = WAVELENGTH_850NM,
    .supported_wavelengths = {WAVELENGTH_850NM, WAVELENGTH_940NM},
    .wavelengths_nm = {WAVELENGTH_850NM_VALUE, WAVELENGTH_940NM_VALUE},
    .wavelength_switch_enabled = 1U
};

static Optical_ModulationParamsTypeDef g_modulation_params = {
    .frequency_hz = MODULATION_DEFAULT_FREQ_HZ,
    .duty_cycle_percent = MODULATION_DEFAULT_DUTY_PCT,
    .pulse_width_us = MODULATION_DEFAULT_PULSE_US,
    .enabled = 0U
};

static Optical_TriggerConfigTypeDef g_trigger_config = {
    .mode = TRIG_MODE_SOFTWARE,
    .polarity = TRIGGER_DEFAULT_POLARITY,
    .trigger_delay_us = TRIGGER_DEFAULT_DELAY_US,
    .exposure_width_us = TRIGGER_DEFAULT_WIDTH_US,
    .sync_enabled = 0U,
    .trigger_filter_samples = TRIGGER_FILTER_SAMPLES
};

static Optical_FilterParamsTypeDef g_filter_params = {
    .type = FILTER_NARROWBAND_850NM,
    .center_wavelength_nm = FILTER_850NM_CENTER,
    .fwhm_nm = FILTER_850NM_FWHM,
    .bandwidth_nm = FILTER_850NM_BANDWIDTH,
    .peak_transmission = FILTER_PEAK_TRANSMISSION,
    .cut_on_wavelength_nm = (FILTER_850NM_CENTER - FILTER_850NM_FWHM / 2.0f),
    .cut_off_wavelength_nm = (FILTER_850NM_CENTER + FILTER_850NM_FWHM / 2.0f),
    .blocking_od = FILTER_BLOCKING_OD
};

static Optical_NIRConfigTypeDef g_nir_config = {
    .nir_enhance_enabled = 1U,
    .gain_factor = NIR_DEFAULT_GAIN,
    .offset_adjust = 0.0f,
    .ambient_suppression = NIR_AMBIENT_SUPPRESSION
};

static volatile uint8_t g_trigger_event_flag = 0U;
static volatile uint32_t g_last_trigger_timestamp = 0U;

/* Private function prototypes -----------------------------------------------*/
static void Optical_LoadDefaultWavelengthConfig(void);
static void Optical_LoadDefaultModulationParams(void);
static void Optical_LoadDefaultTriggerConfig(void);
static void Optical_LoadDefaultFilterParams(void);
static void Optical_LoadDefaultNIRConfig(void);
static void Optical_UpdateFilterParamsFromType(Optical_FilterTypeDef filter_type);

/* ============================================================================
 * INITIALIZATION FUNCTIONS
 * ============================================================================*/

/**
 * @brief Initialize optical configuration module
 */
void Optical_Init(void)
{
    Optical_LoadDefaultWavelengthConfig();
    Optical_LoadDefaultModulationParams();
    Optical_LoadDefaultTriggerConfig();
    Optical_LoadDefaultFilterParams();
    Optical_LoadDefaultNIRConfig();

    g_trigger_event_flag = 0U;
    g_last_trigger_timestamp = 0U;
}

/**
 * @brief Get the complete optical system configuration
 */
void Optical_GetSystemConfig(Optical_SystemConfigTypeDef *config)
{
    if (config != NULL) {
        config->wavelength = g_wavelength_config;
        config->modulation = g_modulation_params;
        config->trigger = g_trigger_config;
        config->filter = g_filter_params;
        config->nir = g_nir_config;
    }
}

/* ============================================================================
 * WAVELENGTH CONTROL FUNCTIONS (P0)
 * ============================================================================*/

/**
 * @brief Set the active wavelength
 */
int8_t Optical_SetWavelength(Optical_WavelengthTypeDef wavelength)
{
    if (wavelength >= WAVELENGTH_COUNT) {
        return -1;
    }

    if (!Optical_IsWavelengthSupported(wavelength)) {
        return -1;
    }

    g_wavelength_config.current_wavelength = wavelength;
    return 0;
}

/**
 * @brief Get the current wavelength
 */
void Optical_GetWavelength(Optical_WavelengthTypeDef *wavelength)
{
    if (wavelength != NULL) {
        *wavelength = g_wavelength_config.current_wavelength;
    }
}

/**
 * @brief Check if wavelength switching is supported
 */
uint8_t Optical_IsWavelengthSupported(Optical_WavelengthTypeDef wavelength)
{
    uint8_t i;
    for (i = 0U; i < WAVELENGTH_COUNT; i++) {
        if (g_wavelength_config.supported_wavelengths[i] == wavelength) {
            return 1U;
        }
    }
    return 0U;
}

/* ============================================================================
 * MODULATION CONTROL FUNCTIONS (P0)
 * ============================================================================*/

/**
 * @brief Set PWM modulation parameters
 */
int8_t Optical_SetModulationParams(uint32_t frequency_hz,
                                     uint8_t duty_cycle_percent,
                                     uint16_t pulse_width_us)
{
    if (!Optical_ValidateModulationParams(frequency_hz, pulse_width_us)) {
        return -1;
    }

    g_modulation_params.frequency_hz = frequency_hz;
    g_modulation_params.duty_cycle_percent = (duty_cycle_percent > 100U) ? 100U : duty_cycle_percent;
    g_modulation_params.pulse_width_us = pulse_width_us;

    return 0;
}

/**
 * @brief Get current modulation parameters
 */
void Optical_GetModulationParams(Optical_ModulationParamsTypeDef *params)
{
    if (params != NULL) {
        params->frequency_hz = g_modulation_params.frequency_hz;
        params->duty_cycle_percent = g_modulation_params.duty_cycle_percent;
        params->pulse_width_us = g_modulation_params.pulse_width_us;
        params->enabled = g_modulation_params.enabled;
    }
}

/**
 * @brief Enable or disable modulation
 */
void Optical_SetModulationEnable(uint8_t enable)
{
    g_modulation_params.enabled = enable ? 1U : 0U;
}

/**
 * @brief Get modulation enable state
 */
uint8_t Optical_GetModulationEnable(void)
{
    return g_modulation_params.enabled;
}

/**
 * @brief Validate modulation parameters
 */
uint8_t Optical_ValidateModulationParams(uint32_t frequency_hz, uint16_t pulse_width_us)
{
    /* Check frequency bounds */
    if (frequency_hz < MODULATION_FREQ_MIN_HZ || frequency_hz > MODULATION_FREQ_MAX_HZ) {
        return 0U;
    }

    /* Check pulse width bound */
    if (pulse_width_us > MODULATION_PULSE_WIDTH_MAX_US) {
        return 0U;
    }

    /* Calculate minimum period and verify pulse width */
    uint32_t period_us = 1000000U / frequency_hz;
    if (pulse_width_us >= period_us) {
        return 0U;
    }

    return 1U;
}

/* ============================================================================
 * TRIGGER/SYNCHRONIZATION FUNCTIONS (P1)
 * ============================================================================*/

/**
 * @brief Configure external trigger synchronization
 */
void Optical_SetTriggerMode(Optical_TriggerModeTypeDef mode,
                            Optical_TriggerPolarityTypeDef polarity)
{
    if (mode < TRIG_MODE_COUNT) {
        g_trigger_config.mode = mode;
    }
    g_trigger_config.polarity = polarity;
}

/**
 * @brief Get current trigger configuration
 */
void Optical_GetTriggerConfig(Optical_TriggerConfigTypeDef *config)
{
    if (config != NULL) {
        config->mode = g_trigger_config.mode;
        config->polarity = g_trigger_config.polarity;
        config->trigger_delay_us = g_trigger_config.trigger_delay_us;
        config->exposure_width_us = g_trigger_config.exposure_width_us;
        config->sync_enabled = g_trigger_config.sync_enabled;
        config->trigger_filter_samples = g_trigger_config.trigger_filter_samples;
    }
}

/**
 * @brief Set trigger delay
 */
void Optical_SetTriggerDelay(uint16_t delay_us)
{
    g_trigger_config.trigger_delay_us = delay_us;
}

/**
 * @brief Set LED exposure pulse width
 */
void Optical_SetExposureWidth(uint16_t width_us)
{
    g_trigger_config.exposure_width_us = width_us;
}

/**
 * @brief Enable or disable hardware synchronization
 */
void Optical_SetSyncEnable(uint8_t enable)
{
    g_trigger_config.sync_enabled = enable ? 1U : 0U;
}

/**
 * @brief Get sync enable state
 */
uint8_t Optical_GetSyncEnable(void)
{
    return g_trigger_config.sync_enabled;
}

/**
 * @brief Wait for external trigger (blocking)
 */
int8_t Optical_WaitForTrigger(uint32_t timeout_ms)
{
    uint32_t start_tick = HAL_GetTick();
    uint32_t timeout_ticks = timeout_ms;

    while (!g_trigger_event_flag) {
        if ((HAL_GetTick() - start_tick) >= timeout_ticks) {
            return -1;
        }
    }

    g_trigger_event_flag = 0U;
    return 0;
}

/**
 * @brief Signal trigger event (called from ISR)
 */
void Optical_SignalTriggerEvent(void)
{
    g_trigger_event_flag = 1U;
    g_last_trigger_timestamp = HAL_GetTick();
}

/* ============================================================================
 * FILTER CONFIGURATION FUNCTIONS (P2)
 * ============================================================================*/

/**
 * @brief Set filter type and update parameters
 */
void Optical_SetFilterType(Optical_FilterTypeDef filter_type)
{
    if (filter_type >= FILTER_COUNT) {
        return;
    }

    g_filter_params.type = filter_type;
    Optical_UpdateFilterParamsFromType(filter_type);
}

/**
 * @brief Get current filter parameters
 */
void Optical_GetFilterParams(Optical_FilterParamsTypeDef *params)
{
    if (params != NULL) {
        params->type = g_filter_params.type;
        params->center_wavelength_nm = g_filter_params.center_wavelength_nm;
        params->fwhm_nm = g_filter_params.fwhm_nm;
        params->bandwidth_nm = g_filter_params.bandwidth_nm;
        params->peak_transmission = g_filter_params.peak_transmission;
        params->cut_on_wavelength_nm = g_filter_params.cut_on_wavelength_nm;
        params->cut_off_wavelength_nm = g_filter_params.cut_off_wavelength_nm;
        params->blocking_od = g_filter_params.blocking_od;
    }
}

/**
 * @brief Set custom narrowband filter parameters
 */
int8_t Optical_SetCustomNarrowbandFilter(float center_nm,
                                          float fwhm_nm,
                                          float peak_transmission)
{
    if (fwhm_nm > FILTER_FWHM_MAX_NM) {
        return -1;
    }

    if (peak_transmission < 0.0f || peak_transmission > 1.0f) {
        return -1;
    }

    g_filter_params.center_wavelength_nm = center_nm;
    g_filter_params.fwhm_nm = fwhm_nm;
    g_filter_params.bandwidth_nm = fwhm_nm;
    g_filter_params.peak_transmission = peak_transmission;
    g_filter_params.cut_on_wavelength_nm = center_nm - fwhm_nm / 2.0f;
    g_filter_params.cut_off_wavelength_nm = center_nm + fwhm_nm / 2.0f;

    return 0;
}

/**
 * @brief Check if current filter is narrowband (FWHM <= 20nm)
 */
uint8_t Optical_IsFilterNarrowband(void)
{
    return (g_filter_params.fwhm_nm <= FILTER_FWHM_MAX_NM) ? 1U : 0U;
}

/* ============================================================================
 * NIR ENHANCEMENT FUNCTIONS
 * ============================================================================*/

/**
 * @brief Get current NIR enhancement configuration
 */
void Optical_GetNIRConfig(Optical_NIRConfigTypeDef *config)
{
    if (config != NULL) {
        config->nir_enhance_enabled = g_nir_config.nir_enhance_enabled;
        config->gain_factor = g_nir_config.gain_factor;
        config->offset_adjust = g_nir_config.offset_adjust;
        config->ambient_suppression = g_nir_config.ambient_suppression;
    }
}

/**
 * @brief Enable or disable NIR enhancement
 */
void Optical_SetNIREnhance(uint8_t enable)
{
    g_nir_config.nir_enhance_enabled = enable ? 1U : 0U;
}

/**
 * @brief Set NIR gain factor
 */
void Optical_SetNIRGain(float gain)
{
    if (gain > 0.0f) {
        g_nir_config.gain_factor = gain;
    }
}

/* ============================================================================
 * UTILITY FUNCTIONS
 * ============================================================================*/

/**
 * @brief Reset all optical configuration to defaults
 */
void Optical_ResetToDefaults(void)
{
    Optical_LoadDefaultWavelengthConfig();
    Optical_LoadDefaultModulationParams();
    Optical_LoadDefaultTriggerConfig();
    Optical_LoadDefaultFilterParams();
    Optical_LoadDefaultNIRConfig();
}

/**
 * @brief Get optical system status string for debugging
 */
void Optical_DebugStatus(char *status_str, uint32_t buf_len)
{
    if (status_str == NULL || buf_len == 0U) {
        return;
    }

    uint8_t narrowband_status = Optical_IsFilterNarrowband();
    Optical_WavelengthTypeDef current_wl;
    Optical_GetWavelength(&current_wl);

    (void)snprintf(status_str, (size_t)buf_len,
             "Opt[WL:%s,FWHM:%.1f,Narrow:%s,Mod:%luHz,D:%d%%,Sync:%s]",
             (current_wl == WAVELENGTH_850NM) ? "850nm" : "940nm",
             (double)g_filter_params.fwhm_nm,
             narrowband_status ? "Yes" : "No",
             (unsigned long)g_modulation_params.frequency_hz,
             g_modulation_params.duty_cycle_percent,
             g_trigger_config.sync_enabled ? "ON" : "OFF");
}

/**
 * @brief Calculate modulation timing from frequency
 */
void Optical_CalcModulationTiming(uint32_t frequency_hz,
                                  uint8_t duty_cycle_percent,
                                  uint32_t *period_us,
                                  uint32_t *pulse_width_us)
{
    if (period_us != NULL) {
        *period_us = (frequency_hz > 0U) ? (1000000U / frequency_hz) : 0U;
    }

    if (pulse_width_us != NULL) {
        if (period_us != NULL && *period_us > 0U) {
            *pulse_width_us = (*period_us * duty_cycle_percent) / 100U;
        } else {
            *pulse_width_us = 0U;
        }
    }
}

/* ============================================================================
 * PRIVATE FUNCTIONS - DEFAULT CONFIGURATION LOADERS
 * ============================================================================*/

static void Optical_LoadDefaultWavelengthConfig(void)
{
    g_wavelength_config.current_wavelength = WAVELENGTH_850NM;
    g_wavelength_config.supported_wavelengths[0] = WAVELENGTH_850NM;
    g_wavelength_config.supported_wavelengths[1] = WAVELENGTH_940NM;
    g_wavelength_config.wavelengths_nm[0] = WAVELENGTH_850NM_VALUE;
    g_wavelength_config.wavelengths_nm[1] = WAVELENGTH_940NM_VALUE;
    g_wavelength_config.wavelength_switch_enabled = 1U;
}

static void Optical_LoadDefaultModulationParams(void)
{
    g_modulation_params.frequency_hz = MODULATION_DEFAULT_FREQ_HZ;
    g_modulation_params.duty_cycle_percent = MODULATION_DEFAULT_DUTY_PCT;
    g_modulation_params.pulse_width_us = MODULATION_DEFAULT_PULSE_US;
    g_modulation_params.enabled = 0U;
}

static void Optical_LoadDefaultTriggerConfig(void)
{
    g_trigger_config.mode = TRIG_MODE_SOFTWARE;
    g_trigger_config.polarity = TRIGGER_DEFAULT_POLARITY;
    g_trigger_config.trigger_delay_us = TRIGGER_DEFAULT_DELAY_US;
    g_trigger_config.exposure_width_us = TRIGGER_DEFAULT_WIDTH_US;
    g_trigger_config.sync_enabled = 0U;
    g_trigger_config.trigger_filter_samples = TRIGGER_FILTER_SAMPLES;
}

static void Optical_LoadDefaultFilterParams(void)
{
    g_filter_params.type = FILTER_NARROWBAND_850NM;
    g_filter_params.center_wavelength_nm = FILTER_850NM_CENTER;
    g_filter_params.fwhm_nm = FILTER_850NM_FWHM;
    g_filter_params.bandwidth_nm = FILTER_850NM_BANDWIDTH;
    g_filter_params.peak_transmission = FILTER_PEAK_TRANSMISSION;
    g_filter_params.cut_on_wavelength_nm = (FILTER_850NM_CENTER - FILTER_850NM_FWHM / 2.0f);
    g_filter_params.cut_off_wavelength_nm = (FILTER_850NM_CENTER + FILTER_850NM_FWHM / 2.0f);
    g_filter_params.blocking_od = FILTER_BLOCKING_OD;
}

static void Optical_LoadDefaultNIRConfig(void)
{
    g_nir_config.nir_enhance_enabled = 1U;
    g_nir_config.gain_factor = NIR_DEFAULT_GAIN;
    g_nir_config.offset_adjust = 0.0f;
    g_nir_config.ambient_suppression = NIR_AMBIENT_SUPPRESSION;
}

/**
 * @brief Update filter parameters based on filter type
 */
static void Optical_UpdateFilterParamsFromType(Optical_FilterTypeDef filter_type)
{
    switch (filter_type) {
        case FILTER_NONE:
            g_filter_params.center_wavelength_nm = 0.0f;
            g_filter_params.fwhm_nm = 0.0f;
            g_filter_params.bandwidth_nm = 0.0f;
            g_filter_params.peak_transmission = 1.0f;
            g_filter_params.cut_on_wavelength_nm = 0.0f;
            g_filter_params.cut_off_wavelength_nm = 0.0f;
            g_filter_params.blocking_od = 0.0f;
            break;

        case FILTER_850NM_BANDPASS:
            g_filter_params.center_wavelength_nm = 850.0f;
            g_filter_params.fwhm_nm = 30.0f;
            g_filter_params.bandwidth_nm = 30.0f;
            g_filter_params.peak_transmission = 0.85f;
            g_filter_params.cut_on_wavelength_nm = 835.0f;
            g_filter_params.cut_off_wavelength_nm = 865.0f;
            g_filter_params.blocking_od = 3.0f;
            break;

        case FILTER_940NM_BANDPASS:
            g_filter_params.center_wavelength_nm = 940.0f;
            g_filter_params.fwhm_nm = 30.0f;
            g_filter_params.bandwidth_nm = 30.0f;
            g_filter_params.peak_transmission = 0.80f;
            g_filter_params.cut_on_wavelength_nm = 925.0f;
            g_filter_params.cut_off_wavelength_nm = 955.0f;
            g_filter_params.blocking_od = 3.0f;
            break;

        case FILTER_850NM_LONGPASS:
            g_filter_params.center_wavelength_nm = 900.0f;
            g_filter_params.fwhm_nm = 200.0f;
            g_filter_params.bandwidth_nm = 200.0f;
            g_filter_params.peak_transmission = 0.80f;
            g_filter_params.cut_on_wavelength_nm = 850.0f;
            g_filter_params.cut_off_wavelength_nm = 1100.0f;
            g_filter_params.blocking_od = 3.0f;
            break;

        case FILTER_NIR_ENHANCE:
            g_filter_params.center_wavelength_nm = 850.0f;
            g_filter_params.fwhm_nm = 50.0f;
            g_filter_params.bandwidth_nm = 50.0f;
            g_filter_params.peak_transmission = 0.90f;
            g_filter_params.cut_on_wavelength_nm = 825.0f;
            g_filter_params.cut_off_wavelength_nm = 875.0f;
            g_filter_params.blocking_od = 3.0f;
            break;

        case FILTER_NARROWBAND_850NM:
            g_filter_params.center_wavelength_nm = FILTER_850NM_CENTER;
            g_filter_params.fwhm_nm = FILTER_850NM_FWHM;
            g_filter_params.bandwidth_nm = FILTER_850NM_BANDWIDTH;
            g_filter_params.peak_transmission = FILTER_PEAK_TRANSMISSION;
            g_filter_params.cut_on_wavelength_nm = (FILTER_850NM_CENTER - FILTER_850NM_FWHM / 2.0f);
            g_filter_params.cut_off_wavelength_nm = (FILTER_850NM_CENTER + FILTER_850NM_FWHM / 2.0f);
            g_filter_params.blocking_od = FILTER_BLOCKING_OD;
            break;

        case FILTER_NARROWBAND_940NM:
            g_filter_params.center_wavelength_nm = FILTER_940NM_CENTER;
            g_filter_params.fwhm_nm = FILTER_940NM_FWHM;
            g_filter_params.bandwidth_nm = FILTER_940NM_BANDWIDTH;
            g_filter_params.peak_transmission = FILTER_PEAK_TRANSMISSION;
            g_filter_params.cut_on_wavelength_nm = (FILTER_940NM_CENTER - FILTER_940NM_FWHM / 2.0f);
            g_filter_params.cut_off_wavelength_nm = (FILTER_940NM_CENTER + FILTER_940NM_FWHM / 2.0f);
            g_filter_params.blocking_od = FILTER_BLOCKING_OD;
            break;

        default:
            break;
    }
}