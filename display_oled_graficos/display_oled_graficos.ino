 /*******************************************************************************
  * 
  * Exemplo de uso do display OLED com controlador SSD1306.
  * 
  * Referências:
  * https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives
  * https://github.com/adafruit/Adafruit_SSD1306
  * 
  * Criado por: Erick León, 2022
  * 
  ********************************************************************************/

#include <Wire.h>                  // Biblioteca para uso da comuicação I2C
#include <Adafruit_GFX.h>          // Biblioteca para construção de gráficos
#include <Adafruit_SSD1306.h>      // Biblioteca para comunicação com o display OLED

// Configuração do display OLED. Parâmetros:
//    - largura_do_display: 128 pixels
//    - altura_do_display: 64 pixels
//    - biblioteca de comunicação I2C utilizada
//    - pino de Reset do display (-1 em caso de não ter)
Adafruit_SSD1306 display(128, 64, &Wire, -1); 

int x = 0;                         // variável que guarda a posição dos objetos em movimento
unsigned long time = millis();     // variável que guarda o instante da última atualização da tela do display

// esta função só roda uma vez, no início
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

// esta função se repete indefinidamente
void loop() {
    x++;                           // aumenta a posição desejada, para a direitaN
    if (x>display.width()) x = 0;  // se chegou no final da tela, volta para o início
    
    display.clearDisplay();        // limpa o buffer da tela
    display.setCursor(0,0);        // coloca o cursor de texto no canto superior esquerdo do buffer da tela
    display.print(millis()-time);  // mostra no buffer da tela quanto tempo passou desde a última atualização
    time = millis();               // guarda o instante desta atualização
    display.println(" ms");        // mostra no buffer da tela a unidade de tempo do intervalo

    // desenha no buffer da tela, na posição 'x', um círculo grande vazio, com cor branca:
    display.drawCircle(x+1, display.height()/2, display.height()/2 -1, SSD1306_WHITE);
    // desenha no buffer da tela, na posição 'x', um círculo pequeno cheio, com cor branca:
    display.fillCircle(x+1, display.height()/2, x/4, SSD1306_WHITE);
    // desenha no buffer da tela uma curva senoidal, invertendo a cor (branco fica preto, preto fica branco):
    for (int idx=0; idx<128;idx++) display.drawPixel(idx, 32+32*sin(float(idx+x)/18.0), SSD1306_INVERSE);
    
    display.display();             // atualiza a tela com as informações do buffer
}
