#pragma once

#include <stdbool.h>
#include "esp_err.h"
#include "esp_camera.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ENABLE_CAMERA
#define ENABLE_CAMERA 1
#endif

typedef struct {
    int pin_pwdn;
    int pin_reset;
    int pin_xclk;

    int pin_sccb_sda;
    int pin_sccb_scl;

    int pin_pclk;
    int pin_vsync;
    int pin_href;

    int pin_d0;
    int pin_d1;
    int pin_d2;
    int pin_d3;
    int pin_d4;
    int pin_d5;
    int pin_d6;
    int pin_d7;

    int xclk_freq_hz;
    framesize_t frame_size;
    pixformat_t pixel_format;
    int jpeg_quality;
    int fb_count;
} ov5640_config_t;

esp_err_t ov5640_init(const ov5640_config_t *cfg);
camera_fb_t *ov5640_capture(void);
void ov5640_return_frame(camera_fb_t *fb);
void ov5640_deinit(void);
bool ov5640_is_ready(void);

#ifdef __cplusplus
}
#endif