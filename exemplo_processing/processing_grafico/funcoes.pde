////////////////////////////////////////////////////////////////////////////////
// Cria os objetos do fundo da janela.
// Esta função só roda quando chamada.
void cria_fundo() {
    // escreve o título grande
    textFont(f, 60);   // fonte utilizada, tamanho da fonte em pixels
    fill(13,98,58);    // cor utilizada: verde logo
    text("EMBARCASAÚDE", 120, 70);  // escreve um texto na posição (120,70) 
    
    // mostra o logo
    PImage img_logo;   // cria um objeto imagem para colocar o logo
    img_logo = loadImage("logo_embarcasaude_100dpi.png"); // carrega arquivo
    img_logo.resize(100, 0); // redimensiona a imagem; usar '0' para manter a proporção dos lados 
    image(img_logo, 10, 10); // coloca a imagem na posição (10,10)
    
    // área do gráfico
    stroke(PRETO);   // cor da borda
    fill(BRANCO);    // cor do interior
    rect(10,200,largura_grafico,altura_grafico);
}

////////////////////////////////////////////////////////////////////////////////
// Desenha um botão com texto dentro.
// Esta função só roda quando chamada.
void desenha_botao(String texto, int x, int y, int lagura, int altura, color cor, color cor_borda){
    fill(cor);         // cor utilizada no interior
    stroke(cor_borda); // cor da borda
    rect(x, y, lagura, altura);
    fill(PRETO);       // cor utilizada
    textFont(f, 12);   // fonte utilizada, tamanho da fonte em pixels
    text(texto, x+5, y+15);
}

////////////////////////////////////////////////////////////////////////////////
// Desenha os botões e portas seriais disponíveis.
// Esta função só roda quando chamada.
void desenha_botoes_serial(){
    if (conectado) {  // se já está conectado...
        desenha_botao("", 410, 105, altura_botao, altura_botao, BRANCO, BRANCO); // 'apaga' botão
        desenha_botao("", 440, 105, altura_botao, altura_botao, BRANCO, BRANCO); // 'apaga' botão
        desenha_botao("Desconectar", 470, 105, 150, altura_botao, cor_botao, cor_borda_botao);
    } else{
        desenha_botao("<", 410, 105, altura_botao, altura_botao, cor_botao, cor_borda_botao);
        desenha_botao(">", 440, 105, altura_botao, altura_botao, cor_botao, cor_borda_botao);
        desenha_botao("Conectar", 470, 105, 150, altura_botao, cor_botao, cor_borda_botao);
    }
    
    atualiza_texto("Portas seriais disponíveis:", 18, 10, 120, 230, BRANCO);
    // caixa onde vai ficar o texto da porta serial
    fill(BRANCO);            // cor utilizada
    stroke(cor_borda_botao); // cor da borda
    rect(250, 105, 150, altura_botao);
    // texto da porta serial
    fill(PRETO);       // cor utilizada
    textFont(f, 12);   // fonte utilizada, tamanho da fonte em pixels
    text(Serial.list()[indice_porta], 255, 120);
    
    atualiza_texto("Status: " + mensagem, 18, mensagem_x, mensagem_y, 600, BRANCO);
}

////////////////////////////////////////////////////////////////////////////////
// Escreve um texto em determinada posição apagando o que tinha antes desenhando
// um retângulo branco em cima.
// Esta função só roda quando chamada.
void atualiza_texto(String texto, int tam_fonte, int x, int y, int largura, int borda){
    // apaga o texto anterior desenhando um retânculo branco por cima:
    fill(BRANCO);            // cor utilizada
    stroke(borda);           // cor da borda (usar outra para ver o que está sendo apagado)
    rect(x, y-int(0.8*tam_fonte), largura, int(1.1*tam_fonte));
    
    // escreve texto:
    textFont(f, tam_fonte);  // fonte utilizada, tamanho da fonte em pixels
    fill(PRETO);             // cor utilizada
    text(texto, x, y);
}

////////////////////////////////////////////////////////////////////////////////
// Verifica se o mouse está dentro de um determinado botão
// Esta função só roda quando chamada.
boolean no_botao(int x, int y, int largura, int altura)  {
  if (mouseX >= x && mouseX <= x+largura && mouseY >= y && mouseY <= y+altura) {
    return true;
  } else {
    return false;
  }
}

////////////////////////////////////////////////////////////////////////////////
// Desenha os botões de controle dos dados.
// Esta função só roda quando chamada.
void desenha_botoes_controle(){
    desenha_botao("inicia",  10, 450, 37, altura_botao, cor_botao, cor_borda_botao);
    desenha_botao("para",    57, 450, 33, altura_botao, cor_botao, cor_borda_botao);
    desenha_botao("f=10Hz", 100, 450, 50, altura_botao, cor_botao, cor_borda_botao);
    desenha_botao("f=20Hz", 160, 450, 50, altura_botao, cor_botao, cor_borda_botao);
    desenha_botao("f=30Hz", 220, 450, 50, altura_botao, cor_botao, cor_borda_botao);
    desenha_botao("f=40Hz", 280, 450, 50, altura_botao, cor_botao, cor_borda_botao);
    desenha_botao("f=50Hz", 340, 450, 50, altura_botao, cor_botao, cor_borda_botao);
    desenha_botao("f=60Hz", 400, 450, 50, altura_botao, cor_botao, cor_borda_botao);
    desenha_botao("f=70Hz", 460, 450, 50, altura_botao, cor_botao, cor_borda_botao);
    desenha_botao("f=80Hz", 520, 450, 50, altura_botao, cor_botao, cor_borda_botao);
    desenha_botao("f=100Hz",580, 450, 50, altura_botao, cor_botao, cor_borda_botao);
}

////////////////////////////////////////////////////////////////////////////////
// Executa uma ação quando o botão do mouse é clicado, dependendo da posição
// do mouse.
// Esta função roda automaticamente quando o botão do mouse é clicado.
void mousePressed() {
    if (conectado) {  // se já está conectado...
        if ( no_botao(10, 450, 37, altura_botao) ) { // botão 'inicia'
            MinhaSerial.write('a');
        }
        else if ( no_botao(57, 450, 33, altura_botao) ) { // botão 'para'
            MinhaSerial.write('b');
        }
        else if ( no_botao(100, 450, 50, altura_botao) ) { // botão '10Hz'
            MinhaSerial.write('1');
        }
        else if ( no_botao(160, 450, 40, altura_botao) ) { // botão '20Hz'
            MinhaSerial.write('2');
        }
        else if ( no_botao(220, 450, 50, altura_botao) ) { // botão '30Hz'
            MinhaSerial.write('3');
        }
        else if ( no_botao(280, 450, 40, altura_botao) ) { // botão '40Hz'
            MinhaSerial.write('4');
        }
        else if ( no_botao(340, 450, 50, altura_botao) ) { // botão '50Hz'
            MinhaSerial.write('5');
        }
        else if ( no_botao(400, 450, 40, altura_botao) ) { // botão '60Hz'
            MinhaSerial.write('6');
        }
        else if ( no_botao(460, 450, 50, altura_botao) ) { // botão '70Hz'
            MinhaSerial.write('7');
        }
        else if ( no_botao(520, 450, 40, altura_botao) ) { // botão '80Hz'
            MinhaSerial.write('8');
        }
        else if ( no_botao(580, 450, 50, altura_botao) ) { // botão '100Hz'
            MinhaSerial.write('9');
        }
        else if ( no_botao(470, 105, 150, altura_botao) ) { // botão 'Conectar'/'Desconectar'
            MinhaSerial.clear();
            MinhaSerial.stop();
            conectado = false;
            mensagem = "desconectado.";
            // 'apaga' botões de controle:
            desenha_botao("",10, 450, 620, altura_botao, BRANCO, BRANCO);
        }
    } else {          // se ainda não está conectado...
        
        if ( no_botao(410, 105, altura_botao, altura_botao) ) { // botão '<'  
            indice_porta = indice_porta - 1;
        }
        else if ( no_botao(440, 105, altura_botao, altura_botao) ) { // botão '>'
            indice_porta = indice_porta + 1;
        }
        else if ( no_botao(470, 105, 150, altura_botao) ) { // botão 'Conectar'/'Desconectar'
            abre_serial();
        }
        // verifica se indice_porta está nos limites das portas disponíveis 
        if (indice_porta < 0) indice_porta = Serial.list().length -1;
        if (indice_porta > (Serial.list().length -1)) indice_porta = 0;
    }
    
    desenha_botoes_serial(); // atualiza o texto com a nova porta serial
}

////////////////////////////////////////////////////////////////////////////////
// Abre uma porta serial para comunicação.
// Esta função só roda quando chamada.
void abre_serial() {
    String nome_porta = Serial.list()[indice_porta];
    // Inicializa a porta serial
    try{ // tenta um comando, mas não encerra o programa em caso de erro...
        conectado = true;
        MinhaSerial = new Serial(this, nome_porta, 115200);
    }catch(Exception e){ // se deu erro...
        mensagem = "ERRO: Não foi possível conectar na porta " + nome_porta + ".";
        conectado = false;
    }
    
    if(conectado){ // se conectou com sucesso...
        mensagem = "conectado na porta " + nome_porta + ".";
        desenha_botoes_controle();
        MinhaSerial.clear(); // limpa qualquer dado anterior na porta
    }
}

////////////////////////////////////////////////////////////////////////////////
// Lê um dado da porta serial.
// Esta função só roda quando chamada.
void le_dado_serial() {
    if (MinhaSerial == null){ // se perdeu a conexão...
        conectado = false;
        mensagem = "conexão perdida.";
        atualiza_texto("Status: " + mensagem, 18, mensagem_x, mensagem_y, 600, 255);
    }
    else if (MinhaSerial.available() > 0){ // Se tiver algum dado disponível
        Buffer = MinhaSerial.readStringUntil(lf); // lê o dado até uma quebra de linha
    }
}

////////////////////////////////////////////////////////////////////////////////
// Atualiza o gráfico com um novo dado do Buffer.
// Esta função só roda quando chamada.
void atualiza_grafico(){
    // limpa dado anterior
    fill(BRANCO);
    stroke(BRANCO);
    rect(posicao+11, 201, 2, altura_grafico-2);
    
    // converte o dado recebido (texto) em um número que cabe no gráfico
    medida = int(float(Buffer)*altura_grafico/1024.0);
    
    // desenha dado atual
    stroke(0,128,0); // cor da linha (R,G,B: verde escuro)
    line(posicao+11, 199+altura_grafico-medida_anterior, posicao+12, 199+altura_grafico-medida);
    posicao = posicao + 1;
    if (posicao >= largura_grafico-3) posicao = 0;
    medida_anterior = medida;
    
    // escreve valor do dado lido na tela:
    atualiza_texto("Medida: " + Buffer.trim(), 18, 10, 180, 120, BRANCO);
}

////////////////////////////////////////////////////////////////////////////////
// Atualiza a taxa de processamento dos dados apresentada na tela.
// Esta função só roda quando chamada.
void atualiza_taxa_processamento(){
    // verifica taxa de atualização dos dados:
    long tempo_atual = millis();
    long intervalo = tempo_atual-tempo_anterior;
    tempo_anterior = tempo_atual;
    float freq = 1.0/(intervalo/1000.0);
    atualiza_texto("Freq: " + nf(freq,4,2) + "Hz", 18, 135, 180, 150, BRANCO);
}
