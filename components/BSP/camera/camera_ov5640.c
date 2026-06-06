#include "camera_ov5640.h"

#include <string.h>
#include "esp_log.h"

static const char *TAG = "OV5640";

static bool s_inited = false;
static camera_config_t s_cam_cfg;

#if ENABLE_CAMERA

static bool check_cfg_valid(const ov5640_config_t *cfg)
{
    if (cfg == NULL) {
        return false;
    }

    if (cfg->pin_xclk < 0 ||
        cfg->pin_sccb_sda < 0 ||
        cfg->pin_sccb_scl < 0 ||
        cfg->pin_pclk < 0 ||
        cfg->pin_vsync < 0 ||
        cfg->pin_href < 0) {
        return false;
    }

    if (cfg->pin_d0 < 0 || cfg->pin_d1 < 0 || cfg->pin_d2 < 0 || cfg->pin_d3 < 0 ||
        cfg->pin_d4 < 0 || cfg->pin_d5 < 0 || cfg->pin_d6 < 0 || cfg->pin_d7 < 0) {
        return false;
    }

    if (cfg->xclk_freq_hz <= 0 || cfg->fb_count <= 0) {
        return false;
    }

    return true;
}

#endif

esp_err_t ov5640_init(const ov5640_config_t *cfg)
{
#if ENABLE_CAMERA
    if (s_inited) {
        return ESP_OK;
    }

    if (!check_cfg_valid(cfg)) {
        ESP_LOGE(TAG, "invalid camera config");
        return ESP_ERR_INVALID_ARG;
    }

    memset(&s_cam_cfg, 0, sizeof(s_cam_cfg));

    s_cam_cfg.pin_pwdn  = cfg->pin_pwdn;
    s_cam_cfg.pin_reset = cfg->pin_reset;
    s_cam_cfg.pin_xclk  = cfg->pin_xclk;

    s_cam_cfg.pin_sccb_sda = cfg->pin_sccb_sda;
    s_cam_cfg.pin_sccb_scl = cfg->pin_sccb_scl;

    s_cam_cfg.pin_pclk  = cfg->pin_pclk;
    s_cam_cfg.pin_vsync = cfg->pin_vsync;
    s_cam_cfg.pin_href  = cfg->pin_href;

    s_cam_cfg.pin_d0 = cfg->pin_d0;
    s_cam_cfg.pin_d1 = cfg->pin_d1;
    s_cam_cfg.pin_d2 = cfg->pin_d2;
    s_cam_cfg.pin_d3 = cfg->pin_d3;
    s_cam_cfg.pin_d4 = cfg->pin_d4;
    s_cam_cfg.pin_d5 = cfg->pin_d5;
    s_cam_cfg.pin_d6 = cfg->pin_d6;
    s_cam_cfg.pin_d7 = cfg->pin_d7;

    s_cam_cfg.ledc_timer   = LEDC_TIMER_0;
    s_cam_cfg.ledc_channel = LEDC_CHANNEL_0;

    s_cam_cfg.xclk_freq_hz = cfg->xclk_freq_hz;
    s_cam_cfg.pixel_format = cfg->pixel_format;
    s_cam_cfg.frame_size   = cfg->frame_size;
    s_cam_cfg.jpeg_quality = cfg->jpeg_quality;
    s_cam_cfg.fb_count     = cfg->fb_count;
    s_cam_cfg.grab_mode    = CAMERA_GRAB_LATEST;
    s_cam_cfg.fb_location   = CAMERA_FB_IN_PSRAM;

    esp_err_t err = esp_camera_init(&s_cam_cfg);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "esp_camera_init failed: %s", esp_err_to_name(err));
        return err;
    }

    s_inited = true;
    ESP_LOGI(TAG, "camera init success");
    return ESP_OK;
#else
    (void)cfg;
    s_inited = false;
    ESP_LOGW(TAG, "camera disabled by ENABLE_CAMERA=0");
    return ESP_OK;
#endif
}

camera_fb_t *ov5640_capture(void)
{
#if ENABLE_CAMERA
    if (!s_inited) {
        ESP_LOGE(TAG, "camera not initialized");
        return NULL;
    }

    camera_fb_t *fb = esp_camera_fb_get();
    if (fb == NULL) {
        ESP_LOGE(TAG, "esp_camera_fb_get failed");
        return NULL;
    }

    return fb;
#else
    return NULL;
#endif
}

void ov5640_return_frame(camera_fb_t *fb)
{
#if ENABLE_CAMERA
    if (fb != NULL) {
        esp_camera_fb_return(fb);
    }
#else
    (void)fb;
#endif
}

void ov5640_deinit(void)
{
#if ENABLE_CAMERA
    if (!s_inited) {
        return;
    }

    esp_camera_deinit();
    s_inited = false;
    ESP_LOGI(TAG, "camera deinit");
#else
    s_inited = false;
#endif
}

bool ov5640_is_ready(void)
{
#if ENABLE_CAMERA
    return s_inited;
#else
    return false;
#endif
}