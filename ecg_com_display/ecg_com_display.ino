// Exemplo de coleta de dados de ECG usando a função AnalogRead(),
// e mostrando o sinal medido em um display OLED de 128x64 pixels.
//
// Criado por: Erick Dario León Bueno de Camargo, 2023

#include <Wire.h>                  // Biblioteca para uso da comuicação I2C
#include <Adafruit_GFX.h>          // Biblioteca para construção de gráficos
#include <Adafruit_SSD1306.h>      // Biblioteca para comunicação com o display OLED

// Configuração do display OLED. Parâmetros:
//    - largura_do_display: 128 pixels
//    - altura_do_display: 64 pixels
//    - biblioteca de comunicação I2C utilizada
//    - pino de Reset do display (-1 em caso de não ter)
Adafruit_SSD1306 display(128, 64, &Wire, -1);
//int x = 0;                         // variável que guarda a posição dos objetos em movimento

#define NRO_MEDIDAS 128

int vetor_medidas[NRO_MEDIDAS];

////////////////////////////////////////////////////////////////////////////////
// Realiza as medidas do ADC, guardando os valores medidos em vetor_medidas[].
void mede_adc(long frequencia){
  long atraso_us = (1000000.0/frequencia)-104.0; // A função analogRead() leva aprox. 104us para rodar
  if (atraso_us<0) atraso_us=0;
  for (int idx = 0; idx < NRO_MEDIDAS; idx++){
    vetor_medidas[idx] = analogRead(A0);
    if (atraso_us < 16000){
      delayMicroseconds(atraso_us);
    }
    else{
      delay(atraso_us/1000);
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
// Esta função só roda uma vez, no início
void setup() {
  Wire.begin();                  // inicia a comunicação I2C
  delay(500);                    // aguarda 500ms
  
  // inicializa a comunicação com o display, no endereço I2C 0x3C
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    for(;;);   // Se não conseguiu inicializar o display, fica aqui e não continua o programa
  }
  
  display.setTextSize(1);        // escolhe o tamanho da letra
  display.setTextColor(WHITE);   // escolhe a cor da letra
}

////////////////////////////////////////////////////////////////////////////////
// Esta função se repete indefinidamente
void loop() {
  int maximo=0;
  int minimo=1023;
  // realiza as medidas
  mede_adc(64); // realiza as medidas a 64Hz

  // verifica máximo e mínimo da medida
  for (int idx = 0; idx < NRO_MEDIDAS; idx++){
    if (vetor_medidas[idx]>maximo) maximo = vetor_medidas[idx];
    if (vetor_medidas[idx]<minimo) minimo = vetor_medidas[idx];
  }

  display.clearDisplay();        // limpa o buffer da tela
  display.setCursor(0,0);        // coloca o cursor de texto no canto superior esquerdo do buffer da tela
  display.print((maximo-minimo)*5000.0/1023.0);  // mostra no buffer da tela a amplitude do sinal medido
  display.print("mV");
  display.print("; ");
  display.print(maximo);         // mostra no buffer da tela o valor máximo do sinal medido
  display.print(" a ");
  display.print(minimo);         // mostra no buffer da tela o valor mínimo do sinal medido

  // desenha a curva medida no buffer da tela
  for (int idx=0; idx<127;idx++){
    int y1 = (vetor_medidas[idx]-minimo)*53/(maximo-minimo+1)+10;
    int y2 = (vetor_medidas[idx+1]-minimo)*53/(maximo-minimo+1)+10;
    display.drawLine(idx, y1, idx+1, y2, WHITE);
  }

  display.display();             // atualiza a tela com as informações do buffer
}
