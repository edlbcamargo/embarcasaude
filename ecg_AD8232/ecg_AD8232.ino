// Teste de leitura de sinais de ECG usando o módulo AD8232.
//
// Criado por: Lucas Jácome Cabral, 2023

/////////////////////////////////////////////////////////////////
// esta função só roda uma vez, no início
void setup() {
  // inicializa a comunicação serial:
  Serial.begin(115200);

  // Define como entrada os pinos de detecção de eletrodo solto:
  pinMode(4, INPUT); 
  pinMode(5, INPUT); 
}

/////////////////////////////////////////////////////////////////
// esta função se repete indefinidamente
void loop() {
  // Se algum eletrodo está solto, imprime '!' pela serial
  if((digitalRead(4) == 1)||(digitalRead(5) == 1)){
    Serial.println('!');
  }
  else{
    //Leitura do pino Analógico e envio pela porta serial
    Serial.println(analogRead(A0));
  }
  
  // aguarda 2ms, o que equivale a aprox. 500 medições por segundo
  delay(2);
}
