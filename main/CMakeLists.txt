idf_component_register(SRCS "main.c" "wifi_mgr.c" "cam.c" "upload.c"
                    INCLUDE_DIRS "."
					REQUIRES BSP
					REQUIRES esp_wifi nvs_flash)
