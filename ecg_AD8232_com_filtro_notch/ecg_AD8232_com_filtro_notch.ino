/*
 * Exemplo de coleta de dados de ECG usando o módulo AD8232.
 * O sinal coletado passa por um filtro notch digital para eliminar 
 * o ruído da rede elétrica em 60Hz.
 * O LED interno da placa Arduino é aceso quando o módulo AD8232
 * detecta um eletrodo solto.
 * 
 * Filtro notch baseado em:
 *   CHOY, T. T. C.; LEUNG, P. M. Real time microprocessor-based 
 *   50 Hz notch filter for ECG. Journal of biomedical engineering, 
 *   v. 10, n. 3, p. 285-288, 1988.
 * 
 * Opções do filtro variam com a frequência de aquisição (Fs):
 *  Para Fs = 120Hz, usar "N_PONTOS 2" e "N 1"
 *  Para Fs = 240Hz, usar "N_PONTOS 3" e "N 2"
 *  Para Fs = 360Hz, usar "N_PONTOS 4" e "N 3"
 *  "a" deve estar entre 0 e 1:
 *    - "a" próximo de 0 distorce mais o sinal de ECG, mas é 
 *      menos suscetível a variações nas frequências do ruído
 *      e de amostragem;
 *    - "a" próximo de 1 distorce menos o sinal de ECG, mas é
 *      mais suscetível a variações nas frequências do ruído
 *      e de amostragem.
 * 
 * Criado por: Erick Dario León Bueno de Camargo, 2023
 */

// Frequencia de amostragem desejada, em Hz:
long frequencia = 120;

// Constantes necessárias para o filtro notch
#define N_PONTOS 2
#define N 1
#define a 0.05

unsigned long atraso_us = (1000000.0/frequencia);
int X[N_PONTOS];   // vetor com as medidas atuais
int Y[N_PONTOS];   // vetor com as medidas filtradas
unsigned long tempo_atual, tempo_anterior;
int contador;

/////////////////////////////////////////////////////////////////
// Esta função só roda uma vez, no início
void setup() {
  // initialize the serial communication:
  Serial.begin(115200);
  pinMode(4, INPUT); 
  pinMode(5, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  tempo_anterior = micros();
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
      X[contador] = analogRead(A0); // realiza medida atual
      int ponto_anterior = (contador+N_PONTOS-N)%N_PONTOS;
      
      Y[contador] = ((X[contador]+X[ponto_anterior])/2) + a*(((X[contador]+X[ponto_anterior])/2) - Y[ponto_anterior]);
      Serial.println(Y[contador]);
      contador = contador + 1;
      if (contador >= N_PONTOS){
        contador = 0;
      }
    }
  }
}
