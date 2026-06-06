#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "wifi_mgr.h"
#include "cam.h"
#include "upload.h"

static const char *TAG = "MAIN";
static TaskHandle_t s_task;

static void capture_and_upload(void)
{
    camera_fb_t *fb = camera_capture();
    if (!fb) { ESP_LOGE(TAG, "capture fail"); return; }
    http_upload_jpeg(fb->buf, fb->len);
    camera_return_fb(fb);
}

static void task(void *arg)
{
    while (1) {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        capture_and_upload();
    }
}

static void timer_cb(void *arg) { xTaskNotifyGive(s_task); }

void app_main(void)
{
    wifi_mgr_init();

    ESP_ERROR_CHECK(camera_init());

    xTaskCreate(task, "cam_task", 4096, NULL, 5, &s_task);

    esp_timer_handle_t timer;
    const esp_timer_create_args_t ta = {
        .callback = timer_cb, .dispatch_method = ESP_TIMER_TASK,
        .name = "cam_timer", .skip_unhandled_events = true,
    };
    esp_timer_create(&ta, &timer);
    esp_timer_start_periodic(timer, 5000000);

    ESP_LOGI(TAG, "ready, capturing every 5s");
}
