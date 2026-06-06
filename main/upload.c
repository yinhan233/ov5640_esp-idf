#include "upload.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "secrets.h"

static const char *TAG = "UPLOAD";

void http_upload_jpeg(const uint8_t *data, size_t len)
{
    esp_http_client_config_t config = {
        .url = UPLOAD_URL, .method = HTTP_METHOD_POST, .timeout_ms = 10000,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_http_client_set_header(client, "Content-Type", "image/jpeg");
    esp_http_client_set_post_field(client, (const char *)data, len);

    esp_err_t err = esp_http_client_perform(client);
    if (err == ESP_OK) {
        ESP_LOGI(TAG, "OK %d bytes", (int)len);
    } else {
        ESP_LOGE(TAG, "fail");
    }
    esp_http_client_cleanup(client);
}
