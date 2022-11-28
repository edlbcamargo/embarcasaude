// Comunicação com o servidor ThingSpeak para envio de dados.
//
// O ThingSpeak é uma plataforma que fornece vários serviços direcionados exclusivamente
// para a construção de análise de dados enviados via WI-FI de dispositivos inteligentes.
// Oferece os recursos de coleta de dados em tempo real, visualizando os dados coletados
// em forma de gráficos utilizando protocolo HTTPS. Nele enviamos as informações por
// canais, e cada canal pode conter até 8 campos de informações ao mesmo tempo. Neste
// tutorial utilizaremos apenas um, mas é uma boa ferramenta caso seja necessário
// monitorar vários sensores de uma vez só.
// A conta gratuíta permite o armazenamento de 100 valores, a um intervalo de no mínimo
// 15 segundos a cada valor. Caso mais valores sejam enviados, os últimos 100 são
// armazenados.
//
// Referências:
// Código adaptado do exemplo da biblioteca "ThingSpeak", que pode ser encontrada na aba
// Exemplos da IDE Arduino --> ThingSpeak --> ESP 32 --> WriteSingleField.
//
// Adaptado por: Lucas Jácome Cabral, 2022

#include <WiFi.h>
#include "ThingSpeak.h" 

// Definindo as configurações da rede. Trocar pelas informações da sua rede:
const char* ssid = "NOME_DA_REDE_WIFI";    // nome da sua rede WiFi
const char* senha = "SENHA_DA_REDE_WIFI";  // senha da sua rede WiFi

// Definindo as configurações do seu canal no ThingSpeak. Troque pelas informações do seu canal:
unsigned long myChannelID = 0000000;   // Aqui vai a ID do seu canal (Channel ID)
const char* myWriteAPIKey = "xxxxxxx"; // Aqui vai a chave de escrita do seu canal (Write API Key)

// Outras variáveis
long distancia;    // variável que irá guardar a distância do sensor
int pinoTrig = 27; // pino usado para disparar os pulsos do sensor
int pinoEcho = 26; // pino usado para ler a saída do sensor

WiFiClient  client; // Iniciando um cliente HTTP

///////////////////////////////////////////////
// Esta função só roda quando é chamada dentro de outra função.
// Esta função devolve a distância lida no sensor de ultrassom, em centímetros.
void leitura() {
  digitalWrite(pinoTrig, HIGH); // Manda um sinal de nível alto por 10 microssegundos
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);
  distancia = pulseIn(pinoEcho, HIGH); // Le o comprimento de onda em alta
  distancia = distancia / 2; // Divide por 2 pois esse e o tempo da onda ir e voltar
  distancia = distancia * 0.034029; // calcula a distância usando a velocidade do som
}

///////////////////////////////////////////////
// Esta função só roda uma vez, no início
void setup() {
  pinMode(pinoTrig, OUTPUT); // configura pino TRIG como saída
  pinMode(pinoEcho, INPUT); // configura pino ECHO como entrada
  Serial.begin(115200);  //Initialize serial
  WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

///////////////////////////////////////////////
// Esta função se repete indefinidamente
void loop() {
  leitura(); // faz a leitura do sensor de ultrassom e guarda na variável 'distancia'

  // Verifica se está conectado na refe Wifi, e conecta se não estiver...
  if(WiFi.status() != WL_CONNECTED){ // se não está conectado...
    Serial.print("Conectando na rede ");
    Serial.println(ssid);
    while(WiFi.status() != WL_CONNECTED){ // enquanto não conectar...
      WiFi.begin(ssid, senha); 
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nWiFi conectado.");
  }

  // Tenta escrever a distância lida no canal do ThingSpeak
  int x = ThingSpeak.writeField(myChannelID, 1,distancia, myWriteAPIKey);
  if(x == 200){ // se conseguiu enviar o dado...
    Serial.println("Canal atualizado!");
  }
  else{
    Serial.println("Problema atualizando o canal. Código de erro HTTP: " + String(x));
  }
  delay(15000); // o intervalo mínimo é de um envio a cada 15 segundos na conta gratuíta
}