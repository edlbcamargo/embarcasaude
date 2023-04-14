// Exemplo de comunicação entre o Arduino e o Processing.
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

import processing.serial.*;    // importa a biblioteca serial.

// variáveis usadas na interface gráfica:
PFont f;                       // declaração da variável da fonte que será usada
int altura_botao = 20;
color cor_botao = color(200,200,255);    // RGB: azul claro
color cor_borda_botao = color(0,0,100);  // RGB: azul escuro
int BRANCO = 255;
int PRETO = 0;
int altura_grafico = 240;
int largura_grafico = 620;

// variáveis usadas na comunicação serial:
Serial MinhaSerial;            // cria um objeto da classe serial
int indice_porta = 0;          // posição da porta que será utilizada
String Buffer;                 // variável onde ficarão os dados lidos
boolean conectado = false;     // variável que indica se a porta serial está conectada
int lf = 10;                   // Código de nova linha (Linefeed) em ASCII
String mensagem = "desconectado."; // variável que vai guardar o status da porta serial
int mensagem_x = 10;           // posição x da mensagem
int mensagem_y = 145;          // posição y da mensagem

// variáveis usadas na manipulação dos dados
int medida;
int medida_anterior = 0;
int posicao = 0;
long tempo_anterior;

////////////////////////////////////////////////////////////////////////////////
// Esta função só roda uma vez, no início
void setup() {
    size(640, 480);   // indica o tamanho da janela do programa
    background(BRANCO);  // cor de fundo da tela
    f = createFont("Arial", 10); // cria uma fonte para ser usada nos textos
    
    cria_fundo();
    desenha_botoes_serial();
}

////////////////////////////////////////////////////////////////////////////////
// Esta função se repete indefinidamente
void draw(){
    if (conectado) {
        Buffer = null;    // limpa o dado anterior
        le_dado_serial(); // lê um dado da porta serial, se disponível
        atualiza_texto("dados acumulados na serial: " + MinhaSerial.available(), 18, 320, 180, 310, BRANCO);
        
        if (Buffer != null) {     // se teve um novo dado...
            atualiza_taxa_processamento();
            atualiza_grafico();
        }
    }
    else {
        atualiza_texto("", 18, 10, 180, 620, BRANCO); // 'apaga' textos
    }
}
