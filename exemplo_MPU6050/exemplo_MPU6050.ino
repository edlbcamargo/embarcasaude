// Programa de teste do aceletômetro / giroscópio MPU6050
//
// Referências:
//
// [1] MPU-6000/MPU-6050 Register Map and Descriptions
//     Disponível em https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf
//
// [2] https://www.filipeflop.com/blog/tutorial-acelerometro-mpu6050-arduino/
//
//
// Criado por: Erick León, 2022
//
// Adaptado por: Lucas Jácome

int buzzer = 7;
#include<Wire.h>                           // Biblioteca para comunicação I2C
const int endereco_MPU = 0x68;             // Variável que irá armazenar o endereço I2C do módulo MPU6050
int eixo_X, eixo_Y, eixo_Z, Tmp, GyX, GyY, GyZ;     // Criando variaveis de inclinação e acelaração.

// esta função só roda uma vez, no início
void setup()
{
    Serial.begin(115200);                  // Inicia comunicação serial com velocidade de 115200bps
    pinMode(buzzer,OUTPUT);
    
    Wire.begin();                          // Iniciando comunicação I2C
    Wire.beginTransmission(endereco_MPU);  // Iniciando transmissão para o endereço do MPU6050
    //Inicializa o MPU-6050 com clock interno 8MHz
    Wire.write(0x6B);                      // Gerenciamento de energia do módulo.
    Wire.write(0);                         // Ver [1], pág. 40
    Wire.endTransmission(true);
}

// esta função se repete indefinidamente
void loop()
{
    Wire.beginTransmission(endereco_MPU);
    Wire.write(0x3B);                        // Começando com o registrador 0x3B (ACCEL_YOUT_H). Ver [1], pág. 29
    Wire.endTransmission(false);
    
    //Solicita 14 bytes de dados do módulo MPU6050
    Wire.requestFrom(endereco_MPU, 14, true);
    
    // Armazena o valor dos 7 sensores nas variáveis correspondentes
    // Cada medida possui 16-bits, ou seja, é formada por dois bytes:
    eixo_Y = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_YOUT_H) & 0x3C (ACCEL_YOUT_L)
    eixo_X = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_XOUT_H) & 0x3E (ACCEL_XOUT_L)
    eixo_Z = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
    Tmp = Wire.read() << 8 | Wire.read();    // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
    GyX = Wire.read() << 8 | Wire.read();    // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
    GyY = Wire.read() << 8 | Wire.read();    // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
    GyZ = Wire.read() << 8 | Wire.read();    // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
    
    Serial.print("Temp= "); Serial.print((Tmp/340)+36.53);       // Envia valor da temperatura para a serial. Ver [1] pág. 30
  
    Serial.print("\tInclin_X= "); Serial.print(eixo_X/180);      // Envia inclinação no eixo X para a serial
    Serial.print("\tInclin_Y= "); Serial.print(eixo_Y/180);      // Envia inclinação no eixo Y para a serial
    Serial.print("\tInclin_Z= "); Serial.print(eixo_Z/180);      // Envia inclinação no eixo Z para a serial
    
    Serial.print("\tVel.Inclinacao_X= "); Serial.print(GyX/180); // Envia velocidade de rotação no eixo X para a serial
    Serial.print("\tVel.Inclinacao_Y= "); Serial.print(GyY/180); // Envia velocidade de rotação no eixo Y para a serial
    Serial.print("\tVel.Inclinacao_Z= "); Serial.print(GyZ/180); // Envia velocidade de rotação no eixo Z para a serial
    
    Serial.println("");
    //Aguarda 100 ms e reinicia o processo
    delay(100);
    if (Gyz > 40)
    {
      digitalWrite(buzzer,HIGH|);
      delay(1000);
      digitalWrite(buzzer,LOW);
      delay(1000);
    }
}
