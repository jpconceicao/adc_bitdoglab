# Projeto: Controle via UART com Raspberry Pi Pico W

## Descrição
Este projeto implementa uma interface de controle via UART utilizando a Raspberry Pi Pico W. O sistema recebe caracteres pela UART e, dependendo do caractere recebido, realiza diferentes operações, como exibição no display OLED SSD1306 ou alteração do estado de uma matriz de LEDs RGB.

## Autor
**Jorge Palma**  
**Data:** 03/02/2025

## Link do vídeo
- https://drive.google.com/file/d/1HcCegf9VKohb0-SqYr80QXcsILHDdz7n/view?usp=sharing

## Recursos Utilizados
- **Microcontrolador:** Raspberry Pi Pico W
- **Display OLED:** SSD1306 via I2C
- **UART:** Para recepção de comandos
- **Matriz de LEDs RGB:** Controlada via PIO
- **Botões Físicos:** Para interação com LED RGB

## Pinos Utilizados

### UART (uart0 - Padrão)
- **TX:** GPIO 0
- **RX:** GPIO 1

### I2C (i2c1 - Display SSD1306)
- **SDA:** GPIO 14
- **SCL:** GPIO 15
- **Endereço:** 0x3C

### LEDs RGB
- **Vermelho:** GPIO 13
- **Verde:** GPIO 11
- **Azul:** GPIO 12

### Botões
- **Botão A:** GPIO 5
- **Botão B:** GPIO 6

### Matriz de LEDs RGB
- **Saída de Controle:** GPIO 7

## Funcionalidades
1. **Recepção de caracteres via UART**
   - Se um número de '0' a '9' for recebido, aciona a matriz de LEDs para exibir o correspondente.
   - Exibe o caractere recebido no display OLED.

2. **Controle de LEDs RGB via botões**
   - O **Botão A** liga/desliga o LED **Verde**.
   - O **Botão B** liga/desliga o LED **Azul**.
   - Mensagens de status são exibidas no display OLED e no terminal UART.

## Dependências
- **Pico SDK**
- **Bibliotecas de hardware da Raspberry Pi Pico (hardware/uart, hardware/i2c, hardware/pio, etc.)**
- **Bibliotecas customizadas:**
  - `ssd1306.h` (para controle do display OLED)
  - `matriz_led_control.h` (para controle da matriz de LEDs RGB)
  - `vetores_numeros.h` (para desenhos da matriz de LEDs)

## Como Compilar e Executar
1. Certifique-se de que o **Pico SDK** está configurado corretamente.
2. Clone este repositório:
   ```sh
   git clone <URL_DO_REPOSITORIO>
   cd <NOME_DO_REPOSITORIO>
   ```
3. Compile o projeto:
   ```sh
   mkdir build
   cd build
   cmake ..
   make
   ```
4. Conecte a Raspberry Pi Pico W ao seu computador e envie o arquivo `.uf2` gerado:
   ```sh
   cp nome_do_firmware.uf2 /media/$USER/RPI-RP2/
   ```

## Como Testar
1. Conecte-se à UART da BitDogLab usando um emulador de terminal (exemplo: `minicom`, `screen` ou `PuTTY`).
2. Envie caracteres pelo terminal e observe a resposta no display OLED e na matriz de LEDs RGB.
3. Pressione os botões para alternar os LEDs RGB e veja as mensagens no terminal.

