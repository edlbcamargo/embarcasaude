 /*******************************************************************************
  * 
  * Exemplo de uso de um Servidor Web usando ESP32. O LED1 está ligado ao pino 25
  * e seu brilho é controlado por PWM (0%, 33%, 67% e 100%). O LED2 está ligado
  * ao pino 26 e seu brilho é controlador como uma porta digital padrão (ligado
  * ou desligado).
  * 
  * Criado por: Erick León, 2022
  * 
  ********************************************************************************/

// Carregando biblioteca para uso do Wi-Fi
#include <WiFi.h>

// Definindo as configurações da rede. Trocar pelas informações da sua rede:
const char* ssid = "NOME_DA_REDE_WIFI";       // nome da sua rede WiFi
const char* senha = "SENHA_DA_REDE_WIFI";  // senha da sua rede WiFi
const char* hostname = "embarcasaude";        // nome como o módulo ESP32 vai aparecer na sua rede

// Definindo outras variáveis:
const int led1 = 25; // Pino onde está ligado o LED1 (o pino 25 também funciona como DAC1)
const int led2 = 26; // Pino onde está ligado o LED2 (o pino 26 também funciona como DAC2)

// Propriedades do PWM do LED1
const int freq = 5000;   // frequência: 5000Hz
const int canalPWM = 0;  // Canal do PWM usado (o SP32 tem 16 PWMs: canais de 0 a 15)
const int resolucao = 2; // Resolução em bits: 2 = 4 valores de duty cycle (00, 01, 10, 11)
int led1_dc = 0;         // Guarda o duty cycle do PWM do LED1

// Limite de tempo que aguarda por dados para cada conexão (em milissegundos).
// Cada carregamento de página ou clique em um botão conta como uma conexão.
// 2000 milissegundos equivale a 2 segundos.
const long tempoLimite = 2000;
unsigned long tempoAtual, tempoInicial; // variáveis para controle do tempo de conexão

// Um servidor web usa, por padrão, a porta 80.
// Inicializando o servidor web:
WiFiServer server(80);

// esta função só roda uma vez, no início
void setup() {
  // Inicializando a comunicação serial para debug via Serial Monitor
  Serial.begin(115200);
  Serial.println("Iniciando configuração...");
  
  // Inicializando os pinos dos LEDs
  ledcSetup(canalPWM, freq, resolucao); // inicializa o PWM
  ledcAttachPin(led1, canalPWM);        // liga o LED1 ao PWM
  ledcWrite(canalPWM, led1_dc);         // desligando LED1 (duty cycle inicial de 0)
  pinMode(led2, OUTPUT);                // definindo pino do LED2 como saída
  digitalWrite(led2, LOW);              // desligando LED2

  // Conectando na rede WiFi
  Serial.print("Conectando na rede ");
  Serial.println(ssid);
  WiFi.setHostname(hostname); // define o nome do módulo ESP32 na rede (hostname)
  WiFi.begin(ssid, senha);
  while (WiFi.status() != WL_CONNECTED) { // Mostrando uma 'barra de progressão' enquanto tenta conectar
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.print("WiFi conectado no endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
  Serial.println("Tente acessar na mesma rede local pelos seguintes endereços:");
  Serial.print("http://");
  Serial.println(WiFi.localIP());
  Serial.print("http://");
  Serial.println(hostname);
  Serial.print("http://");
  Serial.print(hostname);
  Serial.println(".local");
  Serial.print("http://");
  Serial.print(hostname);
  Serial.println(".home");
  Serial.println("");
  server.begin();
}

// esta função se repete indefinidamente
void loop(){
  WiFiClient client = server.available();   // Verifica se existe alguém acessando o servidor web

  if (client) {                             // Se alguém acessar...
    tempoInicial = millis();                // marca tempo inicial da conexão
    tempoAtual = tempoInicial;
    Serial.println("Detectado novo acesso.");
    String comando; // string onde será guardado o comando que chega via HTTP
    bool finalDaMensagem = false;
    // Loop enquanto o acesso estiver em andamento, ou enquanto não exceder o tempo limite
    while (client.connected() && tempoAtual - tempoInicial <= tempoLimite) {  
      tempoAtual = millis();                // atualiza tempo atual
      if (client.available()) {             // se tem dados para ler
        char c = client.read();             // lê um byte
        Serial.write(c);                    // imprime o byte lido na serial (para debug)
        comando += c;

        if (c == '\n') {                    // se chegou uma nova linha (\n)
          if (finalDaMensagem) {  // só é fim de mensagem no segundo \n...
            client.println("HTTP/1.1 200 OK");        // envia um código de resposta padrão
            client.println("Content-type:text/html"); // avisa que o conteúdo a seguir é uma página html
            client.println("Connection: close");
            client.println();
            
            // liga ou desliga os LEDs, dependendo do comando recebido
            if (comando.indexOf("GET /led1/on1") >= 0) {
              Serial.println("Ligando LED1 em dc=1");
              led1_dc=1;
              ledcWrite(canalPWM, led1_dc); // LED1 está ligado ao PWM
            } else if (comando.indexOf("GET /led1/on2") >= 0) {
              Serial.println("Ligando LED1 em dc=2");
              led1_dc=2;
              ledcWrite(canalPWM, led1_dc); // LED1 está ligado ao PWM
            } else if (comando.indexOf("GET /led1/on3") >= 0) {
              Serial.println("Ligando LED1 em dc=3");
              led1_dc=3;
              ledcWrite(canalPWM, led1_dc); // LED1 está ligado ao PWM
            } else if (comando.indexOf("GET /led1/off") >= 0) {
              Serial.println("Desligando LED1");
              led1_dc=0;
              ledcWrite(canalPWM, led1_dc); // LED1 está ligado ao PWM
            } else if (comando.indexOf("GET /led2/on") >= 0) {
              Serial.println("Ligando LED2");
              digitalWrite(led2, HIGH); // LED2 está configurado como um OUTPUT digital
            } else if (comando.indexOf("GET /led2/off") >= 0) {
              Serial.println("Desligando LED2");
              digitalWrite(led2, LOW); // LED2 está configurado como um OUTPUT digital
            }
            
            // Envia o código HTML da página

            // Cabeçalho da página
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"); // para melhorar a visualização da página
            client.println("<meta charset=\"UTF-8\">"); // para mostrar os acentos corretamente
            client.println("<title>Embarca Saúde: Servidor Web na ESP32</title>");

            // configuração do estilo da página:
            client.println("<style>html { text-align: center;}");
            // configuração do estilo dos botões:
            client.println(".botao { background-color: #444444; border: 2px solid black; color: white; padding: 16px 40px;");
            client.println("border-radius: 50%; text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".botao_ligado {background-color: #FF0000;}");  // alteração para botão ligado 100%
            client.println(".botao_ligado1 {background-color: #400000;}"); // alteração para botão ligado 30%
            client.println(".botao_ligado2 {background-color: #900000;}"); // alteração para botão ligado 67%
            client.println("</style></head>");

            // Conteúdo (corpo) da página
            client.println("<body><h1>Servidor Web na ESP32</h1>");
            
            if ( led1_dc == 0 ) {             // se o LED1 está desligado (dc=0)...
              client.println("<p><a href=\"/led1/on1\"><botao class=\"botao\">Ligar LED1 (0%->33%)</botao></a></p>");
            } else if ( led1_dc == 1 ) {      // senão, se dc=1
              client.println("<p><a href=\"/led1/on2\"><botao class=\"botao botao_ligado1\">Ligar LED1 (33%->67%)</botao></a></p>");
            } else if ( led1_dc == 2 ) {      // senão, se dc=2
              client.println("<p><a href=\"/led1/on3\"><botao class=\"botao botao_ligado2\">Ligar LED1 (67%->100%)</botao></a></p>");
            } else {                          // senão (dc=3)...
              client.println("<p><a href=\"/led1/off\"><botao class=\"botao botao_ligado\">Desligar LED1</botao></a></p>");
            }
            
            client.println("<br>");

            if ( digitalRead(led2) == LOW ) { // se o LED2 está desligado...
              client.println("<p><a href=\"/led2/on\"><botao class=\"botao\">Ligar LED2</botao></a></p>");
            } else {                          // senão (se o LED2 está ligado)...
              client.println("<p><a href=\"/led2/off\"><botao class=\"botao botao_ligado\">Desligar LED2</botao></a></p>");
            }
            client.println("</body></html>"); // Fim da página HTML
            
            client.println(); // termina o envio HTTP com uma linha em branco
            break; // sai do loop while (fim da conexão)
            
          } else { // se chegou uma nova linha (\n) e ainda não tinha terminado a mensagem...
            finalDaMensagem = true; // termina a mensagem no segundo \n (linha em branco)
          }
        } else if (c != '\r') {  // se não chegou uma nova linha (\n ou \r)...
          finalDaMensagem = false; // mensagem em andamento
        }
      }
    }

    client.stop(); // termina a conexão
    Serial.println("Conexão terminada.");
    Serial.println("");
  }
}