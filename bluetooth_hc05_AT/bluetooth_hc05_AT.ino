// Comunicação com o módulo Bluetooth HC05
//
// Programa de comunicação com o módulo Bluetooth HC05,
// usado para enviar comandos AT para configuração do
// módulo.
//
// IMPORTANTE: Algumas versões do módulo HC05 só entram no modo
// AT se o botão próximo do pino 'Enable' estiver pressionado
// quando o módulo é ligado.
//
// Também pode ser usado para enviar e receber dados de
// um celular, por exemplo, quando o pino 'Key'
// estiver desconectado.
//
// Criado por: Lucas Jácome Cabral, 2022

#include <SoftwareSerial.h>

SoftwareSerial SerialHC05(10, 11); /* RX pino 10 , TX pino 11 */

///////////////////////////////////////////////
// Esta função só roda uma vez, no início
void setup() {
  Serial.begin(115200);   // Porta Serial do computador
  SerialHC05.begin(38400);      // Porta Serial do HC05
  // Caso a comunicação com o HC05 não funcione, teste com outras velocidades:
  // 9600, 19200, 38400 (padrão), 57600, 115200, 230400 ou 460800
  Serial.println("Digite os comandos para o módulo:");
}

///////////////////////////////////////////////
// Esta função se repete indefinidamente
void loop() {
  if (SerialHC05.available()) {      // Se chegou algum dado do módulo
    while (SerialHC05.available()) { // Enquanto tiver dados disponíveis
      char c = SerialHC05.read();    // Lê um caractere
      Serial.write(c);               // Envia o dado na Serial do computador
    }
  }

  if (Serial.available()) {          // Se chegou algum dado do computador
    delay(10);
    String comando = "";
    while (Serial.available()) {     // Enquanto tiver dados disponíveis
      comando += (char)Serial.read();// Lê os caracteres escritos no monitor serial
    }
    SerialHC05.print(comando);     // Envia os dados para o HC05 */
  }
}