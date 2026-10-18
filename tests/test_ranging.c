/**
 * @file test_ranging.c
 * @brief Host unit test suite for AirMark Range Finder algorithms
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>

#include "main.h"
#include "triangulation.h"
#include "subpixel_center.h"
#include "mark_detection.h"
#include "calibration.h"

#define TEST_PASS 0
#define TEST_FAIL 1

static int g_tests_run = 0;
static int g_tests_passed = 0;

#define ASSERT_NEAR(val, target, eps, name) do { \
    g_tests_run++; \
    float _diff = fabsf((val) - (target)); \
    if (_diff <= (eps)) { \
        printf("  [PASS] %s: got %.4f, expected %.4f (diff: %.4f <= %.4f)\n", name, (val), (target), _diff, (eps)); \
        g_tests_passed++; \
    } else { \
        printf("  [FAIL] %s: got %.4f, expected %.4f (diff: %.4f > %.4f)\n", name, (val), (target), _diff, (eps)); \
    } \
} while(0)

/* Test 1: Pinhole Model Depth Calculation */
void test_pinhole_depth(void)
{
    printf("\n=== Running Test: Pinhole Depth Calculation ===\n");
    Triangulation_Init();
    Triangulation_SetMarkerSize(0.05f); /* 5cm marker baseline */

    /* Z = f * L / y = 320 * 0.05 / y */
    /* Target at 1.00m -> y = 16.0 px */
    float depth_1m = Triangulation_CalcDepth(16.0f);
    ASSERT_NEAR(depth_1m, 1.00f, 0.02f, "Depth at y=16px (1.0m)");

    /* Target at 1.50m -> y = 10.667 px */
    float depth_1_5m = Triangulation_CalcDepth(10.667f);
    ASSERT_NEAR(depth_1_5m, 1.50f, 0.03f, "Depth at y=10.67px (1.5m)");

    /* Target at 2.00m -> y = 8.0 px */
    float depth_2m = Triangulation_CalcDepth(8.0f);
    ASSERT_NEAR(depth_2m, 2.00f, 0.04f, "Depth at y=8px (2.0m)");

    /* Target at 0.50m -> y = 32.0 px */
    float depth_0_5m = Triangulation_CalcDepth(32.0f);
    ASSERT_NEAR(depth_0_5m, 0.50f, 0.01f, "Depth at y=32px (0.5m)");
}

/* Test 2: Subpixel Gray-Moment Centroid Localization */
void test_subpixel_centroid(void)
{
    printf("\n=== Running Test: Subpixel Gray-Moment Centroid ===\n");
    const uint16_t W = 320;
    const uint16_t H = 240;
    uint8_t *img = (uint8_t *)calloc(W * H, sizeof(uint8_t));

    /* Create an IR spot with center at (cx=50.35, cy=60.65) */
    const float true_cx = 50.35f;
    const float true_cy = 60.65f;
    const float sigma = 2.0f;

    for (int y = 55; y <= 66; y++) {
        for (int x = 45; x <= 56; x++) {
            float dx = (float)x - true_cx;
            float dy = (float)y - true_cy;
            float intensity = 250.0f * expf(-(dx * dx + dy * dy) / (2.0f * sigma * sigma));
            if (intensity > 255.0f) intensity = 255.0f;
            img[y * W + x] = (uint8_t)intensity;
        }
    }

    MarkDetect_ComponentTypeDef comp = {
        .label = 1,
        .area = 100,
        .bbox = { .x_min = 45, .y_min = 55, .x_max = 56, .y_max = 66 },
        .centroid_x = 50,
        .centroid_y = 60,
        .is_valid = 1
    };

    Subpixel_PointTypeDef subpix_result;
    ImgProc_SubpixelGrayMoment(img, W, H, &comp, &subpix_result);

    ASSERT_NEAR(subpix_result.x, true_cx, 0.10f, "Subpixel X centroid (<0.1px)");
    ASSERT_NEAR(subpix_result.y, true_cy, 0.10f, "Subpixel Y centroid (<0.1px)");

    free(img);
}

/* Test 3: Camera 3D Position Reconstruction */
void test_3d_reconstruction(void)
{
    printf("\n=== Running Test: 3D Point Reconstruction ===\n");
    Triangulation_Init();

    Subpixel_ResultTypeDef subpix_res;
    memset(&subpix_res, 0, sizeof(subpix_res));
    subpix_res.valid_count = 5;
    subpix_res.is_valid = 1;
    for (int i = 0; i < 5; i++) {
        subpix_res.points[i].x = 160.0f; /* Center */
        subpix_res.points[i].y = 120.0f;
        subpix_res.points[i].marker_image_size = 16.0f; /* 16px -> 1.0m */
        subpix_res.points[i].weight = 1.0f;
    }

    Triangulation_ResultTypeDef tri_res;
    Triangulation_StatusTypeDef status = Triangulation_Calc3DPosition(&subpix_res, &tri_res);

    ASSERT_NEAR((float)status, (float)TRI_STATUS_OK, 0.01f, "Triangulation Status OK");
    ASSERT_NEAR(tri_res.positions[0].z, 1.00f, 0.02f, "3D Z position");
    ASSERT_NEAR(tri_res.positions[0].x, 0.00f, 0.01f, "3D X position (on optical axis)");
    ASSERT_NEAR(tri_res.positions[0].y, 0.00f, 0.01f, "3D Y position (on optical axis)");
}

int main(int argc, char **argv)
{
    (void)argc;
    (void)argv;
    printf("=========================================\n");
    printf("AirMark Range Finder Algorithm Host Tests\n");
    printf("=========================================\n");

    test_pinhole_depth();
    test_subpixel_centroid();
    test_3d_reconstruction();

    printf("\n-----------------------------------------\n");
    printf("Summary: %d / %d tests passed (%.1f%%)\n",
           g_tests_passed, g_tests_run, 100.0f * g_tests_passed / g_tests_run);
    printf("=========================================\n");

    return (g_tests_passed == g_tests_run) ? 0 : 1;
}
