#pragma once
#include "esp_err.h"
#include "camera_ov5640.h"

esp_err_t camera_init(void);
camera_fb_t *camera_capture(void);
static inline void camera_return_fb(camera_fb_t *fb) { ov5640_return_frame(fb); }
