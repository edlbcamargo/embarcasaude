// Exemplo de utilização do ADC do Arduino usando a função analogRead(),
// realizando as medições em uma taxa de amostragem conhecida.
//
// A IDE do Arduino fornece uma função pronta para uso do ADC,
// chamada analogRead(), que permite realizar medições em até
// 9615Hz, ou seja, 9615 medidas por segundo, aproximadamente.
//
// Criado por: Erick Dario León Bueno de Camargo, 2023

#define NRO_MEDIDAS 500

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
  Serial.begin(115200);
  Serial.println("Configuracao_ok!");
}

////////////////////////////////////////////////////////////////////////////////
// Esta função se repete indefinidamente
void loop() {
  // realiza as medidas
  mede_adc(1000); // realiza as medidas a 1kHz, ou 1000Hz

  // envia as medidas pela porta serial
  for (int idx = 0; idx < NRO_MEDIDAS; idx++){
    float medida = vetor_medidas[idx]*5.0/1023.0;
    Serial.println(medida);
  }

  // aguarda 1 segundo (1000ms)
  delay(1000);
}
