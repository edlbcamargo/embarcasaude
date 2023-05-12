/*******************************************************************************
 * Funções usadas com o sensor AD8232 (ECG).
 * 
 * Criado por: Erick León, 2023
 ********************************************************************************/

const byte NUM_PONTOS_ECG = NUM_PONTOS_GRAF; // Número de pontos do gráfico do ECG

int ecg_vec[NUM_PONTOS_ECG]; // vetor com as medidas atuais
bool lendo_bateria=false; // variável que verifica se é uma leitura de bateria
int tensao_bateria=0;

////////////////////////////////////////////////////////////////
// 'Anda' com o vetor de medidas do ADC, e guarda a nova medida
// na última posição.
// Esta interrupção roda assim que o ADC termina uma medida.
ISR (ADC_vect){
    if(lendo_bateria){
        tensao_bateria = ADC;  // lê valor do ADC (tensão na bateria)
        ADMUX = bit (REFS0) | (0 & 7);      // volta para canal do ECG (A0)
        lendo_bateria = false;
    }
    else{ // lendo ECG
        // 'anda' com os dados do gráfico para a esquerda e guarda nova medida
        for (int idx=0; idx<NUM_PONTOS_ECG-1;idx++) ecg_vec[idx] = ecg_vec[idx+1];
        ecg_vec[NUM_PONTOS_ECG-1] = ADC;  // lê valor do ADC (sinal de ECG)
    }
}

////////////////////////////////////////////////////////////////
// Configura o ADC com prescaler de 128, leitura única no pino
// A0, disparando interrupção (ADC_vect) ao final da medição.
// Esta função só roda quando chamada.
void configuraAdcEcg(){
    // configuração do ADC usado para medir o sinal de ECG
    ADCSRA =  bit (ADEN) | bit (ADIE) | bit (ADIF);     // liga o ADC e interrupção (ADC_vect)
    ADCSRA |= bit (ADPS0) |  bit (ADPS1) | bit (ADPS2); // Prescaler de 128
    ADMUX = bit (REFS0) | (0 & 7);      // leitura do canal 0 (A0)
}

////////////////////////////////////////////////////////////////
// Inicia uma conversão (medida de tensão na bateria) do ADC.
// Esta função só roda quando chamada.
void disparaLeituraBateria(){
    lendo_bateria=true; // avisa a interrupção do ADC que não é leitura de ECG
    ADMUX = bit (REFS0) | (1 & 7);      // leitura do canal 1 (A1)
    bitSet (ADCSRA, ADSC);  // inicia uma medida no ADC
}

////////////////////////////////////////////////////////////////
// Inicia uma conversão (medida de ECG) do ADC.
// Esta função só roda quando chamada.
void disparaLeituraEcg(){
    bitSet (ADCSRA, ADSC);  // inicia uma medida no ADC
}
