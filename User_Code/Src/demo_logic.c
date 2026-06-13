/**
 * @file demo_logic.c
 * @brief Application Logic Implementation - AirMark Range Finder main processing
 * @author Development Team
 *
 * Main processing pipeline:
 * 1. Camera capture (trigger + DMA transfer)
 * 2. Optical configuration (850nm wavelength + LED ON)
 * 3. Image processing (binarization -> connected components -> validation)
 * 4. Subpixel localization (gray moment method)
 * 5. Triangulation (depth calculation)
 * 6. LED OFF
 */
#include "demo_logic.h"
#include "led_driver.h"
#include "camera_driver.h"
#include "optical_config.h"
#include "mark_detection.h"
#include "subpixel_center.h"
#include "triangulation.h"

/* Private variables ---------------------------------------------------------*/
static App_LightingStateTypeDef g_lighting_state = {
    .lower_active = 0U,
    .middle_active = 0U,
    .upper_active = 0U,
    .current_zone = ZONE_UNKNOWN
};

static App_CollisionStateTypeDef g_collision_state = {
    .is_collision = 0U,
    .is_too_close = 0U,
    .is_x_out_of_range = 0U,
    .is_y_out_of_range = 0U,
    .alert_level = ALERT_LEVEL_NONE
};

/* Frame buffers */
static uint8_t g_grayscale_frame[FRAME_WIDTH * FRAME_HEIGHT];
static uint8_t g_binary_frame[(FRAME_WIDTH * FRAME_HEIGHT + 3U) / 4U];

/* Detection results */
static MarkDetect_ResultTypeDef g_detection_result;
static Subpixel_ResultTypeDef g_subpixel_result;
static Triangulation_ResultTypeDef g_triangulation_result;

/* Processing state */
static uint8_t g_initialized = 0U;

/* Private function prototypes -----------------------------------------------*/
static void App_CaptureAndProcess(void);
static void App_ProcessDepthData(float depth);

/**
 * @brief Initialize application logic module
 */
void App_Init(void)
{
    /* Reset lighting state */
    g_lighting_state.lower_active = 0U;
    g_lighting_state.middle_active = 0U;
    g_lighting_state.upper_active = 0U;
    g_lighting_state.current_zone = ZONE_UNKNOWN;

    /* Reset collision state */
    g_collision_state.is_collision = 0U;
    g_collision_state.is_too_close = 0U;
    g_collision_state.is_x_out_of_range = 0U;
    g_collision_state.is_y_out_of_range = 0U;
    g_collision_state.alert_level = ALERT_LEVEL_NONE;

    /* Initialize modules */
    ImgProc_Init();
    Triangulation_Init();
    Triangulation_SetMarkerSize(MARKER_SIDE_LEN);

    Optical_Init();
    (void)Optical_SetWavelength(WAVELENGTH_850NM);

    /* Set default LED config */
    LED_Init();
    LED_SetMode(LED_MODE_OFF);
    LED_SetEnableWord(LED_MASK_NONE);

    g_initialized = 1U;
}

/**
 * @brief Main system loop handler
 * @note Executes the complete pipeline once per cycle
 */
void Sys_MainLoop(void)
{
    if (!g_initialized) {
        App_Init();
    }

    /* Execute the complete capture and processing pipeline */
    App_CaptureAndProcess();
}

/**
 * @brief Capture one frame and execute full processing pipeline
 */
static void App_CaptureAndProcess(void)
{
    Camera_StatusTypeDef cam_status;
    Camera_FrameTypeDef frame;

    /* Step 1: Start camera capture */
    cam_status = Camera_StartCapture();
    if (cam_status != CAM_STATUS_OK) {
        /* If busy, skip this cycle */
        return;
    }

    /* Step 2: Wait for frame capture completion (polling mode) */
    uint32_t timeout = 100U;  /* 100ms timeout */
    uint32_t start_tick = HAL_GetTick();
    while (!Camera_IsCapturing() && (HAL_GetTick() - start_tick) < timeout) {
        /* Wait for capture to start */
    }

    /* Wait for frame ready (in real system would use DMA interrupt) */
    start_tick = HAL_GetTick();
    while ((HAL_GetTick() - start_tick) < 50U) {
        cam_status = Camera_GetFrame(&frame);
        if (cam_status == CAM_STATUS_OK) {
            break;
        }
    }

    /* Step 3: Configure optical system - 850nm + LED ON */
    (void)Optical_SetWavelength(WAVELENGTH_850NM);
    LED_SetMode(LED_MODE_CONSTANT);
    LED_SetEnableWord(LED_MASK_ALL);

    /* Step 4: Binarize image with default threshold 25 */
    (void)ImgProc_Binarize(g_grayscale_frame, g_binary_frame,
                           FRAME_WIDTH, FRAME_HEIGHT,
                           BIN_THRESHOLD_DEFAULT);

    /* Step 5: Find connected components (8-connectivity, Union-Find) */
    cam_status = ImgProc_FindConnectedComponents(g_binary_frame,
                                                 FRAME_WIDTH, FRAME_HEIGHT,
                                                 &g_detection_result);

    /* Step 6: Validate detection results */
    if (!ImgProc_ValidateResult(&g_detection_result)) {
        /* Invalid detection - turn off LED and return */
        LED_SetMode(LED_MODE_OFF);
        LED_SetEnableWord(LED_MASK_NONE);
        return;
    }

    /* Step 7: Calculate subpixel centers using gray moment method */
    (void)Subpixel_ProcessAll(g_grayscale_frame, &g_detection_result, &g_subpixel_result);

    /* Step 8: Calculate depth via triangulation */
    Triangulation_StatusTypeDef tri_status;
    tri_status = Triangulation_Calc3DPosition(&g_subpixel_result, &g_triangulation_result);

    if (tri_status == TRI_STATUS_OK && g_triangulation_result.is_valid) {
        /* Process depth for each valid marker */
        for (uint8_t i = 0U; i < g_triangulation_result.valid_count; i++) {
            float depth = g_triangulation_result.distances[i];
            if (depth > 0.0f) {
                App_ProcessDepthData(depth);
            }
        }
    }

    /* Step 9: Turn off LED after processing */
    LED_SetMode(LED_MODE_OFF);
    LED_SetEnableWord(LED_MASK_NONE);
}

/**
 * @brief Process depth data and update zone state
 * @param depth_data Latest depth measurement in meters
 */
void App_ProcessDepth(float depth_data)
{
    /* Determine current zone */
    App_ZoneTypeDef zone = App_DetermineZone(depth_data);
    g_lighting_state.current_zone = zone;

    /* Update lighting based on zone */
    switch (zone) {
        case ZONE_LOWER:
            /* 0.5m - 1.2m: Lower zone */
            g_lighting_state.lower_active = 1U;
            g_lighting_state.middle_active = 0U;
            g_lighting_state.upper_active = 0U;
            break;

        case ZONE_MIDDLE:
            /* 1.2m - 1.8m: Middle zone */
            g_lighting_state.lower_active = 0U;
            g_lighting_state.middle_active = 1U;
            g_lighting_state.upper_active = 0U;
            break;

        case ZONE_UPPER:
            /* 1.8m - 2.2m: Upper zone */
            g_lighting_state.lower_active = 0U;
            g_lighting_state.middle_active = 0U;
            g_lighting_state.upper_active = 1U;
            break;

        case ZONE_UNKNOWN:
        default:
            g_lighting_state.lower_active = 0U;
            g_lighting_state.middle_active = 0U;
            g_lighting_state.upper_active = 0U;
            break;
    }

    /* Check for collision conditions */
    for (uint8_t i = 0U; i < g_triangulation_result.valid_count; i++) {
        (void)App_CheckCollision(&g_triangulation_result.positions[i]);
    }
}

/**
 * @brief Get current lighting state
 */
void App_GetLightingState(App_LightingStateTypeDef *state)
{
    if (state != NULL) {
        state->lower_active = g_lighting_state.lower_active;
        state->middle_active = g_lighting_state.middle_active;
        state->upper_active = g_lighting_state.upper_active;
        state->current_zone = g_lighting_state.current_zone;
    }
}

/**
 * @brief Get current collision state
 */
void App_GetCollisionState(App_CollisionStateTypeDef *state)
{
    if (state != NULL) {
        state->is_collision = g_collision_state.is_collision;
        state->is_too_close = g_collision_state.is_too_close;
        state->is_x_out_of_range = g_collision_state.is_x_out_of_range;
        state->is_y_out_of_range = g_collision_state.is_y_out_of_range;
        state->alert_level = g_collision_state.alert_level;
    }
}

/**
 * @brief Check for collision conditions
 * @param pos 3D position to check
 * @retval App_CollisionStateTypeDef collision state
 */
App_CollisionStateTypeDef App_CheckCollision(const Triangulation_Point3DTypeDef *pos)
{
    App_CollisionStateTypeDef collision = {
        .is_collision = 0U,
        .is_too_close = 0U,
        .is_x_out_of_range = 0U,
        .is_y_out_of_range = 0U,
        .alert_level = ALERT_LEVEL_NONE
    };

    if (pos == NULL) {
        return collision;
    }

    /* Check Z (depth) threshold - collision if < 0.5m */
    if (pos->z < WORKSPACE_Z_MIN) {
        collision.is_too_close = 1U;
        collision.is_collision = 1U;
        collision.alert_level = ALERT_LEVEL_HIGH;
    }

    /* Check X boundary */
    if (pos->x < WORKSPACE_X_MIN || pos->x > WORKSPACE_X_MAX) {
        collision.is_x_out_of_range = 1U;
        collision.is_collision = 1U;
        if (collision.alert_level < ALERT_LEVEL_HIGH) {
            collision.alert_level = ALERT_LEVEL_HIGH;
        }
    }

    /* Check Y boundary */
    if (pos->y < WORKSPACE_Y_MIN || pos->y > WORKSPACE_Y_MAX) {
        collision.is_y_out_of_range = 1U;
        collision.is_collision = 1U;
        if (collision.alert_level < ALERT_LEVEL_HIGH) {
            collision.alert_level = ALERT_LEVEL_HIGH;
        }
    }

    /* Update global collision state */
    g_collision_state = collision;

    return collision;
}

/**
 * @brief Determine zone from depth
 * @param depth Depth in meters
 * @retval Zone classification
 */
App_ZoneTypeDef App_DetermineZone(float depth)
{
    if (depth < ZONE_LOWER_MIN) {
        return ZONE_UNKNOWN;
    }
    if (depth < ZONE_LOWER_MAX) {
        return ZONE_LOWER;
    }
    if (depth < ZONE_MID_MAX) {
        return ZONE_MIDDLE;
    }
    if (depth <= ZONE_UPPER_MAX) {
        return ZONE_UPPER;
    }
    return ZONE_UNKNOWN;
}