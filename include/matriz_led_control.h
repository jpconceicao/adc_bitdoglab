/* 
* Feita por:     Jorge Palma 
* Data:          30/01/2025
*/

#ifndef MATRIZ_LED_CONTROL
#define MATRIZ_LED_CONTROL

#include <stdio.h>
#include <stdint.h>
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "pio_matrix.pio.h"

#define NUM_PIXELS 25
#define FRAMES_FLAGS 10
#define MAX_LEDS 25

typedef struct {
    PIO pio;
    bool ok;
    uint16_t i;
    double r;
    double g;
    double b;
    uint sm;
} pio_t;

void init_pio_routine(pio_t * meu_pio, uint PIN);
void imprimir_binario(int num) ;
uint32_t matrix_rgb(double b, double r, double g);
void desenho_pio(double *desenho, pio_t * meu_pio);
void desenho_pio_rgb(double *desenho, pio_t * meu_pio);

#endif  // MATRIZ_LED_CONTROL