/*******************************************************************************
 * Funções usadas com o sensor MAX30102 (oxímetro).
 * 
 * Criado por: Erick León, 2023
 ********************************************************************************/

#include "MAX30105.h"            // comunicação com sensor MAX30102

#define PONTOS_PICO_FREQ 5       // nro de pontos para verificar se ocorreu pico
const byte NUM_PONTOS_OX = NUM_PONTOS_GRAF;   // Número de pontos do gráfico do Oxim

MAX30105 meuOximetro;            // declaração do sensor MAX30102

const byte NUM_PONTOS_FREQ = 4;  // Número de medidas usadas na média
byte freqs_vec[NUM_PONTOS_FREQ]; // Vetor de medidas de frequência
byte posFreq = 0;                // guarda a posição atual do vetor
float batidasPorMinuto;          // frequência (batidas por minuto)
int freqMedia;                   // média da frequência
long valorIR;                    // valor lido pelo sensor
int valorIR_vec[NUM_PONTOS_OX]; // vetor com valores lidos (IR)
long maximoOxim;                 // valor máximo do gráfico do oxímetro
long minimoOxim;                 // valor mínimo do gráfico do oxímetro
long amplitudeOxim;              // amplitude do gráfico do oxímetro
unsigned long ultimaBatida = millis(); // instante da última batida
int contadorTemperatura = 0;     // contador da última medida da temperatura
int contadorBatida = 100;        // contador da última batida detectada
float temperatura;               // valor da temperatura
long media = 0;                  // média dos valores lidos

// para o cálculo do SpO2:
int16_t spo2 = 0;                // guarda o valor de SpO2
#include "spo2.h"                // biblioteca modificada p/ cálculo do SpO2

/////////////////////////////////////////////////////////////////////
// Inicializa a comunicação com o módulo de oximetria MAX30102.
// Esta função só roda quando chamada.
void inicializaOximetro(){
    display.clearDisplay(); // limpa o buffer da tela
    display.setCursor(0,0); // coloca o cursor de texto no canto superior esquerdo do buffer da tela
    
    // Initializa o sensor MAX30102, usando a porta I2C padrão, velocidade de 400kHz
    if (!meuOximetro.begin(Wire, I2C_SPEED_FAST)) // se não encontrou
    {
        display.print(F("Sensor nao encontrado"));  // coloca mensagem de erro no buffer
        display.display();  // atualiza a tela com as informações do buffer
        while (1); // Se não encontra o sensor, fica parado aqui em um loop infinito
    }
    display.println(F("Coloque o dedo no sensor"));  // coloca mensagem no buffer
    display.display();      // atualiza a tela com as informações do buffer
    delay(1000);            // aguarda 1 segundo
    
    // Configura o sensor MAX30102
    meuOximetro.setup();
    temperatura = meuOximetro.readTemperature(); // mede temperatura inicial
}

/////////////////////////////////////////////////////////////////////
// Encontra os valores máximo e mínimo e a amplitude dos dados do
// oxímetro.
// Esta função só roda quando chamada.
void encontraMaxMinOxim(){
    //encontra máximos e mínimos dos valores lidos
    maximoOxim = -2000000;
    minimoOxim = 2000000;
    for (int idx=0; idx<NUM_PONTOS_OX;idx++){
        if (valorIR_vec[idx] > maximoOxim) maximoOxim = valorIR_vec[idx];
        if (valorIR_vec[idx] < minimoOxim) minimoOxim = valorIR_vec[idx];
    }
    amplitudeOxim = maximoOxim-minimoOxim;
}

/////////////////////////////////////////////////////////////////////
// Verifica se ocorreu uma batida cardíaca procurando por um pico
// no sinal do oxímetro.
// Esta função só roda quando chamada.
void verificaBatidaOximetro(){
    encontraMaxMinOxim(); // encontra amplitude dos dados do oxímetro
    //verifica se ocorreu um pico
    int distancia = PONTOS_PICO_FREQ; // distância entre pontos para verificar um pico
    long variacao = (float)amplitudeOxim/5.0; // variação mínima para considerar pico
    unsigned long tempoAtual = millis();
    // condição 1: ponto candidato acima do ponto à frente
    bool condicao1 = valorIR_vec[NUM_PONTOS_OX-distancia-1]-valorIR_vec[NUM_PONTOS_OX-1]>variacao;
    // condição 2: ponto candidato acima do ponto atrás
    bool condicao2 = valorIR_vec[NUM_PONTOS_OX-distancia-1]-valorIR_vec[NUM_PONTOS_OX-2*distancia-1]>variacao;
    // condição 3: frequencia < 200bpm (tempo entre batidas > 300ms)
    bool condicao3 = tempoAtual-ultimaBatida > 300;
    // condição 4: dedo no sensor
    bool condicao4 = valorIR > 50000;
    if ( condicao1 && condicao2 && condicao3 && condicao4 ){
        contadorBatida = 0; // zera contador para mostrar indicador de batida na tela

        // calcula a frequência em batidas por minuto
        batidasPorMinuto = 60 / ((tempoAtual-ultimaBatida) / 1000.0);
        ultimaBatida = tempoAtual;

        if ( batidasPorMinuto>20 && batidasPorMinuto<200 ){ // se valor é válido
            freqs_vec[posFreq++] = (byte)batidasPorMinuto; // Armazena a leitura no vetor
            posFreq %= NUM_PONTOS_FREQ; // se chegou no final, volta para o início
            
            // Calcula a média das leituras, eliminando maior e menor valores
            freqMedia = 0;
            int freqMax=0;
            int freqMin=999;
            for (byte x = 0 ; x < NUM_PONTOS_FREQ ; x++){
                    if (freqs_vec[x]>freqMax) freqMax = freqs_vec[x];
                    if (freqs_vec[x]<freqMin) freqMin = freqs_vec[x];
                    freqMedia += freqs_vec[x];
            }
            freqMedia = freqMedia - freqMax - freqMin;
            freqMedia /= (NUM_PONTOS_FREQ-2);
        }
    }
}

/////////////////////////////////////////////////////////////////////
// Realiza leitura do módulo oxímetro MAX30102 e dispara a leitura
// do ECG, para as medidas ficarem sincronizadas.
// Esta função só roda quando chamada.
void medeOximetro(){
    disparaLeituraEcg();           // para leituras ficarem sincronizadas...
    valorIR = meuOximetro.getIR(); // mede valor do led IR do MAX30102
    adicionaMedidaIR(valorIR);               // para cálculo do SpO2
    adicionaMedidaRed(meuOximetro.getRed()); // para cálculo do SpO2

    // encontra a média exponencial dos últimos valores lidos
    media = 0.8*media + 0.2*valorIR;

    // 'anda' com os dados do gráfico para a esquerda
    for (int idx=0; idx<NUM_PONTOS_OX-1;idx++) valorIR_vec[idx] = valorIR_vec[idx+1];

    // acrescenta o novo valor no final, subtraindo o valor médio
    valorIR_vec[NUM_PONTOS_OX-1] = valorIR - media;

    verificaBatidaOximetro(); // verifica se ocorreu uma batida cardíaca
}
