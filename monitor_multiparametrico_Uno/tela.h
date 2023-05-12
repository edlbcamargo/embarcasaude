/*******************************************************************************
 * Funções usadas com o display OLED com controlador SSD1306, usado para 
 * gráficos e apresentação de informações diversas.
 * 
 * Criado por: Erick León, 2023
 ********************************************************************************/

#include <Adafruit_GFX.h>          // construção de gráficos


/////////////////////////////////////////////////////////////////////
// Iniciliza o display OLED.
// Esta função só roda quando chamada.
void inicializaDisplay(){
    // inicializa a comunicação com o display, no endereço I2C 0x3C
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        while (1);   // Se não conseguiu inicializar o display, fica aqui parado
    }
    display.setTextSize(1);        // escolhe o tamanho da letra
    display.setTextColor(WHITE);   // escolhe a cor da letra
}

/////////////////////////////////////////////////////////////////////
// Desenha uma curva (gráfico) no buffer do display OLED,
// acrescentando um retângulo ao redor da curva.
// Esta função só roda quando chamada.
void desenhaCurva(int posicao_inferior, int altura, int n_pontos, int *vec){
    long maximo = -99999;
    long minimo = 99999;
    long amplitude;
    for (int idx=0; idx<n_pontos-1;idx++){ // encontra maximo e minimo
        if (vec[idx] > maximo) maximo = vec[idx];
        if (vec[idx] < minimo) minimo = vec[idx];
    }
    amplitude = maximo - minimo;
    if (amplitude==0) amplitude = 1;
    for (int idx=0; idx<n_pontos-1;idx++){ // desenha curva
        // normaliza de 0 a 1
        float valorNorm = ((float)vec[idx+1]-(float)minimo)/((float)amplitude);
        float valorNorm_anterior = ((float)vec[idx]-(float)minimo)/((float)amplitude);
        // encontra posição no display
        int ponto = (float)posicao_inferior-(float)(altura-1)*valorNorm;
        int ponto_anterior = (float)posicao_inferior-(float)(altura-1)*valorNorm_anterior;
        display.drawLine(idx+1, ponto_anterior, idx+2, ponto, SSD1306_WHITE);
    }

    // desenha retângulo em volta do gráfico
    display.drawRect(0, posicao_inferior-altura-1, n_pontos+2, altura+2, SSD1306_WHITE);
}

/////////////////////////////////////////////////////////////////////
// Desenha todos os elementos da tela.
// Esta função só roda quando chamada.
void desenhaTela(){
    byte posXTexto = 70; // posição X do texto na tela
    byte altura_linha = 11; // latura da linha de texto, em pixels
    // soma 1 nos contadores
    contadorTemperatura++;
    contadorBatida++;
    
    display.clearDisplay();   // limpa o buffer da tela
    
    // Dados do oxímetro
    display.setCursor(0,0);   // posiciona cursor de texto
    display.print(F("Oximetro:")); // mostra mensagem
    display.setCursor(posXTexto,altura_linha*1); // posiciona cursor de texto
    display.print(freqMedia); // mostra no buffer da tela frequência cardíaca medida
    display.print(F(" bpm")); // mostra no buffer da tela a unidade de tempo do intervalo
    if (valorIR < 50000){
        display.setCursor(posXTexto,altura_linha*4); // posiciona cursor de texto
        display.print(F("sem dedo?")); // mostra mensagem
        spo2 = 0;
    }
    
    desenhaCurva(30,20,NUM_PONTOS_OX,valorIR_vec); // desenha curva do oxímetro no buffer

    // se ocorreu uma batida a menos de 3 atualizações, mostra círculo
    if (contadorBatida < 3) display.fillCircle(122, 5, 5, SSD1306_WHITE);

    //a cada 20 atualizações (aprox. 2 segundos), mede temperatura e SpO2
    if ( contadorTemperatura > 20 ) {
        temperatura = meuOximetro.readTemperature();
        contadorTemperatura = 0;
        int16_t spo2_temp = medeSpO2();
        if (spo2_temp>0) spo2 = spo2_temp;
    }

    display.setCursor(posXTexto,altura_linha*2); // posiciona cursor de texto
    display.print(F("SpO2:")); // mostra mensagem no buffer
    display.print(spo2);      // mostra no buffer da tela SpO2
    
    display.setCursor(posXTexto,altura_linha*3); // posiciona cursor de texto
    display.print(temperatura); // mostra no buffer da tela frequência cardíaca medida
    display.cp437(true);      // usa código de caracteres 'Code page 437' para símbolo de grau
    display.print((char)248); // símbolo de grau
    display.print(F("C"));    // mostra no buffer da tela a unidade da temperatura
    
    // Dados do ECG
    display.setCursor(0,33);  // posiciona cursor de texto
    display.print(F("ECG:")); // mostra mensagem

    desenhaCurva(62,20,NUM_PONTOS_ECG,ecg_vec); // desenha curva do ECG no buffer

    // mostra frequência de aquisição na tela:
    display.setCursor(posXTexto,0); // posiciona cursor de texto
    if (freqAquisicao<10.0) display.print(F(" ")); // para alinhar valores
    display.print(freqAquisicao);
    display.print(F("Hz"));

    // atualiza valor da tensão da bateria:
    display.setCursor(posXTexto,altura_linha*5); // posiciona cursor de texto
    display.print(F("Vin:"));
    disparaLeituraBateria();
    display.print(tensao_bateria*5.0*2.0/1024.0);
    display.print(F("V"));
    
    display.display();        // atualiza a tela com as informações do buffer
 
}
