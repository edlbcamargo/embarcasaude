// Leitura de oxigenação do sangue utilizando Arduino 
// Programa baseado no Exemplo 8:SPO2 da biblioteca MAX30105
// Modificado por Lucas Jácome e Erick León, 2022
 
#include <LiquidCrystal.h>
#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // pinos usados no LCD: (rs, enable, d4, d5, d6, d7)
MAX30105 particleSensor;

#define BUFFER_LENGTH 100  // tamanho dos vetores de dados. Deve ser 100!
#define N 10               // pontos medidos a cada cálculo de SpO2

#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
  uint16_t irBuffer[BUFFER_LENGTH];   // vetor dos dados do LED ir
  uint16_t redBuffer[BUFFER_LENGTH];  // vetor dos dados do LED vermelho
#else
  uint32_t irBuffer[BUFFER_LENGTH];   // vetor dos dados do LED ir
  uint32_t redBuffer[BUFFER_LENGTH];  // vetor dos dados do LED vermelho
#endif

int32_t spo2;          // guarda o valor de SpO2
int8_t validSPO2;      // guarda se a medida é válida
int32_t heartRate;     // guarda a frequência cardíaca
int8_t validHeartRate; // guarda se a frequência é válida
byte readLED = 13;     // pino do LED interno do Arduino
  
// esta função só roda uma vez, no início
void setup()
{
    // inicializa o LCD
    lcd.begin(16, 2); // usando um display LCD de 16x2 caracteres
    lcd.clear();

    pinMode(readLED, OUTPUT);
  
    // Inicializa o sensor
    if (!particleSensor.begin(Wire, I2C_SPEED_FAST))
    {
        lcd.setCursor(0, 0);
        lcd.println(F("Sem sensor!"));
        while (1); // Se não encontra o sensor, fica parado aqui em um loop infinito
    }

    // configura o sen14)sor
    byte ledBrightness = 60; // Opções: 0=desligado a 255=50mA; padrão: 60
    byte sampleAverage = 4;  // Opções: 1, 2, 4, 8, 16, 32; padrão: 4
    byte ledMode = 2;        // Opções: 1=apenas verm, 2=verm+IR, 3=verm+IR+verde; padrão: 2
    byte sampleRate = 100;   // Opções: 50, 100, 200, 400, 800, 1000, 1600, 3200; padrão: 100
    int pulseWidth = 411;    // Opções: 69, 118, 215, 411; padrão: 411
    int adcRange = 4096;     // Opções: 2048, 4096, 8192, 16384; padrão: 4096
    particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);

    lcd.setCursor(0, 0);
    lcd.println(F("Iniciando medida"));

    // Completa os vetores com medidas:
    for (byte i = 0 ; i < BUFFER_LENGTH ; i++)
    {
        while (particleSensor.available() == false) // se ainda não tem novos dados...
            particleSensor.check(); // verifica se tem novos dados
    
        redBuffer[i] = particleSensor.getRed(); // guarda dado do LED vermelho no vetor
        irBuffer[i] = particleSensor.getIR(); // guarda dado do LED ir no vetor
        particleSensor.nextSample(); // dados lidos... fazer nova leitura
    }
}
  
// esta função se repete indefinidamente
void loop()
{
    // desloca os valores nos vetores em N posições
    for (byte i = N; i < BUFFER_LENGTH; i++)
    {
        redBuffer[i - N] = redBuffer[i];
        irBuffer[i - N] = irBuffer[i];
    }

    // faz mais N medições, nas últimas N posições dos vetores
    for (byte i = BUFFER_LENGTH-N; i < BUFFER_LENGTH; i++)
    {
      while (particleSensor.available() == false) // se ainda não tem novos dados...
        particleSensor.check(); // verifica se tem novos dados

      // pisca o LED interno do Arduino para avisar que está medindo
      digitalWrite(readLED, !digitalRead(readLED));

      redBuffer[i] = particleSensor.getRed(); // guarda dado do LED vermelho no vetor
      irBuffer[i] = particleSensor.getIR(); // guarda dado do LED ir no vetor
      particleSensor.nextSample(); // dados lidos... fazer nova leitura
    }

    // calcula SpO2 e frequência cardíaca
    maxim_heart_rate_and_oxygen_saturation(irBuffer, BUFFER_LENGTH, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);

    // apresenta valores no display LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(F("SPO2="));
    if(validSPO2){
        lcd.print(spo2, DEC);
        lcd.print(F("%"));
    }
    else{
        lcd.print(F("???"));
    }
    if(validHeartRate){
        lcd.print(F(", "));
        lcd.print(heartRate, DEC);
        lcd.print(F("bpm"));
    }
    
    lcd.setCursor(0, 1);
    lcd.print(F("Red="));
    lcd.print(redBuffer[BUFFER_LENGTH-1]/256);
    lcd.print(F(" IR="));
    lcd.print(irBuffer[BUFFER_LENGTH-1]/256);
}
