/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "freertos/semphr.h"
#include "esp_log.h"

float temperature;
 
SemaphoreHandle_t mutexI2C;


float acessa_i2c(int comando){
    if(comando == 1){
        ESP_LOGI("I2C", "Leitura do sensor de Temp..");
        return 20.0 + ((float) rand() / (float)(RAND_MAX/10));
    }else{
        ESP_LOGI("I2C", "Escrita no LCD");
        printf("Tela LCD: TEMP: %f", temperature);
    }

    return 0.0;
}

void le_sensor(void * params){
    while(1){
        if(xSemaphoreTake(mutexI2C, 1000 / portTICK_PERIOD_MS)){
            temperature = acessa_i2c(1);
            ESP_LOGI("Leitura", "Temperatura: %f", temperature);
            xSemaphoreGive(mutexI2C);
        }else{
            ESP_LOGE("Leitura", "Não foi possivel ler temp");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);

    }
}

void lcd_display(void * params){
    while(1){
        if(xSemaphoreTake(mutexI2C, 1000 / portTICK_PERIOD_MS)){
            ESP_LOGI("Diplay", "ESCREVE NO LCD");
            acessa_i2c(2);
            xSemaphoreGive(mutexI2C);
        }else{
            ESP_LOGE("Leitura", "Não foi possivel escrever no display");
        }
        vTaskDelay(2000 / portTICK_PERIOD_MS);

    }
}

void app_main(void)
{
    mutexI2C = xSemaphoreCreateMutex();

    xTaskCreate(&le_sensor, "leitura sensor 1", 2048, NULL, 2, NULL);
    xTaskCreate(&lcd_display, "leitura sensor 1", 2048, NULL, 2, NULL);
   
}
