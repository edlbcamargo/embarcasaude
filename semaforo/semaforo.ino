// Programa de controle de um semáforo.
//
// Criado por: Lucas Jácome Cabral, 2022

int verde = 3;        // pino onde está ligado o LED verde
int vermelho = 4;     // pino onde está ligado o LED vermelho
int amarelo = 5;      // pino onde está ligado o LED amarelo

void setup() {  // este código só é rodado uma vez, no início
    pinMode(verde, OUTPUT);       // define verde como saída
    pinMode(vermelho, OUTPUT);    // define vermelho como saída
    pinMode(amarelo, OUTPUT);     // define amarelo como saída
}

void loop() {  // este código se repete indefinidamente
    digitalWrite(vermelho, HIGH); // ativa LED vermelho
    digitalWrite(verde,LOW);      // mantém LED verde desligado
    digitalWrite(amarelo,LOW);    // mantém LED amarelo desligado
    delay(4000);                  // tempo de quatro segundos
    digitalWrite(vermelho, LOW);  // mantém LED vermelho desligado
    digitalWrite(verde,LOW);      // mantém LED verde desligado
    digitalWrite(amarelo, HIGH);  // ativa LED amarelo
    delay(1000);                  // tempo de um segundo
    digitalWrite(vermelho, LOW);  // mantém LED vermelho desligado
    digitalWrite(verde,HIGH);     // ativa LED verde
    digitalWrite(amarelo, LOW);   // mantém LED amarelo desligado
    delay(4000);                  // tempo de quatro segundos
}
