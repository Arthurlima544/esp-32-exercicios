#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"

void task1(void * params){
    while(1){
        ESP_LOGI("TASK 1:", "Leitura de Sensores");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void task2(void * params){
    while(1){
        ESP_LOGI("TASK 2:", "Escrita de Sensores");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    xTaskCreate(&task1, "leitura", 2048, "Task 1", 1, NULL);
    xTaskCreate(&task2, "Display", 2048, "Task 2", 1, NULL);

}
