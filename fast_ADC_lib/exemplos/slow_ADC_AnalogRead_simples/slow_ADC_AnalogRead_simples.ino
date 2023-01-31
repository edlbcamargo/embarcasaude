// Exemplo de utilização do ADC do Arduino usando a função analogRead(),
// realizando as medições e enviando os valores medidos pela porta serial.
//
// A IDE do Arduino fornece uma função pronta para uso do ADC, chamada
// analogRead(), que permite realizar medições em até 9615Hz, ou seja,
// 9615 medidas por segundo, aproximadamente. Entretanto, como o envio
// em tempo real do valor medido toma algum tempo, a taxa de medições
// pode cair para cerca de 2000 medidas por segundo (comunicando a
// 115200bps), ou até 170 medidas por segundo (comunicando a 9600bps).
//
// Criado por: Erick Dario León Bueno de Camargo, 2023


int medida;

////////////////////////////////////////////////////////////////////////////////
// Esta função só roda uma vez, no início
void setup() {
  Serial.begin(115200);
  Serial.println("Configuracao_ok!");
}

////////////////////////////////////////////////////////////////////////////////
// Esta função se repete indefinidamente
void loop() {
  medida = analogRead(A0);
  Serial.println(medida);
}
