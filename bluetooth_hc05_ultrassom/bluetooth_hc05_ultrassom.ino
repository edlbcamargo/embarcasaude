// Comunicação com o módulo Bluetooth HC05
//
// Programa de comunicação com o módulo Bluetooth HC05,
// lê a distância de um objeto medida por um sensor
// ultrassônico e envia pelo módulo HC05 para um celular.
//
// Criado por: Lucas Jácome Cabral, 2022

long distancia;
int pinoTrig = 3; // pino usado para disparar os pulsos do sensor
int pinoEcho = 2; // pino usado para ler a saída do sensor

///////////////////////////////////////////////
// Esta função só roda quando é chamada dentro de outra função
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
  pinMode(pinoEcho, INPUT);  // configura pino ECHO como entrada
  Serial.begin(38400); // Velocidade da comunicação com o módulo HC05 (ou monitor serial)
}

///////////////////////////////////////////////
// Esta função se repete indefinidamente
void loop() {
  leitura(); // executa a leitura do sensor de ultrassom
  Serial.print("VALOR DO SENSOR:"); //envia para o módulo via serial
  Serial.println(distancia); // envia para o módulo via serial
  delay(2000); //espera 2 segundos
}
