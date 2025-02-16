/*
* Feito por: Jorge Palma
* Data: 16/02/2025
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/timer.h"
#include "hardware/clocks.h"
#include "hardware/uart.h"
#include "hardware/pwm.h"

#include "include/ssd1306.h"
#include "include/font.h"


// I2C defines
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define ENDERECO 0x3C
#define SCREEN_WIDTH WIDTH
#define SCREEN_HEIGHT HEIGHT
#define LADO_QUADRADO 8

#define CENTRO_X 2131
#define CENTRO_Y 1990
#define ZONA_OFF 45

ssd1306_t ssd;

// LED RGB Defines
#define RED_LED_RGB 13
#define GREEN_LED_RGB 11
#define BLUE_LED_RGB 12

// Definições para os botões
#define BOTAO_A 5
#define BOTAO_JOYSTIC 22
static volatile uint32_t ultimo_tempo = 0;

// PWM
#define WRAP 65535
#define DI_DF 4.0

// Joystic
#define JOY_X 27  // ADC1
#define JOY_Y 26  // ADC0
uint16_t leitura_joy_x; 
uint16_t leitura_joy_y;

// Variáveis de controle
volatile bool leds_azul_vermelho_on = true;
volatile uint8_t borda = 1;
uint16_t posicao_x = SCREEN_WIDTH / 2 - LADO_QUADRADO / 2;  // Iniciando no meio da tela
uint16_t posicao_y = SCREEN_HEIGHT / 2 - LADO_QUADRADO / 2;  // Iniciando no meio da tela


void setup_gpios();
uint16_t ler_adc(uint8_t canal);
void atualizar_posicao_quadrado(uint16_t adc_x, uint16_t adc_y);
int converter_adc_para_tela(uint16_t centro, uint16_t valor_adc, uint16_t tela_max);
void atualizar_led_rgb(uint led_pin, uint16_t valor_adc, uint16_t centro);
static void gpio_irq_handler(uint gpio, uint32_t events);

int main()
{
    stdio_init_all();
    setup_gpios();
    
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled_with_callback(BOTAO_JOYSTIC, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while (true) 
    {
        uint16_t adc_x = ler_adc(1);  // Ler o ADC 1 - X
        uint16_t adc_y = ler_adc(0);

        atualizar_posicao_quadrado(adc_x, adc_y);

        atualizar_led_rgb(RED_LED_RGB, adc_x, CENTRO_X);
        atualizar_led_rgb(BLUE_LED_RGB, adc_y, CENTRO_Y);

        sleep_ms(40);
    }

    return 0;
}

void setup_gpios()
{
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);        
    gpio_pull_up(BOTAO_A);
    gpio_init(BOTAO_JOYSTIC);
    gpio_set_dir(BOTAO_JOYSTIC, GPIO_IN);        
    gpio_pull_up(BOTAO_JOYSTIC);

    gpio_init(GREEN_LED_RGB);                 
    gpio_set_dir(GREEN_LED_RGB, GPIO_OUT);  
    gpio_put(GREEN_LED_RGB, 0);  // Iniciando desligado
    
    gpio_set_function(BLUE_LED_RGB, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(BLUE_LED_RGB);
    pwm_set_clkdiv(slice, DI_DF);
    pwm_set_wrap(slice, WRAP);
    pwm_set_gpio_level(BLUE_LED_RGB, 0);
    pwm_set_enabled(slice, true);

    gpio_set_function(RED_LED_RGB, GPIO_FUNC_PWM);
    slice = pwm_gpio_to_slice_num(RED_LED_RGB);
    pwm_set_clkdiv(slice, 4.0);
    pwm_set_wrap(slice, 65535);
    pwm_set_gpio_level(RED_LED_RGB, 0);
    pwm_set_enabled(slice, true); 

    // ADC
    adc_init();
    adc_gpio_init(JOY_X);
    adc_gpio_init(JOY_Y);

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init(&ssd, SCREEN_WIDTH, SCREEN_HEIGHT, false, ENDERECO, I2C_PORT); // Inicializa o display
    ssd1306_config(&ssd); // Configura o display
    ssd1306_send_data(&ssd); // Envia os dados para o display

    // Limpa o display. O display inicia com todos os pixels apagados.
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);

    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, posicao_y, posicao_x, LADO_QUADRADO, LADO_QUADRADO, true, true);
    ssd1306_send_data(&ssd);
}

uint16_t ler_adc(uint8_t canal)
{
    adc_select_input(canal);
    return adc_read();
}

void atualizar_posicao_quadrado(uint16_t adc_x, uint16_t adc_y)
{
    uint16_t tela_maxima_x = SCREEN_WIDTH - LADO_QUADRADO - borda; 
    uint16_t tela_maxima_y = SCREEN_HEIGHT - LADO_QUADRADO - borda;

    posicao_x = converter_adc_para_tela(CENTRO_X, adc_x, tela_maxima_x);
    posicao_y = SCREEN_HEIGHT - LADO_QUADRADO - converter_adc_para_tela(CENTRO_Y, adc_y, tela_maxima_y);

    ssd1306_fill(&ssd, false);

    for (int i = 0; i < borda; i++)
    {
        ssd1306_hline(&ssd, 0, SCREEN_WIDTH - 1, i, true);
        ssd1306_hline(&ssd, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 - i, true);
        ssd1306_vline(&ssd, i, 0, SCREEN_HEIGHT - 1, true);
        ssd1306_vline(&ssd, SCREEN_WIDTH - 1 - i, 0, SCREEN_HEIGHT - 1, true);
    }

    ssd1306_rect(&ssd, posicao_y, posicao_x, LADO_QUADRADO, LADO_QUADRADO, true, true);
    ssd1306_send_data(&ssd);
}

int converter_adc_para_tela(uint16_t centro, uint16_t valor_adc, uint16_t tela_max)
{
    uint16_t min = centro;     
    uint16_t max = 4095 - centro;
    
    int offset = valor_adc - centro; 

    int valor;
    if (offset < 0) {
        valor = ((offset * (tela_max / 2)) / min) + (tela_max / 2);
    } else {
        valor = ((offset * (tela_max / 2)) / max) + (tela_max / 2);
    }

    if (valor < 0) 
        valor = 0;

    if (valor > tela_max) 
        valor = tela_max;

    if (valor < borda)
        valor = borda;

    return valor;
}

void atualizar_led_rgb(uint led_pin, uint16_t valor_adc, uint16_t centro)
{
    int offset, max;
    if (!leds_azul_vermelho_on)
    {
        pwm_set_gpio_level(led_pin, 0);
    }
    else
    {
        offset = valor_adc - centro;
        if (offset > -ZONA_OFF && offset < ZONA_OFF)
        {
            pwm_set_gpio_level(led_pin, 0);
            return;
        }
        else
        {
            if (offset > 0)
                max = 4095 - centro;
            else
                max = centro;
            
            pwm_set_gpio_level(led_pin, ((abs(offset) - ZONA_OFF) * 65535) / (max - ZONA_OFF));
        }
    }
}

static void gpio_irq_handler(uint gpio, uint32_t events)
{
    uint32_t tempo_atual = to_us_since_boot(get_absolute_time());

    if (tempo_atual - ultimo_tempo > 220000)
    {
        ultimo_tempo = tempo_atual;

        if (gpio == BOTAO_A)
        {
            leds_azul_vermelho_on = !leds_azul_vermelho_on;
        }

        if (gpio == BOTAO_JOYSTIC)
        {
            gpio_put(GREEN_LED_RGB, !gpio_get(GREEN_LED_RGB));
            if (borda == 1)
            {
                borda = 4;
            }
            else if (borda == 4)
            {
                borda = 1;
            }
        }
    }
}