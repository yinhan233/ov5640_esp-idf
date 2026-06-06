#include "cam.h"
#include "esp_log.h"
#include "esp_camera.h"

static const char *TAG = "CAM";

#define PIN_SDA   4
#define PIN_SCL   5
#define PIN_XCLK  15
#define PIN_D0    11
#define PIN_D1    9
#define PIN_D2    8
#define PIN_D3    10
#define PIN_D4    12
#define PIN_D5    18
#define PIN_D6    17
#define PIN_D7    16
#define PIN_VSYNC 6
#define PIN_HREF  7
#define PIN_PCLK  13

#define XCLK_MHZ   20
#define JPEG_QUAL  12
#define FB_COUNT   1

esp_err_t camera_init(void)
{
    static ov5640_config_t cfg = {
        .pin_pwdn = -1, .pin_reset = -1, .pin_xclk = PIN_XCLK,
        .pin_sccb_sda = PIN_SDA, .pin_sccb_scl = PIN_SCL,
        .pin_d0 = PIN_D0, .pin_d1 = PIN_D1, .pin_d2 = PIN_D2, .pin_d3 = PIN_D3,
        .pin_d4 = PIN_D4, .pin_d5 = PIN_D5, .pin_d6 = PIN_D6, .pin_d7 = PIN_D7,
        .pin_vsync = PIN_VSYNC, .pin_href = PIN_HREF, .pin_pclk = PIN_PCLK,
        .xclk_freq_hz = XCLK_MHZ * 1000000,
        .pixel_format = PIXFORMAT_JPEG,
        .frame_size = FRAMESIZE_VGA,
        .jpeg_quality = JPEG_QUAL,
        .fb_count = FB_COUNT,
    };

    esp_err_t err = ov5640_init(&cfg);
    if (err == ESP_OK) {
        sensor_t *s = esp_camera_sensor_get();
        if (s && s->id.PID == OV5640_PID) {
            s->set_vflip(s, 1);
        }
        ESP_LOGI(TAG, "init OK");
    }
    return err;
}

camera_fb_t *camera_capture(void)
{
    if (!ov5640_is_ready()) return NULL;
    camera_fb_t *fb = ov5640_capture();
    if (fb) {
        ESP_LOGI(TAG, "captured %u bytes", (unsigned)fb->len);
    }
    return fb;
}
