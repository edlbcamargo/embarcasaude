// Exemplo simples de comunicação entre o Arduino e o Processing.
// O Arduino mede dados do ADC usando a função analogRead(), realizando
// as medições em uma taxa de amostragem conhecida e enviando
// continuamente os valores medidos pela porta serial. O Processing
// realiza a leitura dos dados da porta serial e apresenta os mesmos
// em um gráfico.
//
// A frequência de aquisição do Arduino pode ser alterada através de
// comandos enviados pelo Processing através da porta serial.
//
// Criado por: Erick Dario León Bueno de Camargo, 2023

import processing.serial.*;   // importa a biblioteca serial.
Serial MinhaSerial;           // cria um objeto da classe serial
String Buffer;                // variável onde ficarão os dados lidos
int posicao = 0;              // guarda posição atual do gráfico
int medida;                   // guarda medida atual
int medida_anterior = 0;      // guarda medida anterior

////////////////////////////////////////////////////////////////////////////////
// Envia uma tecla pressionada pela serial.
// Esta função roda automaticamente quando uma tecla é pressionada
void keyPressed() {
    MinhaSerial.write(key);
}

////////////////////////////////////////////////////////////////////////////////
// Esta função só roda uma vez, no início
void setup() {
    size(640, 480); // indica o tamanho da janela do programa
    background(255);  // limpa a tela na cor de fundo (255 = branco, 0 = preto)
    
    println("Portas seriais disponiveis:"); // lista portas disponíveis no console
    println(Serial.list());
    MinhaSerial = new Serial(this, "/dev/ttyUSB0", 115200); // alterar para a sua porta
    MinhaSerial.clear(); // limpa qualquer dado anterior na porta
}

////////////////////////////////////////////////////////////////////////////////
// Esta função se repete indefinidamente
void draw(){
    Buffer = null;
    if (MinhaSerial.available() > 0){ // se tem dados na porta serial
        Buffer = MinhaSerial.readStringUntil(10); // lê os dados até o final da linha (código ASCII 10)
        if (Buffer != null) {        // buffer ok
            // limpa dado anterior desenhando um retângulo branco em cima
            fill(255);
            stroke(255);
            rect(posicao, 0, 2, 480);
            
            // converte o dado recebido (texto) em um número que cabe no gráfico
            medida = int(float(Buffer)*480/1023.0);
            
            // desenha dado atual (y=0 fica no topo da tela)
            stroke(255,0,0); // cor da linha (R,G,B: vermelho)
            line(posicao, 479-medida_anterior, posicao+1, 479-medida);
            posicao = posicao + 1;
            if (posicao >= 638) posicao = 0; // se chegou no final da tela
            medida_anterior = medida;
        }
    }
    if (MinhaSerial.available() > 10) println(MinhaSerial.available());
}
