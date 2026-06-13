/**
 * @file demo_logic.h
 * @brief Application Module - Layered illumination and anti-collision logic
 * @author Development Team
 */
#ifndef __DEMO_LOGIC_H
#define __DEMO_LOGIC_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "triangulation.h"

/* Exported types ------------------------------------------------------------*/
/**
 * @brief Application status
 */
typedef enum {
    APP_STATUS_OK = 0,
    APP_STATUS_ERROR,
    APP_STATUS_WARNING
} App_StatusTypeDef;

/**
 * @brief Zone classification for layered illumination
 */
typedef enum {
    ZONE_LOWER = 0,   /**< Z: 0.5m - 1.2m */
    ZONE_MIDDLE,      /**< Z: 1.2m - 1.8m */
    ZONE_UPPER,       /**< Z: 1.8m - 2.2m */
    ZONE_UNKNOWN
} App_ZoneTypeDef;

/**
 * @brief Lighting state structure
 */
typedef struct {
    uint8_t lower_active;   /**< Lower zone LEDs active */
    uint8_t middle_active;  /**< Middle zone LEDs active */
    uint8_t upper_active;   /**< Upper zone LEDs active */
    App_ZoneTypeDef current_zone;
} App_LightingStateTypeDef;

/**
 * @brief Collision state structure
 */
typedef struct {
    uint8_t is_collision;       /**< Collision detected flag */
    uint8_t is_too_close;       /**< Z < 0.5m flag */
    uint8_t is_x_out_of_range;  /**< X out of bounds flag */
    uint8_t is_y_out_of_range;  /**< Y out of bounds flag */
    uint8_t alert_level;        /**< Alert level (0-3) */
} App_CollisionStateTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Work space boundaries */
#define WORKSPACE_X_MIN    -1.0f   /**< X minimum in meters */
#define WORKSPACE_X_MAX    1.0f    /**< X maximum in meters */
#define WORKSPACE_Y_MIN    -1.0f   /**< Y minimum in meters */
#define WORKSPACE_Y_MAX    1.0f    /**< Y maximum in meters */
#define WORKSPACE_Z_MIN    0.5f    /**< Z minimum in meters (collision threshold) */
#define WORKSPACE_Z_MAX    2.2f    /**< Z maximum in meters */

/* Alert thresholds */
#define ALERT_LEVEL_NONE       0U
#define ALERT_LEVEL_LOW        1U
#define ALERT_LEVEL_MEDIUM     2U
#define ALERT_LEVEL_HIGH       3U

/* Exported functions --------------------------------------------------------*/
/**
 * @brief Initialize application logic module
 * @retval None
 */
void App_Init(void);

/**
 * @brief Process depth data and update zone state
 * @param depth_data Latest depth measurement in meters
 * @retval None
 */
void App_ProcessDepth(float depth_data);

/**
 * @brief Get current lighting state
 * @param state Pointer to lighting state structure
 * @retval None
 */
void App_GetLightingState(App_LightingStateTypeDef *state);

/**
 * @brief Get current collision state
 * @param state Pointer to collision state structure
 * @retval None
 */
void App_GetCollisionState(App_CollisionStateTypeDef *state);

/**
 * @brief Main system loop handler
 * @note  Called periodically from main() loop
 * @retval None
 */
void Sys_MainLoop(void);

/**
 * @brief Check for collision conditions
 * @param pos 3D position to check
 * @retval App_CollisionStateTypeDef collision state
 */
App_CollisionStateTypeDef App_CheckCollision(const Triangulation_Point3DTypeDef *pos);

/**
 * @brief Determine zone from depth
 * @param depth Depth in meters
 * @retval Zone classification
 */
App_ZoneTypeDef App_DetermineZone(float depth);

#ifdef __cplusplus
}
#endif

#endif /* __DEMO_LOGIC_H */