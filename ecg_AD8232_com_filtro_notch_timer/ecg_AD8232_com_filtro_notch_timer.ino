/*
 * Exemplo de coleta de dados de ECG usando o módulo AD8232, e 
 * configurando o disparo do ADC por timer para controlar a 
 * frequência de aquisição.
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
 * O timer deve ser ajustado de acordo com a frequência de 
 * aquisição desejada:
 *   - Fs = 120Hz, usar "OCR1A = 16666;" e "OCR1B = 16666;"
 *   - Fs = 240Hz, usar "OCR1A = 8332;" e "OCR1B = 8332;"
 *   - Fs = 360Hz, usar "OCR1A = 5554;" e "OCR1B = 5554;"
 *   
 * Criado por: Erick Dario León Bueno de Camargo, 2023
 */

// Constantes necessárias para o filtro notch
#define N_PONTOS 2
#define N 1
#define a 0.95

int X[N_PONTOS];   // vetor com as medidas atuais
int Y[N_PONTOS];   // vetor com as medidas filtradas
int contador;

bool medida_disponivel = false;


/////////////////////////////////////////////////////////////////
// Esta interrupção roda assim que o ADC termina uma medida:
ISR (ADC_vect){
  X[contador] = ADC;
  medida_disponivel = true;
}

EMPTY_INTERRUPT (TIMER1_COMPB_vect); // limpa a configuração do timer

/////////////////////////////////////////////////////////////////
// Esta função só roda uma vez, no início
void setup() {
  // initialize the serial communication:
  Serial.begin(115200);
  pinMode(4, INPUT); 
  pinMode(5, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  contador = 0;

  // configuração do Timer 1
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  TCCR1B = bit (CS11) | bit (WGM12);  // CTC, prescaler = 8
  TIMSK1 = bit (OCIE1B);  // habilita interrupção por comparação
  //OCR1A = 5554;           // 16Mz/8/(5554+1) = 360,036Hz
  //OCR1B = 5554;           // 16Mz/8/(5554+1) = 360,036Hz
  //OCR1A = 8332;           // 16Mz/8/(8332+1) = 240,0096Hz
  //OCR1B = 8332;           // 16Mz/8/(8332+1) = 240,0096Hz
  OCR1A = 16666;          // 16Mz/8/(16666+1) = 119,9976Hz
  OCR1B = 16666;          // 16Mz/8/(16666+1) = 119,9976Hz

  // configuração do ADC
  ADCSRA =  bit (ADEN) | bit (ADIE) | bit (ADIF); // liga o ADC e interrupção (ADC_vect)
  ADCSRA |= bit (ADPS2);              // Prescaler de 16
  ADMUX = bit (REFS0) | (0 & 7);      // leitura do canal 0 (A0)
  ADCSRB = bit (ADTS0) | bit (ADTS2); // ADC dispara com TIMER1_COMPB_vect
  ADCSRA |= bit (ADATE);              // liga o disparo automático
}

/////////////////////////////////////////////////////////////////
// Esta função se repete indefinidamente
void loop() {

  if((digitalRead(4) == 1)||(digitalRead(5) == 1)){
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else{
    digitalWrite(LED_BUILTIN, LOW);
  }
  if(medida_disponivel){
    int ponto_anterior = (contador+N_PONTOS-N)%N_PONTOS;
    Y[contador] = ((X[contador]+X[ponto_anterior])/2) + a*(((X[contador]+X[ponto_anterior])/2) - Y[ponto_anterior]);
    Serial.println(Y[contador]);
  
    contador = contador + 1;
    if (contador >= N_PONTOS){
      contador = 0;
    }
    medida_disponivel = false;
  }
}
