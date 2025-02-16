# Projeto: Controle de Display OLED e LED RGB com Raspberry Pi Pico W

## Autor
- Jorge Palma - 16/02/2025

## Link do vídeo
- https://drive.google.com/file/d/1M_RcEuRHKZdQkI5pG2pzWZuyQukl9wuR/view?usp=sharing

## Descrição
Este projeto utiliza uma Raspberry Pi Pico W na placa BitDogLab para controlar um display OLED SSD1306 via I2C e um LED RGB utilizando PWM. O joystick analógico é usado para mover um quadrado na tela do display, enquanto botões físicos alternam o estado dos LEDs e ajustam a borda do display.

## Hardware Utilizado
- Raspberry Pi Pico W
- Display OLED SSD1306 (I2C)
- LED RGB (PWM)
- Joystick analógico (ADC)
- Botões físicos

## Conexões
### I2C (Display OLED SSD1306)
- SDA: GPIO 14
- SCL: GPIO 15
- Endereço: 0x3C

### LED RGB (PWM)
- Vermelho: GPIO 13
- Verde: GPIO 11
- Azul: GPIO 12

### Joystick (ADC)
- Eixo X: ADC1 (GPIO 27)
- Eixo Y: ADC0 (GPIO 26)

### Botões
- Botão A: GPIO 5
- Botão do Joystick: GPIO 22

## Funcionalidades
- Controle do quadrado na tela com o joystick.
- Mudança na borda da tela ao pressionar o botão do joystick.
- Liga/desliga os LEDs vermelho e azul com o botão A.
- Controle de brilho dos LEDs RGB baseado no deslocamento do joystick.

## Compilação e Execução
1. Instale o SDK do Raspberry Pi Pico.
2. Compile o código com CMake e GCC ARM.
3. Carregue o firmware na Pico W.

