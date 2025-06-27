idf_component_register(SRCS "wifi.c" "hello_world_main.c" "http_client.c"
                    PRIV_REQUIRES spi_flash esp_wifi nvs_flash esp_http_client
                    INCLUDE_DIRS "")
