// Um programa "inútil", para mostrar como o comando 'for' funciona, 
// que cria uma variável 'segundos', inicializa ela com valor 0, e a cada
// 1 segundo (100x10ms) acrescenta um ao seu valor
//
// Criado por: Erick León, 2022

int segundos;           // uma variável de número inteiro chamada segundos

void setup() {
    segundos = 0;     // inicializa a variável 'segundos' com o valor 0
}

void loop() {
    for (int i = 1; i <= 100; i++){  // com 'i' saindo de 1, enquanto for menor igual a 100, soma 1 em 'i' a cada iteração
        delay(10);    // aguarda 10ms
    }
    segundos++;  // o mesmo que: segundos = segundos+1;
}
