/**
 * @file camera_driver.c
 * @brief Camera Driver Implementation - OV2640 8-bit grayscale 320x240
 * @author Development Team
 *
 * Features:
 * - DCMI + DMA for frame capture
 * - NIR mode support (850nm enhanced)
 * - 16-bit frame buffer support for NIR high-resolution mode
 * - External trigger synchronization
 */
#include "camera_driver.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>

/* Private variables ---------------------------------------------------------*/
static Camera_StatusTypeDef g_cam_status = CAM_STATUS_NOT_INIT;
static Camera_FrameTypeDef g_current_frame;
static Camera_CallbackTypeDef g_frame_callback = NULL;
static uint8_t g_capture_in_progress = 0U;
static Camera_ExposureTypeDef g_exposure_level = CAM_EXPOSURE_MEDIUM;
static Camera_NIRModeTypeDef g_nir_mode = CAM_NIR_MODE_OFF;
static uint8_t g_use_16bit = 0U;

/* Frame buffers */
static uint8_t g_frame_buffer[CAM_FRAME_SIZE];       /* 8-bit: 320x240 */
static uint16_t g_frame_buffer_16b[CAM_FRAME_SIZE];   /* 16-bit: 320x240 */

/* DCMI and DMA handles */
static DCMI_HandleTypeDef g_dcmi_handle;
static DMA_HandleTypeDef g_hdma_dcmi;

/* I2C handle for OV2640 configuration (external) */
extern I2C_HandleTypeDef hi2c1;

/* OV2640 I2C slave address (7-bit) */
#define OV2640_I2C_ADDR  0x60U

/* Trigger/sync private variables */
static Camera_TriggerModeTypeDef g_trigger_mode = CAM_TRIG_MODE_SOFTWARE;
static Optical_TriggerPolarityTypeDef g_trigger_polarity = TRIG_POLARITY_ACTIVE_HIGH;
static uint8_t g_sync_enabled = 0U;
static uint16_t g_trigger_delay_us = 10U;
static uint16_t g_exposure_width_us = 100U;
static volatile uint32_t g_trigger_count = 0U;
static volatile uint32_t g_last_trigger_timestamp = 0U;
static volatile uint8_t g_trigger_event_flag = 0U;
static Camera_TriggerCallbackTypeDef g_trigger_callback = NULL;

/* Private function prototypes -----------------------------------------------*/
static void Camera_ConfigureExtTrigger(void);
static void Camera_I2C_Write(uint8_t reg_addr, uint8_t reg_value);
static uint8_t Camera_I2C_Read(uint8_t reg_addr);
static void Camera_ConfigureOV2640(void);
static void Camera_ConfigureNIR(void);
static void Camera_Configure16BitMode(void);
static void Camera_StartDMA(void);

/* ============================================================================
 * INITIALIZATION FUNCTIONS
 * ============================================================================*/

/**
 * @brief Initialize camera driver
 */
Camera_StatusTypeDef Camera_Init(void)
{
    return Camera_InitEx(CAM_NIR_MODE_OFF, 0U);
}

/**
 * @brief Deinitialize camera driver
 */
Camera_StatusTypeDef Camera_DeInit(void)
{
    if (g_capture_in_progress) {
        (void)HAL_DCMI_Stop(&g_dcmi_handle);
        g_capture_in_progress = 0U;
    }

    (void)HAL_DMA_DeInit(&g_hdma_dcmi);
    (void)HAL_DCMI_DeInit(&g_dcmi_handle);

    HAL_GPIO_DeInit(CAM_EXT_TRIGGER_PORT, CAM_EXT_TRIGGER_PIN);

    g_cam_status = CAM_STATUS_NOT_INIT;
    return CAM_STATUS_OK;
}

/**
 * @brief Extended camera initialization with NIR mode and 16-bit support
 */
Camera_StatusTypeDef Camera_InitEx(Camera_NIRModeTypeDef nir_mode, uint8_t use_16bit)
{
    g_nir_mode = nir_mode;
    g_use_16bit = use_16bit;

    /* Configure DCMI peripheral */
    g_dcmi_handle.Instance = DCMI;
    g_dcmi_handle.Init.CaptureRate = DCMI_CR_ALL_FRAME;
    g_dcmi_handle.Init.VSPolarity = DCMI_VSPOLARITY_HIGH;
    g_dcmi_handle.Init.HSPolarity = DCMI_HSPOLARITY_HIGH;
    g_dcmi_handle.Init.PCKPolarity = DCMI_PCKPOLARITY_RISING;
    g_dcmi_handle.Init.ByteSelectMode = DCMI_BSM_8BIT;
    g_dcmi_handle.Init.ByteSelectStart = DCMI_OEBS_ODD;
    g_dcmi_handle.Init.LineSelectMode = DCMI_LSM_ALL;

    if (use_16bit) {
        g_dcmi_handle.Init.ExtendedDataMode = DCMI_EDM_16BIT;
    } else {
        g_dcmi_handle.Init.ExtendedDataMode = DCMI_EDM_8BIT;
    }

    if (HAL_DCMI_Init(&g_dcmi_handle) != HAL_OK) {
        g_cam_status = CAM_STATUS_ERROR;
        return CAM_STATUS_ERROR;
    }

    /* Configure DMA */
    g_hdma_dcmi.Instance = DMA2_Stream1;
    g_hdma_dcmi.Init.Channel = DMA_CHANNEL_1;
    g_hdma_dcmi.Init.Direction = DMA_PERIPH_TO_MEMORY;
    g_hdma_dcmi.Init.PeriphInc = DMA_PINC_DISABLE;
    g_hdma_dcmi.Init.MemInc = DMA_MINC_ENABLE;

    if (use_16bit) {
        g_hdma_dcmi.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
        g_hdma_dcmi.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    } else {
        g_hdma_dcmi.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        g_hdma_dcmi.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    }

    g_hdma_dcmi.Init.Mode = DMA_CIRCULAR;
    g_hdma_dcmi.Init.Priority = DMA_PRIORITY_HIGH;
    g_hdma_dcmi.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    g_hdma_dcmi.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    g_hdma_dcmi.Init.MemBurst = DMA_MBURST_INC4;
    g_hdma_dcmi.Init.PeriphBurst = DMA_PBURST_SINGLE;

    if (HAL_DMA_Init(&g_hdma_dcmi) != HAL_OK) {
        g_cam_status = CAM_STATUS_ERROR;
        return CAM_STATUS_ERROR;
    }

    /* Link DMA to DCMI */
    __HAL_LINKDMA(&g_dcmi_handle, DMA_Handle, g_hdma_dcmi);

    /* Configure external trigger GPIO */
    Camera_ConfigureExtTrigger();

    /* Configure OV2640 camera */
    Camera_ConfigureOV2640();

    /* Apply NIR enhancement if enabled */
    if (nir_mode != CAM_NIR_MODE_OFF) {
        Camera_ConfigureNIR();
    }

    /* Apply 16-bit mode configuration if enabled */
    if (use_16bit) {
        Camera_Configure16BitMode();
    }

    /* Initialize frame structure */
    if (use_16bit) {
        g_current_frame.frame_buffer = g_frame_buffer_16b;
        g_current_frame.data_type = 1U;
    } else {
        g_current_frame.frame_buffer = g_frame_buffer;
        g_current_frame.data_type = 0U;
    }
    g_current_frame.width = CAM_FRAME_WIDTH;
    g_current_frame.height = CAM_FRAME_HEIGHT;
    g_current_frame.is_ready = 0U;
    g_current_frame.timestamp = 0U;

    /* Initialize sync/trigger defaults */
    g_trigger_mode = CAM_TRIG_MODE_SOFTWARE;
    g_trigger_polarity = TRIG_POLARITY_ACTIVE_HIGH;
    g_sync_enabled = 0U;
    g_trigger_delay_us = 10U;
    g_exposure_width_us = 100U;
    g_trigger_count = 0U;
    g_trigger_event_flag = 0U;

    g_cam_status = CAM_STATUS_OK;
    return CAM_STATUS_OK;
}

/**
 * @brief Configure external trigger GPIO
 */
static void Camera_ConfigureExtTrigger(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = CAM_EXT_TRIGGER_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(CAM_EXT_TRIGGER_PORT, &GPIO_InitStruct);
}

/* ============================================================================
 * OV2640 CONFIGURATION FUNCTIONS
 * ============================================================================*/

/**
 * @brief Write to OV2640 register via I2C
 */
static void Camera_I2C_Write(uint8_t reg_addr, uint8_t reg_value)
{
    (void)HAL_I2C_Mem_Write(&hi2c1,
                            (uint16_t)(OV2640_I2C_ADDR << 1),
                            reg_addr,
                            I2C_MEMADD_SIZE_8BIT,
                            &reg_value,
                            1U,
                            HAL_MAX_DELAY);
}

/**
 * @brief Read from OV2640 register via I2C
 */
static uint8_t Camera_I2C_Read(uint8_t reg_addr)
{
    uint8_t reg_value = 0U;
    (void)HAL_I2C_Mem_Read(&hi2c1,
                           (uint16_t)(OV2640_I2C_ADDR << 1),
                           reg_addr,
                           I2C_MEMADD_SIZE_8BIT,
                           &reg_value,
                           1U,
                           HAL_MAX_DELAY);
    return reg_value;
}

/**
 * @brief Configure OV2640 for 8-bit grayscale 320x240
 */
static void Camera_ConfigureOV2640(void)
{
    /* Reset OV2640 */
    Camera_I2C_Write(0xFF, 0x01);  /* Select bank 1 */
    Camera_I2C_Write(0x12, 0x80);  /* Reset */
    HAL_Delay(10);

    /* Configure for 320x240 Y8 (8-bit grayscale) mode */
    Camera_I2C_Write(0xFF, 0x00);

    /* Set COM7 for UXGA (320x240) */
    Camera_I2C_Write(0x12, 0x00);

    /* CLKRC - clock prescaler */
    Camera_I2C_Write(0x11, 0x00);  /* CLKRC = 0, no prescaler */

    /* COM1 - special effects */
    Camera_I2C_Write(0x09, 0x00);

    /* R_DFT_LTH - black sun threshold */
    Camera_I2C_Write(0xA2, 0x00);

    /* COM8 - automatic exposure */
    Camera_I2C_Write(0x13, 0x00);

    /* COM10 - DVP and VSYNC settings */
    Camera_I2C_Write(0x15, 0x00);

    /* Select bank 1 for DSP control */
    Camera_I2C_Write(0xFF, 0x01);

    /* Enable 8-bit grayscale output */
    Camera_I2C_Write(0x12, 0x08);

    /* Image window - 320x240 */
    Camera_I2C_Write(0x04, 0x08);  /* CW_ST = 8 */
    Camera_I2C_Write(0x05, 0x80);  /* CW_ST high */
    Camera_I2C_Write(0x06, 0x10);  /* CW_EN */

    Camera_I2C_Write(0x51, 0x40);  /* HREFST[7:0] */
    Camera_I2C_Write(0x52, 0x00);  /* HREFST[11:8] */
    Camera_I2C_Write(0x53, 0x40);  /* HREFEND[7:0] */
    Camera_I2C_Write(0x54, 0x00);  /* HREFEND[11:8] */
    Camera_I2C_Write(0x55, 0xF0);  /* VST[7:0] */
    Camera_I2C_Write(0x56, 0x00);  /* VST[11:8] */
    Camera_I2C_Write(0x57, 0x00);  /* VEND[7:0] */
    Camera_I2C_Write(0x58, 0x00);  /* VEND[11:8] */

    /* Select back to bank 0 */
    Camera_I2C_Write(0xFF, 0x00);
}

/**
 * @brief Configure NIR enhancement mode for 850nm
 */
static void Camera_ConfigureNIR(void)
{
    Camera_I2C_Write(0xFF, 0x00);

    /* COM8: AEC on, AGC off, AWB on */
    Camera_I2C_Write(0x13, 0xE0);

    /* COM7: UXGA mode */
    Camera_I2C_Write(0x12, 0x00);

    /* Vertical exposure control - increase for NIR */
    Camera_I2C_Write(0x3D, 0xC0);

    /* Horizontal window for NIR averaging */
    Camera_I2C_Write(0xFF, 0x01);
    Camera_I2C_Write(0x51, 0x30);
    Camera_I2C_Write(0x52, 0x00);
    Camera_I2C_Write(0x53, 0x50);
    Camera_I2C_Write(0x54, 0x00);

    /* Vertical window for NIR */
    Camera_I2C_Write(0x55, 0xE0);
    Camera_I2C_Write(0x56, 0x00);
    Camera_I2C_Write(0x57, 0x10);
    Camera_I2C_Write(0x58, 0x00);

    Camera_I2C_Write(0xFF, 0x00);

    g_nir_mode = CAM_NIR_MODE_850NM;
}

/**
 * @brief Configure 16-bit mode for enhanced dynamic range
 */
static void Camera_Configure16BitMode(void)
{
    g_current_frame.frame_buffer = g_frame_buffer_16b;
    g_current_frame.data_type = 1U;
    g_use_16bit = 1U;

    Camera_I2C_Write(0xFF, 0x00);

    /* VSAT: Saturation control - higher for NIR */
    Camera_I2C_Write(0x3C, 0x50);

    Camera_I2C_Write(0xFF, 0x00);
}

/* ============================================================================
 * BASIC CAMERA CONTROL FUNCTIONS
 * ============================================================================*/

/**
 * @brief Set camera NIR enhancement mode
 */
void Camera_SetNIRMode(Camera_NIRModeTypeDef mode)
{
    g_nir_mode = mode;

    if (mode != CAM_NIR_MODE_OFF) {
        Camera_ConfigureNIR();
    }
}

/**
 * @brief Get current NIR mode
 */
Camera_NIRModeTypeDef Camera_GetNIRMode(void)
{
    return g_nir_mode;
}

/**
 * @brief Set camera exposure level
 */
void Camera_SetExposure(Camera_ExposureTypeDef exposure)
{
    g_exposure_level = exposure;

    Camera_I2C_Write(0xFF, 0x01);

    switch (exposure) {
        case CAM_EXPOSURE_LOW:
            Camera_I2C_Write(0x13, 0x00);
            break;

        case CAM_EXPOSURE_MEDIUM:
            Camera_I2C_Write(0x13, 0x01);
            break;

        case CAM_EXPOSURE_HIGH:
            Camera_I2C_Write(0x13, 0x02);
            break;

        case CAM_EXPOSURE_AUTO:
            Camera_I2C_Write(0x13, 0x05);
            break;

        default:
            break;
    }
}

/**
 * @brief Start DMA transfer for frame capture
 */
static void Camera_StartDMA(void)
{
    uint32_t buffer_addr;
    uint32_t buffer_size;

    if (g_use_16bit) {
        buffer_addr = (uint32_t)g_frame_buffer_16b;
        buffer_size = CAM_FRAME_SIZE * 2U;  /* 16-bit per pixel */
    } else {
        buffer_addr = (uint32_t)g_frame_buffer;
        buffer_size = CAM_FRAME_SIZE;  /* 8-bit per pixel */
    }

    (void)HAL_DCMI_Start_DMA(&g_dcmi_handle, buffer_addr, buffer_size);
}

/**
 * @brief Start single frame capture
 */
Camera_StatusTypeDef Camera_StartCapture(void)
{
    if (g_capture_in_progress) {
        return CAM_STATUS_BUSY;
    }

    /* In sync mode, check if trigger is active */
    if (g_sync_enabled && g_trigger_mode != CAM_TRIG_MODE_SOFTWARE) {
        if (!g_trigger_event_flag) {
            return CAM_STATUS_TIMEOUT;
        }
        g_trigger_event_flag = 0U;
    }

    /* Reset frame ready flag */
    g_current_frame.is_ready = 0U;

    /* Start DMA transfer */
    Camera_StartDMA();

    g_capture_in_progress = 1U;
    return CAM_STATUS_OK;
}

/**
 * @brief Get the most recent frame
 */
Camera_StatusTypeDef Camera_GetFrame(Camera_FrameTypeDef *frame)
{
    if (frame == NULL) {
        return CAM_STATUS_ERROR;
    }

    if (!g_current_frame.is_ready) {
        return CAM_STATUS_TIMEOUT;
    }

    /* Copy frame data */
    frame->frame_buffer = g_current_frame.frame_buffer;
    frame->width = g_current_frame.width;
    frame->height = g_current_frame.height;
    frame->timestamp = g_current_frame.timestamp;
    frame->is_ready = g_current_frame.is_ready;
    frame->data_type = g_current_frame.data_type;

    return CAM_STATUS_OK;
}

/**
 * @brief Get 16-bit frame data (for NIR mode)
 */
Camera_StatusTypeDef Camera_GetFrame16b(Camera_FrameTypeDef *frame, uint16_t *buffer_16b)
{
    if (frame == NULL || buffer_16b == NULL) {
        return CAM_STATUS_ERROR;
    }

    if (!g_current_frame.is_ready) {
        return CAM_STATUS_TIMEOUT;
    }

    /* Copy frame data */
    uint32_t pixel_count = (uint32_t)g_current_frame.width * (uint32_t)g_current_frame.height;
    for (uint32_t i = 0U; i < pixel_count; i++) {
        buffer_16b[i] = g_frame_buffer_16b[i];
    }

    frame->frame_buffer = buffer_16b;
    frame->width = g_current_frame.width;
    frame->height = g_current_frame.height;
    frame->timestamp = g_current_frame.timestamp;
    frame->is_ready = g_current_frame.is_ready;
    frame->data_type = 1U;

    return CAM_STATUS_OK;
}

/**
 * @brief Register callback for frame ready events
 */
void Camera_RegisterCallback(Camera_CallbackTypeDef callback)
{
    g_frame_callback = callback;
}

/**
 * @brief Check if camera is currently capturing
 */
uint8_t Camera_IsCapturing(void)
{
    return g_capture_in_progress;
}

/* ============================================================================
 * P1: SYNCHRONIZATION/TRIGGER CONTROL FUNCTIONS
 * ============================================================================*/

/**
 * @brief Configure external trigger mode
 */
void Camera_SetTriggerMode(Camera_TriggerModeTypeDef mode,
                           Optical_TriggerPolarityTypeDef polarity)
{
    if (mode < CAM_TRIG_MODE_COUNT) {
        g_trigger_mode = mode;
    }
    g_trigger_polarity = polarity;
}

/**
 * @brief Get current trigger configuration
 */
void Camera_GetTriggerConfig(Camera_TriggerModeTypeDef *mode,
                             Optical_TriggerPolarityTypeDef *polarity)
{
    if (mode != NULL) {
        *mode = g_trigger_mode;
    }
    if (polarity != NULL) {
        *polarity = g_trigger_polarity;
    }
}

/**
 * @brief Enable or disable hardware synchronization
 */
void Camera_SetSyncEnable(uint8_t enable)
{
    g_sync_enabled = enable ? 1U : 0U;

    if (enable) {
        __HAL_GPIO_EXTI_CLEAR_IT(CAM_EXT_TRIGGER_PIN);
        HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
    } else {
        HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
    }
}

/**
 * @brief Get synchronization enable state
 */
uint8_t Camera_GetSyncEnable(void)
{
    return g_sync_enabled;
}

/**
 * @brief Set trigger delay
 */
void Camera_SetTriggerDelay(uint16_t delay_us)
{
    if (delay_us < CAM_TRIGGER_DELAY_MIN_US) {
        delay_us = CAM_TRIGGER_DELAY_MIN_US;
    } else if (delay_us > CAM_TRIGGER_DELAY_MAX_US) {
        delay_us = CAM_TRIGGER_DELAY_MAX_US;
    }
    g_trigger_delay_us = delay_us;
}

/**
 * @brief Set exposure width for sync mode
 */
void Camera_SetExposureWidth(uint16_t width_us)
{
    if (width_us < CAM_TRIGGER_WIDTH_MIN_US) {
        width_us = CAM_TRIGGER_WIDTH_MIN_US;
    } else if (width_us > CAM_TRIGGER_WIDTH_MAX_US) {
        width_us = CAM_TRIGGER_WIDTH_MAX_US;
    }
    g_exposure_width_us = width_us;
}

/**
 * @brief Register callback for trigger events
 */
void Camera_RegisterTriggerCallback(Camera_TriggerCallbackTypeDef callback)
{
    g_trigger_callback = callback;
}

/**
 * @brief Get camera synchronization status
 */
void Camera_GetSyncStatus(Camera_SyncStatusTypeDef *status)
{
    if (status != NULL) {
        status->sync_enabled = g_sync_enabled;
        status->mode = g_trigger_mode;
        status->polarity = g_trigger_polarity;
        status->trigger_delay_us = g_trigger_delay_us;
        status->exposure_width_us = g_exposure_width_us;
        status->last_trigger_timestamp = g_last_trigger_timestamp;
        status->trigger_count = g_trigger_count;
    }
}

/**
 * @brief Reset trigger counter
 */
void Camera_ResetTriggerCount(void)
{
    g_trigger_count = 0U;
}

/**
 * @brief Wait for external trigger (blocking)
 */
int8_t Camera_WaitForTrigger(uint32_t timeout_ms)
{
    uint32_t start_tick = HAL_GetTick();
    uint32_t timeout_ticks = timeout_ms;

    while (!g_trigger_event_flag) {
        if ((HAL_GetTick() - start_tick) >= timeout_ticks) {
            return -1;  /* Timeout */
        }
    }

    g_trigger_event_flag = 0U;
    return 0;
}

/**
 * @brief Handle external trigger interrupt
 * @note Called from EXTI interrupt handler
 */
void Camera_HandleExtTrigger(void)
{
    g_trigger_event_flag = 1U;
    g_trigger_count++;
    g_last_trigger_timestamp = HAL_GetTick();

    if (g_trigger_callback != NULL) {
        g_trigger_callback();
    }
}

/* ============================================================================
 * DCMI FRAME COMPLETE CALLBACK
 * ============================================================================*/

/**
 * @brief DCMI frame complete callback
 * @note Called by HAL when frame capture is complete
 */
void HAL_DCMI_FrameEventCallback(DCMI_HandleTypeDef *hdcmi)
{
    (void)hdcmi;

    g_current_frame.is_ready = 1U;
    g_current_frame.timestamp = HAL_GetTick();
    g_capture_in_progress = 0U;

    /* Call user callback if registered */
    if (g_frame_callback != NULL) {
        g_frame_callback(&g_current_frame);
    }
}

/* ============================================================================
 * DEBUG FUNCTIONS
 * ============================================================================*/

/**
 * @brief Debug status string
 */
void Camera_DebugStatus(char *status_str, uint32_t buf_len)
{
    if (status_str == NULL || buf_len == 0U) {
        return;
    }

    (void)snprintf(status_str, (size_t)buf_len,
             "Cam[S:%s,C:%s,E:%d,NIR:%d,Sync:%s,Trig:%d,TC:%lu]",
             (g_cam_status == CAM_STATUS_OK) ? "OK" : "ERR",
             g_capture_in_progress ? "Act" : "Idl",
             g_exposure_level,
             g_nir_mode,
             g_sync_enabled ? "ON" : "OFF",
             g_trigger_mode,
             (unsigned long)g_trigger_count);
}