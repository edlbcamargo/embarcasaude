// Exemplo de utilização do ADC do Arduino usando a função analogRead(),
// realizando as medições em uma taxa de amostragem conhecida e enviando
// continuamente os valores medidos pela porta serial.
//
// A IDE do Arduino fornece uma função pronta para uso do ADC, chamada
// analogRead(), que permite realizar medições em até 9615Hz, ou seja,
// 9615 medidas por segundo, aproximadamente. Entretanto, como o envio
// em tempo real do valor medido toma algum tempo, a taxa de medições
// pode cair para cerca de 2000 medidas por segundo (comunicando a
// 115200bps), ou até 170 medidas por segundo (comunicando a 9600bps).
//
// Criado por: Erick Dario León Bueno de Camargo, 2023

// Frequencia de amostragem desejada, em Hz. Usar valores entre 1 e 2000:
long frequencia = 1000;

unsigned long atraso_us = (1000000.0/frequencia);
int medida;
unsigned long tempo_atual, tempo_anterior;

////////////////////////////////////////////////////////////////////////////////
// Esta função só roda uma vez, no início
void setup() {
  Serial.begin(115200);
  Serial.println("Configuracao_ok!");
  tempo_anterior = micros();
}

////////////////////////////////////////////////////////////////////////////////
// Esta função se repete indefinidamente
void loop() {
  tempo_atual = micros();

  // o abs() evita problemas com overflow da função micros()
  if (abs(tempo_atual - tempo_anterior) > atraso_us) {
    tempo_anterior = tempo_atual;
    medida = analogRead(A0);
    Serial.println(medida);
  }
}
