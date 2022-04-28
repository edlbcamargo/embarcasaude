// Contador com botão e display 7 segmentos
//
// Criado por: Lucas Jácome Cabral, 2022

int a = 13;    // pino correspondente ao LED a
int b = 12;    // pino correspondente ao LED b
int c = 11;    // pino correspondente ao LED c
int d = 10;    // pino correspondente ao LED d
int e = 9;     // pino correspondente ao LED e
int f = 8;     // pino correspondente ao LED f
int g = 7;     // pino correspondente ao LED g
int botao = 2; // pino correspondente ao botão 
int tempo_ms = 500; // tempo de espera, em milissegundos

// este código só é rodado uma vez, no início
void setup () {
    pinMode(a, OUTPUT);           // Define o pino 'a' como saída
    pinMode(b, OUTPUT);           // Define o pino 'b' como saída
    pinMode(c, OUTPUT);           // Define o pino 'c' como saída
    pinMode(d, OUTPUT);           // Define o pino 'd' como saída
    pinMode(e, OUTPUT);           // Define o pino 'e' como saída
    pinMode(f, OUTPUT);           // Define o pino 'f' como saída
    pinMode(g, OUTPUT);           // Define o pino 'g' como saída
    pinMode(botao, INPUT_PULLUP); // Define o pino 'botao' como entrada
}

// este código se repete indefinidamente
void loop () {
    if (digitalRead(botao) == LOW) {
        digitalWrite(a, 1); //zero
        digitalWrite(b, 1);
        digitalWrite(c, 1);
        digitalWrite(d, 1);
        digitalWrite(e, 1);
        digitalWrite(f, 1);
        digitalWrite(g, 0);
        delay(tempo_ms);    
        digitalWrite(a, 0); //um
        digitalWrite(b, 1);
        digitalWrite(c, 1);
        digitalWrite(d, 0);
        digitalWrite(e, 0);
        digitalWrite(f, 0);
        digitalWrite(g, 0);
        delay(tempo_ms);
        digitalWrite(a, 1); //dois
        digitalWrite(b, 1);
        digitalWrite(c, 0);
        digitalWrite(d, 1);
        digitalWrite(e, 1);
        digitalWrite(f, 0);
        digitalWrite(g, 1);
        delay(tempo_ms);
        digitalWrite(a, 1); //três
        digitalWrite(b, 1);
        digitalWrite(c, 1);
        digitalWrite(d, 1);
        digitalWrite(e, 0);
        digitalWrite(f, 0);
        digitalWrite(g, 1);
        delay(tempo_ms);
        digitalWrite(a, 0); //quatro
        digitalWrite(b, 1);
        digitalWrite(c, 1);
        digitalWrite(d, 0);
        digitalWrite(e, 0);
        digitalWrite(f, 1);
        digitalWrite(g, 1);
        delay(tempo_ms);
        digitalWrite(a, 1); //cinco
        digitalWrite(b, 0);
        digitalWrite(c, 1);
        digitalWrite(d, 1);
        digitalWrite(e, 0);
        digitalWrite(f, 1);
        digitalWrite(g, 1);
        delay(tempo_ms);
        digitalWrite(a, 1); //seis
        digitalWrite(b, 0);
        digitalWrite(c, 1);
        digitalWrite(d, 1);
        digitalWrite(e, 1);
        digitalWrite(f, 1);
        digitalWrite(g, 1);
        delay(tempo_ms);
        digitalWrite(a, 1); // sete
        digitalWrite(b, 1);
        digitalWrite(c, 1);
        digitalWrite(d, 0);
        digitalWrite(e, 0);
        digitalWrite(f, 0);
        digitalWrite(g, 0);
        delay(tempo_ms);
        digitalWrite(a, 1); //oito
        digitalWrite(b, 1);
        digitalWrite(c, 1);
        digitalWrite(d, 1);
        digitalWrite(e, 1);
        digitalWrite(f, 1);
        digitalWrite(g, 1);
        delay(tempo_ms);
        digitalWrite(a, 1); //nove
        digitalWrite(b, 1);
        digitalWrite(c, 1);
        digitalWrite(d, 1);
        digitalWrite(e, 0);
        digitalWrite(f, 1);
        digitalWrite(g, 1);
        delay(tempo_ms);
    }
}
