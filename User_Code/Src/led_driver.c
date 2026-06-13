/**
 * @file led_driver.c
 * @brief LED Driver Implementation - Controls 5x 850nm IR LEDs with PWM modulation
 * @author Development Team
 *
 * Features:
 * - 5x 850nm IR LEDs in pentagon arrangement
 * - PWM modulation (20kHz default, 10kHz-1MHz range)
 * - Breathing mode (sine wave brightness)
 * - Coding mode (diameter/brightness classification)
 * - Active-low output (LED on when GPIO reset)
 */
#include "led_driver.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>

/* Private variables ---------------------------------------------------------*/
static LED_StatusTypedef g_led_status = {
    .mode = LED_MODE_OFF,
    .enabled_mask = LED_MASK_NONE,
    .flash_period_ms = 100U,
    .flash_duty_cycle = 50U,
    .modulation_params = {
        .frequency_hz = MODULATION_DEFAULT_FREQ_HZ,
        .duty_cycle_percent = MODULATION_DEFAULT_DUTY_PCT,
        .pulse_width_us = MODULATION_DEFAULT_PULSE_US,
        .enabled = 0U
    },
    .wavelength = WAVELENGTH_850NM
};

static TIM_HandleTypeDef g_pwm_tim_handle;
static uint8_t g_breathing_phase = 0U;
static uint32_t g_breathing_accumulator = 0U;

/* Sync mode variables */
static uint8_t g_sync_mode_enabled = 0U;
static Optical_TriggerPolarityTypeDef g_sync_polarity = TRIG_POLARITY_ACTIVE_HIGH;
static uint16_t g_sync_pulse_width_us = 100U;
static uint16_t g_sync_delay_us = 10U;

/* Private function prototypes -----------------------------------------------*/
static void LED_ConfigureGPIOs(void);
static void LED_ConfigurePwmTimer(void);
static void LED_WriteEnableWord(LED_EnableWordType enable_word);
static uint8_t LED_UpdatePwmFrequencyInternal(uint32_t frequency_hz);
static void LED_UpdatePwmDutyCycleInternal(uint8_t duty_cycle_percent);
static void LED_GenerateBreathingStep(void);

/* GPIO definitions for 5 LEDs in pentagon arrangement */
#define LED0_PORT GPIOB
#define LED0_PIN   GPIO_PIN_0
#define LED1_PORT GPIOB
#define LED1_PIN   GPIO_PIN_1
#define LED2_PORT GPIOB
#define LED2_PIN   GPIO_PIN_2
#define LED3_PORT GPIOB
#define LED3_PIN   GPIO_PIN_3
#define LED4_PORT GPIOB
#define LED4_PIN   GPIO_PIN_4

/* Timer clock: APB1 = 42MHz (168MHz / 4) */
#define LED_APB_CLOCK   42000000U

/* ============================================================================
 * INITIALIZATION FUNCTIONS
 * ============================================================================*/

/**
 * @brief Initialize LED driver
 * @retval LED_STATUS_OK on success
 */
LED_StatusTypeDef LED_Init(void)
{
    /* Enable GPIO clocks */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Configure GPIO pins */
    LED_ConfigureGPIOs();

    /* All LEDs off initially (active low) */
    LED_WriteEnableWord(LED_MASK_NONE);

    /* Configure PWM timer for modulation */
    LED_ConfigurePwmTimer();

    /* Initialize sync mode */
    g_sync_mode_enabled = 0U;
    g_sync_polarity = TRIG_POLARITY_ACTIVE_HIGH;
    g_sync_pulse_width_us = 100U;
    g_sync_delay_us = 10U;

    /* Initialize breathing */
    g_breathing_phase = 0U;
    g_breathing_accumulator = 0U;

    /* Update status */
    g_led_status.mode = LED_MODE_OFF;
    g_led_status.enabled_mask = LED_MASK_NONE;

    return LED_STATUS_OK;
}

/**
 * @brief Deinitialize LED driver
 */
LED_StatusTypeDef LED_DeInit(void)
{
    LED_WriteEnableWord(LED_MASK_NONE);

    if (g_pwm_tim_handle.Instance == TIM2) {
        (void)HAL_TIM_PWM_Stop(&g_pwm_tim_handle, TIM_CHANNEL_1);
    }

    HAL_GPIO_DeInit(LED0_PORT, LED0_PIN | LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN);

    g_led_status.mode = LED_MODE_OFF;

    return LED_STATUS_OK;
}

/**
 * @brief Configure GPIO pins for LED control
 */
static void LED_ConfigureGPIOs(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    GPIO_InitStruct.Pin = LED0_PIN;
    HAL_GPIO_Init(LED0_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED1_PIN;
    HAL_GPIO_Init(LED1_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED2_PIN;
    HAL_GPIO_Init(LED2_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED3_PIN;
    HAL_GPIO_Init(LED3_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LED4_PIN;
    HAL_GPIO_Init(LED4_PORT, &GPIO_InitStruct);
}

/**
 * @brief Configure PWM timer for high-frequency modulation
 */
static void LED_ConfigurePwmTimer(void)
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    g_pwm_tim_handle.Instance = TIM2;
    g_pwm_tim_handle.Init.Prescaler = 0U;
    g_pwm_tim_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_pwm_tim_handle.Init.Period = 2099U;  /* Will be adjusted by frequency update */
    g_pwm_tim_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    g_pwm_tim_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    if (HAL_TIM_PWM_Init(&g_pwm_tim_handle) != HAL_OK) {
        return;
    }

    TIM_OC_InitTypeDef sConfig = {0};
    sConfig.Pulse = 0U;
    sConfig.OCMode = TIM_OCMODE_PWM1;
    sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfig.OCFastMode = TIM_OCFAST_DISABLE;

    if (HAL_TIM_PWM_ConfigChannel(&g_pwm_tim_handle, &sConfig, TIM_CHANNEL_1) != HAL_OK) {
        return;
    }

    /* Default frequency 20kHz */
    (void)LED_UpdatePwmFrequencyInternal(MODULATION_DEFAULT_FREQ_HZ);
}

/* ============================================================================
 * LED CONTROL FUNCTIONS
 * ============================================================================*/

/**
 * @brief Set LED enable word
 */
void LED_SetEnableWord(LED_EnableWordType enable_word)
{
    g_led_status.enabled_mask = enable_word & LED_MASK_ALL;
    LED_WriteEnableWord(g_led_status.enabled_mask);
}

/**
 * @brief Get LED enable word
 */
LED_EnableWordType LED_GetEnableWord(void)
{
    return g_led_status.enabled_mask;
}

/**
 * @brief Write enable word to GPIO pins (active low)
 */
static void LED_WriteEnableWord(LED_EnableWordType enable_word)
{
    HAL_GPIO_WritePin(LED0_PORT, LED0_PIN,
        (enable_word & LED_BIT_0) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED1_PORT, LED1_PIN,
        (enable_word & LED_BIT_1) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED2_PORT, LED2_PIN,
        (enable_word & LED_BIT_2) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED3_PORT, LED3_PIN,
        (enable_word & LED_BIT_3) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(LED4_PORT, LED4_PIN,
        (enable_word & LED_BIT_4) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

/**
 * @brief Set LED operation mode
 */
void LED_SetMode(LED_ModeTypeDef mode)
{
    if (mode >= LED_MODE_COUNT) {
        return;
    }

    g_led_status.mode = mode;

    switch (mode) {
        case LED_MODE_OFF:
            LED_StopModulated();
            LED_WriteEnableWord(LED_MASK_NONE);
            break;

        case LED_MODE_CONSTANT:
            LED_StopModulated();
            LED_WriteEnableWord(g_led_status.enabled_mask);
            break;

        case LED_MODE_DARK_FLASH:
            LED_WriteEnableWord(g_led_status.enabled_mask);
            break;

        case LED_MODE_BREATHING:
            LED_StopModulated();
            LED_WriteEnableWord(g_led_status.enabled_mask);
            g_breathing_phase = 0U;
            break;

        case LED_MODE_ALERT:
            LED_StopModulated();
            LED_WriteEnableWord(LED_MASK_ALL);
            break;

        case LED_MODE_MODULATED:
            LED_SetModulationEnable(1U);
            LED_StartModulated();
            break;

        case LED_MODE_SYNC_TRIGGER:
            LED_StopModulated();
            LED_SetSyncMode(1U, g_sync_polarity);
            break;

        default:
            break;
    }
}

/**
 * @brief Get current LED mode
 */
LED_ModeTypeDef LED_GetMode(void)
{
    return g_led_status.mode;
}

/**
 * @brief Get current LED status
 */
void LED_GetStatus(LED_StatusTypedef *status)
{
    if (status != NULL) {
        status->mode = g_led_status.mode;
        status->enabled_mask = g_led_status.enabled_mask;
        status->flash_period_ms = g_led_status.flash_period_ms;
        status->flash_duty_cycle = g_led_status.flash_duty_cycle;
        status->modulation_params = g_led_status.modulation_params;
        status->wavelength = g_led_status.wavelength;
    }
}

/**
 * @brief Set flash parameters for dark flash sync mode
 */
void LED_SetFlashParams(uint32_t period_ms, uint8_t duty_cycle)
{
    g_led_status.flash_period_ms = period_ms;
    g_led_status.flash_duty_cycle = (duty_cycle > 100U) ? 100U : duty_cycle;
}

/* ============================================================================
 * PWM MODULATION FUNCTIONS
 * ============================================================================*/

/**
 * @brief Set PWM modulation parameters
 */
LED_StatusTypeDef LED_SetModulation(uint32_t frequency_hz,
                                      uint8_t duty_cycle_percent,
                                      uint16_t pulse_width_us)
{
    /* Validate frequency (10kHz - 1MHz) */
    if (frequency_hz < MODULATION_FREQ_MIN_HZ || frequency_hz > MODULATION_FREQ_MAX_HZ) {
        return LED_STATUS_ERROR;
    }

    /* Validate pulse width (<= 100us) */
    if (pulse_width_us > MODULATION_PULSE_WIDTH_MAX_US) {
        return LED_STATUS_ERROR;
    }

    /* Calculate period and verify pulse width */
    uint32_t period_us = 1000000U / frequency_hz;
    if (pulse_width_us >= period_us) {
        return LED_STATUS_ERROR;
    }

    /* Update parameters */
    g_led_status.modulation_params.frequency_hz = frequency_hz;
    g_led_status.modulation_params.duty_cycle_percent = duty_cycle_percent;
    g_led_status.modulation_params.pulse_width_us = pulse_width_us;

    /* If already in modulated mode, update hardware */
    if (g_led_status.mode == LED_MODE_MODULATED) {
        (void)LED_UpdatePwmFrequencyInternal(frequency_hz);
        LED_UpdatePwmDutyCycleInternal(duty_cycle_percent);
    }

    return LED_STATUS_OK;
}

/**
 * @brief Get current modulation parameters
 */
void LED_GetModulationParams(Optical_ModulationParamsTypeDef *params)
{
    if (params != NULL) {
        *params = g_led_status.modulation_params;
    }
}

/**
 * @brief Enable or disable modulated mode
 */
void LED_SetModulationEnable(uint8_t enable)
{
    g_led_status.modulation_params.enabled = enable ? 1U : 0U;
}

/**
 * @brief Get modulation enable state
 */
uint8_t LED_GetModulationEnable(void)
{
    return g_led_status.modulation_params.enabled;
}

/**
 * @brief Start modulated LED output
 */
void LED_StartModulated(void)
{
    if (!g_led_status.modulation_params.enabled) {
        return;
    }

    /* Update PWM frequency and duty cycle */
    (void)LED_UpdatePwmFrequencyInternal(g_led_status.modulation_params.frequency_hz);
    LED_UpdatePwmDutyCycleInternal(g_led_status.modulation_params.duty_cycle_percent);

    /* Enable all selected LEDs */
    LED_WriteEnableWord(g_led_status.enabled_mask);

    /* Start PWM output */
    (void)HAL_TIM_PWM_Start(&g_pwm_tim_handle, TIM_CHANNEL_1);
}

/**
 * @brief Stop modulated LED output
 */
void LED_StopModulated(void)
{
    (void)HAL_TIM_PWM_Stop(&g_pwm_tim_handle, TIM_CHANNEL_1);
    LED_WriteEnableWord(LED_MASK_NONE);
    g_led_status.modulation_params.enabled = 0U;
}

/**
 * @brief Internal frequency update
 */
static uint8_t LED_UpdatePwmFrequencyInternal(uint32_t frequency_hz)
{
    /* Validate frequency bounds */
    if (frequency_hz < MODULATION_FREQ_MIN_HZ || frequency_hz > MODULATION_FREQ_MAX_HZ) {
        return 0U;
    }

    /* Calculate period for desired frequency
     * Timer clock = APB1 = 42MHz
     * Period = timer_clock / frequency - 1
     */
    uint32_t period = LED_APB_CLOCK / frequency_hz;

    /* Clamp period to valid range (min 2, max 65535) */
    if (period < 2U) {
        period = 2U;
    } else if (period > 65535U) {
        period = 65535U;
    }

    /* Update timer registers */
    __HAL_TIM_SET_PRESCALER(&g_pwm_tim_handle, 0U);
    __HAL_TIM_SET_AUTORELOAD(&g_pwm_tim_handle, period - 1U);

    return 1U;
}

/**
 * @brief Internal duty cycle update
 */
static void LED_UpdatePwmDutyCycleInternal(uint8_t duty_cycle_percent)
{
    uint32_t period = __HAL_TIM_GET_AUTORELOAD(&g_pwm_tim_handle) + 1U;
    uint32_t pulse = (period * duty_cycle_percent) / 100U;

    if (pulse >= period) {
        pulse = period - 1U;
    }

    __HAL_TIM_SET_COMPARE(&g_pwm_tim_handle, TIM_CHANNEL_1, pulse);
}

/**
 * @brief Update PWM frequency (public interface)
 */
LED_StatusTypeDef LED_UpdatePwmFrequency(uint32_t frequency_hz)
{
    if (LED_UpdatePwmFrequencyInternal(frequency_hz)) {
        g_led_status.modulation_params.frequency_hz = frequency_hz;
        return LED_STATUS_OK;
    }
    return LED_STATUS_ERROR;
}

/**
 * @brief Generate one step of breathing animation
 * @note Uses sine wave approximation: brightness = 50% * sin(phase) + 50%
 */
static void LED_GenerateBreathingStep(void)
{
    /* Sine wave approximation using incremental algorithm
     * phase advances by 2*pi/256 per call (~1.4 degrees)
     * We use quarter-wave table lookup for efficiency
     */
    static const uint8_t sin_table[64] = {
        0,   12,  25,  37,  49,  60,  71,  82,
        92,  101, 110, 118, 126, 133, 139, 145,
        150, 154, 158, 162, 164, 166, 168, 169,
        170, 170, 169, 168, 166, 164, 162, 158,
        154, 150, 145, 139, 133, 126, 118, 110,
        101,  92,  82,  71,  60,  49,  37,  25,
         12,   0
    };

    uint8_t phase_mod = g_breathing_phase & 0xFFU;
    uint8_t table_idx;
    uint8_t brightness;

    /* Map phase to table index (0-255 -> 0-63) */
    if (phase_mod < 128U) {
        table_idx = phase_mod >> 1U;  /* 0-127 -> 0-63 */
    } else {
        table_idx = (255U - phase_mod) >> 1U;  /* 128-255 -> 63-0 */
    }

    /* Get brightness from table (0-170 range) */
    brightness = sin_table[table_idx & 0x3FU];

    /* Scale to 1-100% range */
    brightness = (uint8_t)((((uint16_t)brightness) * 100U) / 170U);

    /* Apply to LED enable pattern
     * In breathing mode, we modulate all enabled LEDs together
     * Use PWM duty cycle for smooth breathing effect
     */
    LED_UpdatePwmDutyCycleInternal(brightness);

    /* Advance phase */
    g_breathing_phase += 4U;  /* ~1.6 degrees per call at 60Hz update rate */
}

/* ============================================================================
 * WAVELENGTH CONTROL FUNCTIONS
 * ============================================================================*/

/**
 * @brief Set LED wavelength
 */
LED_StatusTypeDef LED_SetWavelength(Optical_WavelengthTypeDef wavelength)
{
    if (wavelength >= WAVELENGTH_COUNT) {
        return LED_STATUS_ERROR;
    }

    if (!LED_IsWavelengthSupported(wavelength)) {
        return LED_STATUS_ERROR;
    }

    g_led_status.wavelength = wavelength;
    return LED_STATUS_OK;
}

/**
 * @brief Get current LED wavelength
 */
void LED_GetWavelength(Optical_WavelengthTypeDef *wavelength)
{
    if (wavelength != NULL) {
        *wavelength = g_led_status.wavelength;
    }
}

/**
 * @brief Check if wavelength is supported
 */
uint8_t LED_IsWavelengthSupported(Optical_WavelengthTypeDef wavelength)
{
    /* 850nm supported, 940nm for future expansion */
    if (wavelength == WAVELENGTH_850NM) {
        return 1U;
    }
    if (wavelength == WAVELENGTH_940NM) {
        return 1U;  /* Software support for future hardware */
    }
    return 0U;
}

/* ============================================================================
 * SYNCHRONIZATION CONTROL FUNCTIONS
 * ============================================================================*/

/**
 * @brief Configure LED for external sync trigger mode
 */
void LED_SetSyncMode(uint8_t enable, Optical_TriggerPolarityTypeDef polarity)
{
    g_sync_mode_enabled = enable ? 1U : 0U;
    g_sync_polarity = polarity;
}

/**
 * @brief Set LED sync pulse parameters
 */
void LED_SetSyncPulseParams(uint16_t pulse_width_us, uint16_t delay_us)
{
    g_sync_pulse_width_us = pulse_width_us;
    g_sync_delay_us = delay_us;
}

/**
 * @brief Get sync mode status
 */
uint8_t LED_GetSyncMode(Optical_TriggerPolarityTypeDef *polarity)
{
    if (polarity != NULL) {
        *polarity = g_sync_polarity;
    }
    return g_sync_mode_enabled;
}

/* ============================================================================
 * PWM TIMER ACCESS FUNCTIONS
 * ============================================================================*/

/**
 * @brief Get PWM timer handle
 */
TIM_HandleTypeDef* LED_GetPwmTimerHandle(void)
{
    return &g_pwm_tim_handle;
}

/* ============================================================================
 * DEBUG FUNCTIONS
 * ============================================================================*/

/**
 * @brief Get LED driver status string for debugging
 */
void LED_DebugStatus(char *status_str, uint32_t buf_len)
{
    if (status_str == NULL || buf_len == 0U) {
        return;
    }

    (void)snprintf(status_str, (size_t)buf_len,
             "LED[M:%d,E:0x%02X,WL:%s,F:%luHz,D:%d%%,Sync:%s]",
             g_led_status.mode,
             g_led_status.enabled_mask,
             (g_led_status.wavelength == WAVELENGTH_850NM) ? "850" : "940",
             (unsigned long)g_led_status.modulation_params.frequency_hz,
             g_led_status.modulation_params.duty_cycle_percent,
             g_sync_mode_enabled ? "ON" : "OFF");
}