// Teste com display LCD
//
// Criado por: Lucas Jácome Cabral, 2022

#include <LiquidCrystal.h> // comando para incluir biblioteca para facilitar o uso do display

// Configura os pinos conectados do display: rs, enable, d4, d5, d6, d7
LiquidCrystal lcd(6, 7, 5, 4, 3, 2); // rs, enable, d4, d5, d6, d7

// esta função só roda uma vez, no início
void setup() {
    lcd.begin(16, 2); // Numero de colunas e linhas do display
}

// esta função se repete indefinidamente
void loop() 
{
    lcd.clear();             // limpa o display e ajusta o cursor no canto superior esquerdo
    lcd.print("Eu amo");     // Comando para enviar texto para o display
    lcd.setCursor(0, 1);     // Comando para enviar o cursor para um ponto do display, neste caso coluna zero, linha 1
    lcd.print("1..");        // Comando para enviar texto para o display
    delay(1000);             // aguarda 1 segundo (1000 ms)
    lcd.setCursor(0,1);      // voltando cursor para a posição 0,1; senão ele continua escrevendo de onde parou...
    lcd.print("1..2..");     // Comando para enviar texto para o display
    delay(1000);             // aguarda 1 segundo (1000 ms)
    lcd.setCursor(0,1);      // voltando cursor para a posição 0,1; senão ele continua escrevendo de onde parou...
    lcd.print("1..2..3..");  // Comando para enviar texto para o display
    delay(1000);             // aguarda 1 segundo (1000 ms)
    lcd.setCursor(0,1);      // voltando cursor para a posição 0,1; senão ele continua escrevendo de onde parou...
    lcd.print("Eletronica"); // Comando para enviar texto para o display
    delay(3000);             // aguarda 3 segundos (3000 ms)
}
