// Biblioteca para uso do ADC do Arduino Uno diretamente a partir dos registradores.
//
// A taxa de aquisicao teorica eh dada por:
// [taxa_ADC] = [clock do ADC] / [prescaler] / [ciclos para conversao]
//
// Em um Arduino Uno ou um Arduino Nano, o clock do ADC é de 16MHz, e o ADC usa
// 13 ciclos para conversao. 
//
// A taxa de aquisicao eh entao controlada pelo prescaler do ADC,
// que é passada a função adc_setup_8bit() (ou adc_setup_10bit()),
// conforme a lista abaixo (taxas teoricas e reais em um Arduino Nano):
// - PS_4: 307.7 kHz (taxa real: aprox. 196.6 kHz) ou (~235 kHz com 8bits) (usar 8 bits)
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

// define um atraso de um ciclo de clock (atraso de 62.5ns em um AtMega a 16MHz)
#define NOP __asm__ __volatile__ ("nop\n\t")

// define uma funcao de atraso em microsegundos, baseado no intervalo de um ciclo de clock
#define delay_us(t_us) for (int idx = 0; idx < int(t_us/0.0625); idx++) NOP;

//////////////////////////////////////////////////////////////////////////////////
// Define variaveis globais
int __adc_contador_medidas__ = 0;
byte __coleta_finalizada__ = 0;
byte *__vetor_medidas_H__;
byte *__vetor_medidas_L__;
int __n_medidas__ = 0;
byte __8bit__ = 0;
uint8_t __ADLAR_usado__ = 0;

const unsigned char PS_2 = (1 << ADPS0);                   // nao funciona
const unsigned char PS_4 = (1 << ADPS1);                   // usar 8-bit
const unsigned char PS_8 = (1 << ADPS1) | (1 << ADPS0);    // usar 8-bit
const unsigned char PS_16 = (1 << ADPS2);                  // maximo com 10-bit
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) |(1 << ADPS0);

//////////////////////////////////////////////////////////////////////////////////
// Funcao de configuracao do ADC.
//
// Registradores utilizados:
//
// - ADMUX (valor inicial: 0x00):
//   - bits 0 a 3 (MUX0 a MUX3): (Analog Channel Selection Bits):
//     Configuracao do multiplexador do ADC, ou seja, do pino a ser lido:
//     - 0x00: ADC0; 0x01: ADC1; ... ; 0x08: ADC8; 0x09 a 0x13: reservado; 0x14: 1,1V; 0x15: 0V(GND)
//   - bit 4: reservado.
//   - bit 5 (ADLAR): (ADC Left Adjust Result): define se os 10 bits lidos sao guardados ajustados a esquerda ou a direita
//     - 0: ajuste a direita (padrão). Os zeros que completam os 16 bits (2 bytes, ADCH e ADCL) ficam a esquerda.
//     - 1: ajuste a esquerda, para ler apenas 8 bits (ADCH).
//   - bits 6 e 7 (REFS0 e REFS1): (Reference Selection Bits): para ajustar a tensao de referencia utilizada:
//     - REFS1=0, REFS0=0: usando pino AREF
//     - REFS1=0, REFS0=1: usando AVCC (5V da alimentacao)
//     - REFS1=1, REFS0=0: reservado
//     - REFS1=1, REFS0=1: usando tensão de referência de 1,1V
//
// - ADCSRA (valor inicial: 0x00):
//   - bits 0 a 2 (ADPS0 a ADPS2): (ADC Prescaler Select Bits): ajuste do pre-scaler do ADC.
//   - bit 3 (ADIE): (ADC Interrupt Enable): habilita a interrupcao quando a conversao for finalizada.
//   - bit 4 (ADIF): (ADC Interrupt Flag): Este bit fica em 1 quando a conversao do ADC finaliza, e volta
//     para 0 quando a interrupcao eh executada.
//   - bit 5 (ADATE): (ADC Auto Trigger Enable): liga o disparo automatico do ADC.
//     O ajuste da fonte do disparo é feito no registrador ADCSRB.
//     Para iniciar a proxima coleta assim que terminar a anterior (Free Running Mode), ou seja, realizar
//     leituras o mais rapido possivel, nao eh necessario alterar o registrador ADCSRB.
//   - bit 6 (ADSC): (ADC Start Conversion): Inicia uma conversao (leitura)
//   - bit 7 (ADEN): (ADC Enable):
//     - 0: desabilita (desliga) o ADC. Termina a conversao atual antes, se for o caso.
//     - 1: habilita (liga) o ADC.
//
void adc_setup_8bit(uint8_t pin, unsigned char PS, int nro_medidas, byte *v_medidas) {
    // Garante que os registradores ADCSRA e ADCSRB estao zerados:
    ADCSRA = 0;
    ADCSRB = 0;
    
    // Ajusta ADC com tensao de referencia 5V, ADLAR e pino desejados
    ADMUX = bit(REFS0) | (1 << ADLAR) | ((pin-A0) & 0x07);
    
    // Ajusta ADCSRA com pre-scaler desejado, liga o disparo automatico e habilita interrupcao
    ADCSRA = bit(ADATE) | bit(ADIE) | PS;

    // Guarda variáveis globais
    __ADLAR_usado__ = 1; // guarda ADLAR usado
    __n_medidas__ = nro_medidas;
    __vetor_medidas_H__ = v_medidas;
    __8bit__ = 1;
}

//////////////////////////////////////////////////////////////////////////////////
// Funcao de configuracao do ADC.
//
void adc_setup_10bit(uint8_t pin, unsigned char PS, int nro_medidas, byte *v_medidas_H, byte *v_medidas_L) {
    // Garante que os registradores ADCSRA e ADCSRB estao zerados:
    ADCSRA = 0;
    ADCSRB = 0;
    
    // Ajusta ADC com tensao de referencia 5V, ADLAR e pino desejados
    ADMUX = bit(REFS0) | ((pin-A0) & 0x07);
    
    // Ajusta ADCSRA com pre-scaler desejado, liga o disparo automatico e habilita interrupcao
    ADCSRA = bit(ADATE) | bit(ADIE) | PS;
    
    // Guarda variáveis globais
    __ADLAR_usado__ = 0; // guarda ADLAR usado
    __n_medidas__ = nro_medidas;
    __vetor_medidas_H__ = v_medidas_H;
    __vetor_medidas_L__ = v_medidas_L;
    __8bit__ = 1;
}

//////////////////////////////////////////////////////////////////////////////////
// Funcao de início de coleta do ADC
void adc_start() {
    __adc_contador_medidas__ = 0;
    __coleta_finalizada__ = 0;
    ADCSRA |= (1 << ADEN);  // habilita o ADC
    ADCSRA |= (1 << ADSC);  // inicia as medidas no ADC
}

//////////////////////////////////////////////////////////////////////////////////
// Funcao de parada de coleta do ADC
void adc_stop() {
    ADCSRA &= ~(1 << ADSC);  // para as medidas no ADC
    ADCSRA &= ~(1 << ADEN);  // desabilita o ADC
}

//////////////////////////////////////////////////////////////////////////////////
// Funcao de interrupcao do ADC
ISR(ADC_vect)
{
    if(__ADLAR_usado__ == 0){ // lendo 10 bits
        // Ler ADCL primeiro!
        __vetor_medidas_L__[__adc_contador_medidas__] = ADCL;  // le 8 bits do ADC
        __vetor_medidas_H__[__adc_contador_medidas__] = ADCH;  // le 8 bits do ADC
    }
    else{ // lendo 8 bits
        __vetor_medidas_H__[__adc_contador_medidas__] = ADCH;  // le 8 bits do ADC
    }
    
    __adc_contador_medidas__++; // incrementa o contador
    if (__adc_contador_medidas__ >= __n_medidas__) // se terminou de ler...
    {
        adc_stop();
        __coleta_finalizada__ = 1;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Espera finalização da conversão
void wait_convertion(){
  while( __coleta_finalizada__ == 0 ) NOP; // enquanto nao termina, aguarda 62.5ns
}

//////////////////////////////////////////////////////////////////////////////////////////////
// troca o canal do ADC
void change_adc_channel(uint8_t pin){
    ADMUX = bit(REFS0) | (__ADLAR_usado__ << ADLAR) | ((pin-A0) & 0x07);
    delay_us(180.0); // aguardar no minimo 90us
}

//////////////////////////////////////////////////////////////////////////////////
// Realiza a leitura de um conjunto de dados
void read_adc() {
    adc_start();
    wait_convertion();
}
