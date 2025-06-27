#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "driver/gpio.h"

#define LED_GPIO 25
#define BOTAO_GPIO 0 // botao da boot da placa

QueueHandle_t filaDeInterrupcao;

int led_state = 1;

static void IRAM_ATTR gpio_isr_handler(void *args)
{
  int pino = (int)args;
  xQueueSendFromISR(filaDeInterrupcao, &pino, NULL);
}

void trataInterrupcaoBotao(void *params)
{
  int pino;
  int contador = 0;

  while(true)
  {
    if(xQueueReceive(filaDeInterrupcao, &pino, portMAX_DELAY))
    {
      // De-bouncing
      int estado = gpio_get_level(pino);
      if(estado == 1)
      {
        gpio_isr_handler_remove(pino);
        while(gpio_get_level(pino) == estado)
        {
          vTaskDelay(50 / portTICK_PERIOD_MS);
        }

        contador++;
        printf("Os botões foram acionados %d vezes. Botão: %d\n", contador, pino);

        led_state = !led_state;
        gpio_set_level(LED_GPIO, led_state);
        



        // Habilitar novamente a interrupção
        vTaskDelay(50 / portTICK_PERIOD_MS);
        gpio_isr_handler_add(pino, gpio_isr_handler, (void *) pino);
      }

    }
  }
}

void app_main()
{
  // Configuração dos pinos dos LEDs 
  esp_rom_gpio_pad_select_gpio(LED_GPIO);   
  // Configura os pinos dos LEDs como Output
  gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
  
  gpio_set_level(LED_GPIO, led_state);

  // Configuração do pino do Botão
  esp_rom_gpio_pad_select_gpio(BOTAO_GPIO);
  // Configura o pino do Botão como Entrada
  gpio_set_direction(BOTAO_GPIO, GPIO_MODE_INPUT);
  // Configura o resistor de Pulldown para o botão (por padrão a entrada estará em Zero)
  gpio_pulldown_en(BOTAO_GPIO);
  // Desabilita o resistor de Pull-up por segurança.
  gpio_pullup_dis(BOTAO_GPIO);

  // Configura pino para interrupção
  gpio_set_intr_type(BOTAO_GPIO, GPIO_INTR_POSEDGE);

  filaDeInterrupcao = xQueueCreate(10, sizeof(int));
  xTaskCreate(trataInterrupcaoBotao, "TrataBotao", 2048, NULL, 1, NULL);

  gpio_install_isr_service(0);
  gpio_isr_handler_add(BOTAO_GPIO, gpio_isr_handler, (void *) BOTAO_GPIO);

}
