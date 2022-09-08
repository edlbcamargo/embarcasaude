// Teste com leitor/gravador de SD Card.
//
// Neste código, vamos analisar a distância do sensor ultrassônico
// sem o uso de bibliotecas, apenas medindo o tempo de resposta
// entre um pulso enviado e o reflexo recebido e multiplicando pela
// velocidade do som. O código salva a distância lida pelo sensor e
// o horário lido pelo módulo RTC no cartão micro SD e mostra todas
// as informações salvas no Monitor Serial.
//
// Criado por: Lucas Jácome Cabral, 2022

///////////////////////////////////////////////
// Bibliotecas utilizadas
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>

///////////////////////////////////////////////
// Criação de objetos para uso das bibliotecas
RTC_DS3231 rtc;
File meuArquivo;

///////////////////////////////////////////////
// Variáveis utilizadas ao longo do código.
int ciclos = 30;
long cont  = 0;
long distancia;
int pinoTrig = 3; // pino usado para disparar os pulsos do sensor.
int pinoEcho = 2; // pino usado para ler a saída do sensor.

///////////////////////////////////////////////
// Esta função só roda quando é chamada dentro de outra função
void leitura(){
    digitalWrite(pinoTrig, HIGH); // Manda um sinal de nível alto por 10 microssegundos
    delayMicroseconds(10);
    digitalWrite(pinoTrig, LOW);
    
    distancia = pulseIn(pinoEcho, HIGH); // Le o comprimento de onda em alta
}

///////////////////////////////////////////////
// Esta função só roda uma vez, no início
void setup () {
    Serial.begin(9600);
    
    pinMode(pinoTrig, OUTPUT); // configura pino TRIG como saída
    pinMode(pinoEcho, INPUT); // configura pino ECHO como entrada
    
    ///////////////////////////////////////////////
    // Inicializa o SD Card
    if (SD.begin(4)) {
        Serial.println("SD Card pronto para uso."); // imprime na tela
    }
    else {
        Serial.println("Falha na inicialização do SD Card.");
        return;
    }
    meuArquivo = SD.open("teste.txt", FILE_WRITE);
    
    ///////////////////////////////////////////////
    // Laço para identificar qualquer problema com o módulo RTC
    if (! rtc.begin()) {
        // caso  o modulo nao seja encontrado, irá aparecer um aviso no monitor serial
        Serial.println("Módulo RTC não encontrado");
        Serial.flush();
        while (1) delay(10);
    }
    ///////////////////////////////////////////////
    // Comando para alterar o horario do modulo RTC
    // Ajusta com data e hora em que o programa foi compilado
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

///////////////////////////////////////////////
// Esta função se repete indefinidamente
void loop () {
    ///////////////////////////////////////////////   
    // Leitura da distancia e cálculos. 
    leitura();
    // Divide por 2 pois esse eh o tempo da onda ir e voltar
    distancia = distancia/2;
    distancia = distancia*0.034029;
    
    ///////////////////////////////////////////////
    //Seção para envio de informações para o monitor serial
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print("-");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print(" ***** ");
    Serial.print("Leitura da distancia: ");
    Serial.println(distancia);
    
    // Seção para envio de informações para o módulo Micro SD
    meuArquivo.print(now.year(), DEC);
    meuArquivo.print('/');
    meuArquivo.print(now.month(), DEC);
    meuArquivo.print('/');
    meuArquivo.print(now.day(), DEC);
    meuArquivo.print('-');
    meuArquivo.print(now.hour(), DEC);
    meuArquivo.print(':');
    meuArquivo.print(now.minute(), DEC);
    meuArquivo.print(':');
    meuArquivo.print(now.second(), DEC);
    meuArquivo.print(" ***** ");
    meuArquivo.print("Leitura da distancia: ");
    meuArquivo.println(distancia);

    delay(1000); // tempo entre as medidas
    cont +=1; // contagem de ciclos
    
    ///////////////////////////////////////////////
    //Laço para fechamento das medidas (quantas vezes queremos medir)
    if (cont >= ciclos)
    {
        // Interrompe o processo e fecha o arquivo
        meuArquivo.close();
        Serial.println("Processo de gravacao interrompido. Retire o SD!");
        while (1) {}
    }
}
