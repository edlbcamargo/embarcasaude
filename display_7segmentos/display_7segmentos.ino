// Contador com botão e display 7 segmentos, utilizando funções
//
// Criado por: Erick León, 2022
// Baseado no código disponível em:
// https://github.com/edlbcamargo/embarcasaude/blob/main/display_7segmentos_longo/display_7segmentos_longo.ino

int a = 13;    // pino correspondente ao LED a
int b = 12;    // pino correspondente ao LED b
int c = 11;    // pino correspondente ao LED c
int d = 10;    // pino correspondente ao LED d
int e = 9;     // pino correspondente ao LED e
int f = 8;     // pino correspondente ao LED f
int g = 7;     // pino correspondente ao LED g
int botao = 2; // pino correspondente ao botão 
int tempo_ms = 500; // tempo de espera, em milissegundos

// este código só roda quando é chamado em outra função
void mostra_numero(int va, int vb, int vc, int vd, int ve, int vf, int vg) {
        digitalWrite(a, va);
        digitalWrite(b, vb);
        digitalWrite(c, vc);
        digitalWrite(d, vd);
        digitalWrite(e, ve);
        digitalWrite(f, vf);
        digitalWrite(g, vg);
        delay(tempo_ms);    
}

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
        mostra_numero(1, 1, 1, 1, 1, 1, 0); // zero
        mostra_numero(0, 1, 1, 0, 0, 0, 0); // um
        mostra_numero(1, 1, 0, 1, 1, 0, 1); // dois
        mostra_numero(1, 1, 1, 1, 0, 0, 1); // tres
        mostra_numero(0, 1, 1, 0, 0, 1, 1); // quatro
        mostra_numero(1, 0, 1, 1, 0, 1, 1); // cinco
        mostra_numero(1, 0, 1, 1, 1, 1, 1); // seis
        mostra_numero(1, 1, 1, 0, 0, 0, 0); // sete
        mostra_numero(1, 1, 1, 1, 1, 1, 1); // oito
        mostra_numero(1, 1, 1, 1, 0, 1, 1); // nove
    }
}
