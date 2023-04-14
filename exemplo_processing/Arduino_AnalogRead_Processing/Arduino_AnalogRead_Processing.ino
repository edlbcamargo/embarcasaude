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

// Frequencia de amostragem desejada, em Hz. Usar valores entre 1 e 2000:
long frequencia = 10;

unsigned long atraso_us;

int medida;
unsigned long tempo_atual, tempo_anterior;
byte comando;

bool coleta = false;

////////////////////////////////////////////////////////////////////////////////
// Esta função só roda quando chamada
void verifica_comandos() {
  if(Serial.available()){
    comando = Serial.read();

    switch (comando) {
      case 'a':            // inicia coleta
        coleta = true;
        break;

      case 'b':            // interrompe coleta
        coleta = false;
        break;

      case '1':            // altera frequencia
        frequencia = 10;
        break;

      case '2':            // altera frequencia
        frequencia = 20;
        break;

      case '3':            // altera frequencia
        frequencia = 30;
        break;

      case '4':            // altera frequencia
        frequencia = 40;
        break;

      case '5':            // altera frequencia
        frequencia = 50;
        break;

      case '6':            // altera frequencia
        frequencia = 60;
        break;

      case '7':            // altera frequencia
        frequencia = 70;
        break;

      case '8':            // altera frequencia
        frequencia = 80;
        break;

      case '9':            // altera frequencia
        frequencia = 100;
        break;

      default: // comando não reconhecido
        break;
    }
    atraso_us = (1000000.0/frequencia);
  }
}

////////////////////////////////////////////////////////////////////////////////
// Esta função só roda uma vez, no início
void setup() {
  Serial.begin(115200);
  atraso_us = (1000000.0/frequencia);
  tempo_anterior = micros();
}

////////////////////////////////////////////////////////////////////////////////
// Esta função se repete indefinidamente
void loop() {

  if (coleta) {
    tempo_atual = micros();
    
    // o abs() evita problemas com overflow da função micros()
    if (abs(tempo_atual - tempo_anterior) > atraso_us) {
      tempo_anterior = tempo_atual;
      medida = analogRead(A0);
      Serial.println(medida);
    }
  }

  verifica_comandos();
}
