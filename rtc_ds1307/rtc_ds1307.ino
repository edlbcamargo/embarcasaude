 /*******************************************************************************
  * 
  * Exemplo de uso do módulo Tiny RTC com DS1307.
  * 
  * Referências:
  * https://github.com/adafruit/RTClib
  * 
  * Criado por: Erick León, 2022
  * 
  ********************************************************************************/
#include "RTClib.h"

RTC_DS1307 rtc;

char diasDaSemana[7][4] = {"DOM", "SEG", "TER", "QUA", "QUI", "SEX", "SAB"};

// esta função só roda quando é chamada dentro de outra função
void imprime_data_e_hora(DateTime data_e_hora){
    Serial.print(data_e_hora.day(), DEC);
    Serial.print('/');
    Serial.print(data_e_hora.month(), DEC);
    Serial.print('/');
    Serial.print(data_e_hora.year(), DEC);
    Serial.print(" (");
    Serial.print(diasDaSemana[data_e_hora.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(data_e_hora.hour(), DEC);
    Serial.print(':');
    Serial.print(data_e_hora.minute(), DEC);
    Serial.print(':');
    Serial.print(data_e_hora.second(), DEC);
    Serial.println();
}

// esta função só roda uma vez, no início
void setup () {
    Serial.begin(9600);
  
    if (!rtc.begin()) { // Se o módulo não estiver conectado ao Arduino...
        Serial.println("Módulo RTC não encontrado.");
        Serial.flush();
        while (1);  // fica aqui parado em um loop infinito...
    }
  
    if (rtc.isrunning()) { // Se o módulo não tiver sido inicializado ainda...
        Serial.println("Módulo RTC não inicializado. Ajustando data e hora.");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // ajusta com data e hora em que o programa foi compilado
    }
}

int k =0;
// esta função se repete indefinidamente
void loop () {
    k++;
    //DateTime agora = rtc.now();
    DateTime agora = rtc.now() + TimeSpan(0,0,0,3*k);
    imprime_data_e_hora(agora);

    DateTime futuro = agora + TimeSpan(1,2,3,4); // agora + 1 dia, 2 horas, 3 minutos e 4 segundos
    Serial.print("Daqui 1 dia, 2 horas, 3 minutos e 4 segundos: ");
    imprime_data_e_hora(futuro);

    delay(3000);
}
