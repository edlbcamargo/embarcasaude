/*******************************************************************************
 * 
 * Exemplo de monitor multiparamétrico usando Arduino Uno.
 * Este exemplo utiliza:
 *  - display OLED com controlador SSD1306 para gráficos e informações;
 *  - módulo de oximetria MAX30102 (comunicação I2C (pinos A4 e A5));
 *  - módulo de ECG AD8232 (pino A0);
 *  - monitoramento da bateria (pino A1).
 * 
 * Criado por: Erick León, 2023
 * 
 ********************************************************************************/

// Bibliotecas utilizadas:
#include <Wire.h>                  // comuicação I2C (OLED e Oxímetro)
#include <Adafruit_SSD1306.h>      // comunicação com o display OLED

// Configuração do display OLED. Parâmetros:
//    - largura_do_display: 128 pixels
//    - altura_do_display: 64 pixels
//    - biblioteca de comunicação I2C utilizada
//    - pino de Reset do display (-1 em caso de não ter)
Adafruit_SSD1306 display(128, 64, &Wire, -1);

const byte NUM_PONTOS_GRAF = 55;   // Número de pontos dos gráficos
unsigned long tempoAnterior = millis(); // para medir taxa de medição dos dados
float freqAquisicao;                    // taxa de medição dos dados

// Outras bibliotecas com as funções utilizadas:
#include "ecg.h"                   // funções do ECG AD8232
#include "oximetro.h"              // funções do sensor MAX30102
#include "tela.h"                  // funções do display OLED

/////////////////////////////////////////////////////////////////////
// esta função só roda uma vez, no início
void setup() {
    Wire.begin();                  // inicia a comunicação I2C
    delay(500);                    // aguarda 500ms

    configuraAdcEcg();
    inicializaDisplay();
    inicializaOximetro();
}

/////////////////////////////////////////////////////////////////////
// esta função se repete indefinidamente
void loop() {
    // mede a taxa de medição dos dados
    freqAquisicao = 1000.0/(millis()-(float)tempoAnterior);
    tempoAnterior = millis();
    
    medeOximetro(); // dispara leitura do oxímetro e do ECG
    desenhaTela();  // atualiza tela
}
