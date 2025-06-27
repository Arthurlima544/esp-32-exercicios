#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "esp32/rom/uart.h"
#include "esp_timer.h"

// Driver da GPIO com funções estendidas para o modo sleep
#include "driver/rtc_io.h"

#define BOTAO 0

RTC_DATA_ATTR int acordou = 0; // na memoria rtc

void app_main()
{
    // Configurando o Sleep Timer (em microsegundos)
    // esp_sleep_enable_timer_wakeup(5 * 1000000);

    rtc_gpio_pullup_en(BOTAO);
    rtc_gpio_pulldown_dis(BOTAO);

    esp_sleep_enable_ext0_wakeup(BOTAO, 0);

    printf("Acordou %d vezes\n",acordou++);
    printf("Entrando em deep sleep\n" );
    esp_deep_sleep_start();
}
