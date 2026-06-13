/**
 * @file main.c
 * @brief Main application entry point for AirMark Range Finder
 * @author Development Team
 *
 * System: STM32F407, 168MHz HSE/PLL
 * Modules: LED (5x 850nm), Camera (OV2640 320x240 8bit grayscale),
 *          Optical (NIR narrowband), Triangulation
 */
#include "main.h"
#include "led_driver.h"
#include "camera_driver.h"
#include "optical_config.h"
#include "mark_detection.h"
#include "subpixel_center.h"
#include "triangulation.h"
#include "demo_logic.h"
#include "calibration.h"

/* Private variables ---------------------------------------------------------*/
static System_StatusTypeDef g_sys_status = SYS_STATUS_NOT_INIT;
static System_ErrorTypeDef g_last_error = SYS_ERROR_NONE;
static uint32_t g_error_log[16];
static uint8_t g_error_count = 0U;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Init_All_Peripherals(void);
static void Error_Handler_Custom(System_ErrorTypeDef error);

/**
 * @brief Main entry point
 */
int main(void)
{
    /* Reset of all peripherals, initialize the Flash interface and Systick */
    HAL_Init();

    /* Configure the system clock to 168MHz using HSE/PLL */
    SystemClock_Config();

    /* Initialize all peripherals */
    Init_All_Peripherals();

    /* Update system status */
    g_sys_status = SYS_STATUS_OK;

    /* Main processing loop */
    while (1) {
        /* Execute main loop processing - capture, process, triangulate */
        Sys_MainLoop();

        /* Small delay to prevent bus congestion */
        HAL_Delay(1);
    }
}

/**
 * @brief System clock configuration - 168MHz using HSE/PLL
 * @note Clock tree:
 *       HSE (8MHz) -> PLLM (8) -> PLLQ (7) -> PLLP (2) -> SYSCLK (168MHz)
 *       APB1 = 168MHz / 4 = 42MHz
 *       APB2 = 168MHz / 2 = 84MHz
 */
static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Enable HSE oscillator and wait for it to be ready */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;

    /* PLL configuration: 8MHz / 8 * 168 / 2 = 168MHz */
    RCC_OscInitStruct.PLL.PLLM = 8U;     /* PLLM = 8 */
    RCC_OscInitStruct.PLL.PLLN = 336U;   /* PLLN = 336 */
    RCC_OscInitStruct.PLL.PLLP = 2U;     /* PLLP = 2 */
    RCC_OscInitStruct.PLL.PLLQ = 7U;     /* PLLQ = 7 (for USB, 48MHz) */

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler_Custom(SYS_ERROR_UNKNOWN);
    }

    /* Select PLL as system clock source */
    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK |
                                    RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;   /* HCLK = 168MHz */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;    /* APB1 = 42MHz */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;    /* APB2 = 84MHz */

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
        Error_Handler_Custom(SYS_ERROR_UNKNOWN);
    }

    /* Configure SysTick for 1ms interrupts */
    if (HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000U) != HAL_OK) {
        Error_Handler_Custom(SYS_ERROR_UNKNOWN);
    }

    /* Set interrupt priority grouping to 4 bits for preemption */
    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
}

/**
 * @brief Initialize all project-specific peripherals and modules
 */
static void Init_All_Peripherals(void)
{
    /* Initialize LED driver (5x 850nm IR LEDs in pentagon arrangement) */
    if (LED_Init() != LED_STATUS_OK) {
        g_last_error = SYS_ERROR_LED_INIT;
        Error_Handler();
    }

    /* Initialize camera driver (OV2640 8bit grayscale 320x240) */
    if (Camera_Init() != CAM_STATUS_OK) {
        g_last_error = SYS_ERROR_CAMERA_INIT;
        Error_Handler();
    }

    /* Configure camera trigger mode for sync with LED */
    Camera_SetTriggerMode(CAM_TRIG_MODE_SOFTWARE, TRIG_POLARITY_ACTIVE_HIGH);
    Camera_SetSyncEnable(0U);  /* Disabled by default */

    /* Initialize optical configuration (850nm narrowband FWHM=18nm, 20kHz PWM) */
    Optical_Init();
    (void)Optical_SetWavelength(WAVELENGTH_850NM);
    (void)Optical_SetModulationParams(MODULATION_DEFAULT_FREQ_HZ,
                                      MODULATION_DEFAULT_DUTY_PCT,
                                      MODULATION_DEFAULT_PULSE_US);
    (void)Optical_SetFilterType(FILTER_NARROWBAND_850NM);
    Optical_SetSyncEnable(0U);

    /* Initialize image processing (binarization, connected components) */
    ImgProc_Init();

    /* Initialize triangulation module */
    Triangulation_Init();
    Triangulation_SetMarkerSize(MARKER_SIDE_LEN);

    /* Initialize calibration module (load from flash or use defaults) */
    if (Calib_Init() != CALIB_STATUS_OK) {
        /* Not calibrated - will use defaults, continue anyway */
        g_last_error = SYS_ERROR_CALIB_INIT;
    }

    /* Initialize application logic */
    App_Init();
}

/**
 * @brief Custom error handler with specific error tracking
 */
static void Error_Handler_Custom(System_ErrorTypeDef error)
{
    g_last_error = error;
    if (g_error_count < 16U) {
        g_error_log[g_error_count++] = error;
    }
    Error_Handler();
}

/**
 * @brief  This function is executed in case of error occurrence
 */
void Error_Handler(void)
{
    /* Log error */
    if (g_error_count < 16U) {
        g_error_log[g_error_count++] = g_last_error;
    }

    /* Turn on red alert LED in breathing mode */
    LED_SetMode(LED_MODE_ALERT);
    LED_SetEnableWord(LED_MASK_ALL);

    /* Infinite loop - can be replaced with system reset in production */
    while (1) {
        HAL_Delay(500);
    }
}