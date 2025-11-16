#include <WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"

#define LDR_PIN      34
#define LED_PIN      4
#define BUZZER_PIN   18
#define DHT_PIN      15

// simula valores aleatórios

const bool RANDOM_MODE = true;

const char* ssid        = "Wokwi-GUEST";
const char* password    = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);
DHTesp dht;

// Controle de tempo de tela
unsigned long tempoInicio       = 0;
const unsigned long INTERVALO_DESCANSO = 10000;

void setup_wifi() {
  delay(10);
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("SmartFocusClient")) {
      Serial.println("Conectado ao broker MQTT!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LDR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  dht.setup(DHT_PIN, DHTesp::DHT22);

  setup_wifi();
  client.setServer(mqtt_server, 1883);

  randomSeed(analogRead(0));
  tempoInicio = millis();
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  float luz, temp, umid;

  if (RANDOM_MODE) {
    // Valores simulados para facilitar a demonstração
    luz  = random(200, 1200);   // 200–1200 (escuro → claro)
    temp = random(22, 32);      // 22–32 °C
    umid = random(30, 70);      // 30–70 %
  } else {
    
    luz = analogRead(LDR_PIN);
    TempAndHumidity data = dht.getTempAndHumidity();
    temp = data.temperature;
    umid = data.humidity;
  }

  int nivelEstresse = 0;

  // Temperatura alta aumenta estresse
  if (temp > 27 && temp <= 30) {
    nivelEstresse += 40;
  } else if (temp > 30) {
    nivelEstresse += 60;
  }

  // Pouca luz também aumenta estresse
  if (luz < 500) {
    nivelEstresse += 30;
  }

  // Umidade muito baixa ou muito alta pode incomodar
  if (umid < 35 || umid > 65) {
    nivelEstresse += 20;
  }

  // Tempo de tela
  unsigned long tempoAtual   = millis();
  unsigned long tempoTelaMs  = tempoAtual - tempoInicio;
  unsigned long tempoTelaMin = tempoTelaMs / 10000;
  if (tempoTelaMs > INTERVALO_DESCANSO) {
    nivelEstresse += 30;
  }

  // valor fique entre 0 e 100
  if (nivelEstresse > 100) nivelEstresse = 100;
  if (nivelEstresse < 0)   nivelEstresse = 0;

  //  nível de estresse
  String classificacao;
  if (nivelEstresse < 35) {
    classificacao = "Baixo";
  } else if (nivelEstresse < 70) {
    classificacao = "Moderado";
  } else {
    classificacao = "Alto";
  }

  // ALERTA (LED + BUZZER) 
  String statusMsg;

  if (nivelEstresse >= 70) {
    digitalWrite(LED_PIN, HIGH);
    tone(BUZZER_PIN, 1000);
    statusMsg = "Estresse alto - sugerir pausa";
  } else {
    digitalWrite(LED_PIN, LOW);
    noTone(BUZZER_PIN);
    statusMsg = "Ambiente confortavel / estresse controlado";
  }

  // --------- LEMBRETE EXPLÍCITO DE TEMPO DE TELA ---------
  if (tempoTelaMs > INTERVALO_DESCANSO) {
    Serial.println("Tempo de tela excedido! Faça uma pausa para descansar os olhos e o corpo.");
    // Reinicia contagem de tempo de tela
    tempoInicio = millis();
  }

  // --------- PAYLOAD MQTT ---------
  String payload = "{";
  payload += "\"temperatura\": " + String(temp, 1) + ",";
  payload += "\"umidade\": " + String(umid, 1) + ",";
  payload += "\"luz\": " + String(luz, 0) + ",";
  payload += "\"nivelEstresse\": " + String(nivelEstresse) + ",";
  payload += "\"classificacaoEstresse\": \"" + classificacao + "\",";
  payload += "\"status\": \"" + statusMsg + "\"";
  payload += "}";

  client.publish("smartfocus/status", payload.c_str());
  Serial.println(payload);

  delay(4000);
}
