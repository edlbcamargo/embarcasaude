// Exemplo de utilização da biblioteca fast_ADC_lib.h com 10 bits
// de resolução do ADC.
//
// Programa de exemplo de leitura de dados pelo ADC do Arduino
// utilizando a biblioteca fast_ADC_lib.h.
//
// A leitura dos dados é feita utilizando-se os registradores
// do Arduino, o que permite uma coleta de dados muito mais
// rápida do que usando a função AnalogRead();
//
// A taxa de aquisição é controlada pelo pre-scaler do ADC,
// passada à função adc_setup_8bit() (ou adc_setup_10bit()),
// conforme a lista abaixo (taxas teóricas):
// - PS_4: 307.7 kHz (taxa real: aprox. 196.6 kHz) (usar 8 bits)
// - PS_8: 153.8 kHz (taxa real: aprox. 153.17 kHz) (usar 8 bits)
// - PS_16: 76.9 kHz (taxa real: aprox. 76.77 kHz)
// - PS_32: 38.5 kHz (taxa real: aprox. 38.38 kHz)
// - PS_64: 19.2 kHz (taxa real: aprox. 19.19 kHz)
// - PS_128: 9.6 kHz (taxa real: aprox. 9.596 kHz)
//
// Para taxas acima de 76,9 kHz (PS_16), o ADC perde resolução, e
// o uso dos 10 bits é desnecessário (usar 8 bits nesses casos).
//
// Criado por: Erick Dario León Bueno de Camargo, 2023

#include "fast_ADC_lib.h"

#define NRO_MEDIDAS 500

byte vetor_medidas_H[NRO_MEDIDAS];
byte vetor_medidas_L[NRO_MEDIDAS];

////////////////////////////////////////////////////////////////////////////////
// Esta função só roda uma vez, no início
void setup() {
  adc_setup_10bit(A0,PS_16,NRO_MEDIDAS,vetor_medidas_H,vetor_medidas_L);

  Serial.begin(115200);
  Serial.println("Configuracao_ok!");
}

////////////////////////////////////////////////////////////////////////////////
// Esta função se repete indefinidamente
void loop() {
  // realiza as medidas
  read_adc();

  // envia as medidas pela porta serial
  for (int idx = 0; idx < NRO_MEDIDAS; idx++){
    float medida = ((vetor_medidas_H[idx]<<8)+vetor_medidas_L[idx])*5.0/1023.0;
    Serial.println(medida);
  }

  // aguarda 1 segundo (1000ms)
  delay(1000);
}
