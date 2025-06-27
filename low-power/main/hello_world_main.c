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

void app_main()
{
    // Configuração da GPIO para o botão de entrada
    esp_rom_gpio_pad_select_gpio(BOTAO);
    gpio_set_direction(BOTAO, GPIO_MODE_INPUT);
    // Habilita o botão para acordar a placa
    gpio_wakeup_enable(BOTAO, GPIO_INTR_LOW_LEVEL);
    esp_sleep_enable_gpio_wakeup();

    // Configurando o Sleep Timer (em microsegundos)
    esp_sleep_enable_timer_wakeup(5 * 1000000);

  while(true)
  {

    if (rtc_gpio_get_level(BOTAO) == 0)
    {
        printf("Aguardando soltar o botão ... \n");
        do
        {
            vTaskDelay(pdMS_TO_TICKS(10));
        } while (rtc_gpio_get_level(BOTAO) == 0);
    }

    printf("Entrando em modo Light Sleep\n");
    
    // Configura o modo sleep somente após completar a escrita na UART para finalizar o printf
    uart_tx_wait_idle(CONFIG_ESP_CONSOLE_UART_NUM);

    int64_t tempo_antes_de_dormir = esp_timer_get_time();

    // Entra em modo Light Sleep
    esp_light_sleep_start();

    int64_t tempo_apos_acordar = esp_timer_get_time();

    esp_sleep_wakeup_cause_t causa = esp_sleep_get_wakeup_cause();

    printf("Dormiu por %lld ms\n", (tempo_apos_acordar - tempo_antes_de_dormir) / 1000);
    printf("O [%s] me acordou !\n", causa == ESP_SLEEP_WAKEUP_TIMER ? "TIMER" : "BOTÃO");

  }

}

