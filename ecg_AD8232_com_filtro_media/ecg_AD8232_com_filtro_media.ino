/*
 * Exemplo de coleta de dados de ECG usando o módulo AD8232.
 * O sinal coletado é coletado a 1800Hz, e passa por um filtro
 * digital simples (média de 30 pontos) para eliminar o ruído 
 * da rede elétrica em 60Hz. Assim, a frequência final do sinal
 * apresentado na porta serial é de 1800Hz/30 = 60Hz.
 * O LED interno da placa Arduino é aceso quando o módulo AD8232
 * detecta um eletrodo solto.
 * 
 * Criado por: Erick Dario León Bueno de Camargo, 2023
 */

// Frequencia de amostragem desejada, em Hz:
long frequencia = 1800;

unsigned long atraso_us = (1000000.0/frequencia);
int medida;
unsigned long tempo_atual, tempo_anterior;
int contador;

/////////////////////////////////////////////////////////////////
// Esta função só roda uma vez, no início
void setup() {
  // initialize the serial communication:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(4, INPUT); 
  pinMode(5, INPUT); 
  tempo_anterior = micros();
  medida = 0;
  contador = 0;
}

/////////////////////////////////////////////////////////////////
// Esta função se repete indefinidamente
void loop() {

  tempo_atual = micros();

  if((digitalRead(4) == 1)||(digitalRead(5) == 1)){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else{
    if (digitalRead(LED_BUILTIN) == 1){ // se está aceso, apaga
      digitalWrite(LED_BUILTIN, LOW);
    }
    
    // o abs() evita problemas com overflow da função micros()
    if (abs(tempo_atual - tempo_anterior) > atraso_us) {
      tempo_anterior = tempo_atual;
      medida = medida + analogRead(A0); // soma a medida atual
      contador = contador + 1;
    }
    if (contador >= 30){
      Serial.println(medida/30);
      medida = 0;
      contador = 0;
    }
  }
}
