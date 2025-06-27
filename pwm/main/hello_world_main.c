
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/ledc.h"

#define LED_GPIO 25
#define BOTAO_GPIO 32

void app_main(void)
{
    //configura pinos
    esp_rom_gpio_pad_select_gpio(LED_GPIO);
    esp_rom_gpio_pad_select_gpio(BOTAO_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(BOTAO_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BOTAO_GPIO, GPIO_PULLUP_ONLY);

    //pwm
    ledc_timer_config_t timer_config ={
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_8_BIT, // 0 a 255
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK,
    };

    ledc_timer_config(&timer_config);
    //config
    ledc_channel_config_t channel_config = {
        .gpio_num = LED_GPIO,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 0,
        .hpoint = 0,
    };
    ledc_channel_config(&channel_config);
    
    int estado = 0;
    int pwm_value = 0;

    while(1){
        estado = gpio_get_level(BOTAO_GPIO);

        if(!estado){
            pwm_value++;
            ESP_LOGI("LED", "Acendendo LED");
        }else{
            pwm_value--;
            ESP_LOGE("LED", "Apagando LED");
        }
        if(pwm_value > 255){
            pwm_value = 255;
        }
        if(pwm_value < 0){
            pwm_value = 0;
        }
        ESP_LOGW("PWM", "PWM: %d", pwm_value);
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, pwm_value);
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
