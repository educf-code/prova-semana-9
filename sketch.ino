#include <WiFi.h>
#include <HTTPClient.h>

#define led_verde 2 // Pino utilizado para controle do led verda
#define led_vermelho 40 // Pino utilizado para controle do led vermelho
#define led_amarelo 9 // Pino utilizado para controle do led amarelo

const int pino_botao = 18;  // Número do pino do pushbutton
int ultimo_estado_botao = HIGH;  // variável para controlar o estado do botão

const int pino_ldr = 4;  // Número do pino do LDR
int limiar_ldr = 600; // Contante do limiar do LDR

unsigned long tempo_debounce = 0; // variavel para controlar o debounce
unsigned long atraso_debounce = 55; // variavel para armazenar o atraso do debounce

void setup() {

  // Configuração inicial dos pinos para controle dos leds como OUTPUTs (saídas) do ESP32
  pinMode(led_amarelo, OUTPUT); // Inicializa o led amarelo como um output
  pinMode(led_verde, OUTPUT); // Inicializa o led verde como um output
  pinMode(led_vermelho, OUTPUT); // Inicializa o led vermelho como um output

  // Inicialização das entradas
  pinMode(pino_botao, INPUT); // Inicializa o pushbutton como um input
  pinMode(pino_ldr, INPUT); // Inicializa o pushbutton como um input

  /// Deixa os leds apagados na inicialização do sistema
  digitalWrite(led_amarelo, LOW); // Apaga o led amarelo
  digitalWrite(led_verde, LOW); // Apaga o led verde
  digitalWrite(led_vermelho, LOW); // Apaga o led vermelho

  Serial.begin(9600); // Configuração para debug por interface serial entre ESP e computador com baud rate de 9600

  WiFi.begin("Wokwi-GUEST", ""); // Conexão à rede WiFi aberta com SSID Wokwi-GUEST

  // enquanto a conexão falhar, printa pontos para monitorar no monitor serial
  while (WiFi.status() == WL_CONNECT_FAILED) {
    delay(100); // atrasa 0,1 segundos
    Serial.println("."); // printa "."
  }
  Serial.println("Conectado ao WiFi com sucesso!"); // Considerando que saiu do loop acima, o ESP32 agora está conectado ao WiFi (outra opção é colocar este comando dentro do if abaixo)

  if (WiFi.status() == WL_CONNECTED) { // Se o ESP32 estiver conectado à Internet
    HTTPClient http; // inicializa aclasse http da lib HTTPClient

    String serverPath = "http://www.google.com.br/"; // Endpoint da requisição HTTP

    http.begin(serverPath.c_str()); // incializa conexão com o endpoint

    int código_resposta_http = http.GET(); // Código do Resultado da Requisição HTTP

    if (código_resposta_http > 0) { // se a resposta da requisição for maior que zero:
      Serial.print("Código da resposta HTTP: ");
      Serial.println(código_resposta_http); // printa o código da resposta da requisição no monitor serial
      String payload = http.getString(); // pega a resposta da requisição
      Serial.println(payload); // printa a resposta da requisição
    }
    else { // se a resposta for igual ou menor que zero
      Serial.print("Código de erro: ");
      Serial.println(código_resposta_http); // printa o código de resposta
    }
    http.end();
  }

  else { // se o esp32 não estiver conectado à internet
    Serial.println("WiFi Disconnected"); // printa que o wifi está desconectado
  }
}

void loop() {
  int estado_botao = digitalRead(pino_botao);  // Verifica estado do botão
  Serial.println(estado_botao);

  if (estado_botao != ultimo_estado_botao) { // se o estado atual for diferente do último estado:
    tempo_debounce = millis(); // começa a contar o tempo do debounce
  }
  if ((millis() - tempo_debounce) > atraso_debounce) { // se o tempo apertado for maior que o delay que causa o debounce
    if (estado_botao != ultimo_estado_botao) { // se o estado atual for diferente do último estado medido
      ultimo_estado_botao = estado_botao; // atualiza para o estado atual
    }
  }
  if (ultimo_estado_botao == LOW) { // se o ultimo estado for "LOW":
    Serial.println("Botão pressionado!"); // printa que o botão foi pressionado
  } else {
    Serial.println("Botão não pressionado!"); // printa que o botão foi pressionado
  }


  int estado_ldr = analogRead(pino_ldr); // variavel para controlar o valor do ldr

  if (estado_ldr <= limiar_ldr) { // se o valor do ldr for menor ou igual ao limiar:
    Serial.println("Está escuro! Ligue o LED"); // printa mensagem para ligar o LED
    Serial.println(estado_ldr); // printa o valor do ldr
    digitalWrite(led_amarelo, HIGH); // acende o led amarelo
    delay(1000); // espera um segundo
    digitalWrite(led_amarelo, LOW); // apaga o led amarelo
    delay(1000); // espera um segundo

  } else { // se o valor do ldr for maior que o limiar:
    Serial.println("Está claro! Desligue o LED");  // printa mensagem para desligar o LED
    Serial.println(estado_ldr); // printa o valor do ldr
    digitalWrite(led_amarelo, LOW); // apaga o led amarelo
    digitalWrite(led_vermelho, LOW); // apaga o led vermelho
    digitalWrite(led_verde, HIGH); //acende o led verde
    delay(3000); //espera 3 segundos
    digitalWrite(led_vermelho, LOW); // apaga o led vermelho
    digitalWrite(led_verde, LOW); //apaga o led verde
    digitalWrite(led_amarelo, HIGH); // acende o led amarelo
    delay(2000); // espera dois segundos
    digitalWrite(led_verde, LOW); //apaga o led verde
    digitalWrite(led_amarelo, LOW); // apaga o led amarelo
    digitalWrite(led_vermelho, HIGH); // acende led vermelho

    if ( ultimo_estado_botao == LOW ) {
      delay(1000);
      digitalWrite(led_amarelo, LOW); // apaga o led amarelo
      digitalWrite(led_vermelho, LOW); // apaga o led vermelho
      digitalWrite(led_verde, HIGH); //acende o led verde
      delay(3000); //espera 3 segundos
      digitalWrite(led_vermelho, LOW); // apaga o led vermelho
      digitalWrite(led_verde, LOW); //apaga o led verde
      digitalWrite(led_amarelo, HIGH); // acende o led amarelo
      delay(2000); // espera dois segundos
      digitalWrite(led_verde, LOW); //apaga o led verde
      digitalWrite(led_amarelo, LOW); // apaga o led amarelo
      digitalWrite(led_vermelho, HIGH); // acende led vermelho
    }
    delay(5000);// espera 5 segundos


  }
}