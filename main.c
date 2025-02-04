#include <stdio.h>
#include "pico/stdlib.h"

// GPIO do LED RGB
#define RGB_GREEN 11
#define RGB_BLUE 12
#define RGB_RED 13

// GPIO do Botão A
#define BUTTON_A 5

// Definindo a máscara para ativar a GPIO
#define OUTPUT_MASK ((1 << RGB_BLUE) | (1 << RGB_GREEN) | (1 << RGB_RED))

uint16_t atraso = 3000;        // Atraso em ms
int contador = 1;              // Contador para definir qual led vai ser ativado
absolute_time_t turn_off_time; // Variável para armazenar o tempo de desligamento dos LEDS
// Variável que armazena o tempo do último evento, em microssegundos
static volatile uint32_t last_time = 0; 

// Função de callback para desligar os LEDS após o tempo programado
int64_t turn_off_callback(alarm_id_t id, void *user_data){
    

    return 0;
}

int main(){
    stdio_init_all();

    // Inicializando a máscara da GPIO utilizada
    gpio_init_mask(OUTPUT_MASK);
    // Definindo como saída
    gpio_set_dir_out_masked(OUTPUT_MASK);

    // Inicializando a GPIO do botão A da BitDogLab
    gpio_init(BUTTON_A);
    // Definindo como entrada
    gpio_set_dir(BUTTON_A, GPIO_IN);
    // Configurando como Pull Up
    gpio_pull_up(BUTTON_A);

    while (true) {
        // Obtendo o tempo atual (em microssegundos)
        uint32_t current_time = to_us_since_boot(get_absolute_time());

        // Verificando se o botão foi pressionado
        if (gpio_get(BUTTON_A) == 0){
            // Verificação do efetio de debounce
            if(current_time - last_time > 200000){
                // Atualizando o last_time para a próxima verificação do debounce
                last_time = current_time;

                // Ligando todos os LEDs na máscara definida anteriormente
                gpio_put_masked(OUTPUT_MASK, OUTPUT_MASK);

                // Agenda um alarme para desligar os LEDs após 3 segundos
                add_alarm_in_ms(atraso, turn_off_callback, NULL, false);
            }

            //Pequeno atraso para reduzir o uso da CPU
            sleep_ms(10);
        }
    }
}
