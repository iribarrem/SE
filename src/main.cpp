#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoOTA.h>

#define SSID        "Ribeiro"
#define PASSWORD    "ribeironet"

// Dados broker MQTT
#define MQTT_SERVER "mqtt.iribarrem.com"
#define MQTT_PORT   1883
#define MQTT_USER   "api"
#define MQTT_PASSWD "api1300"

// Variáveis e função callback para MQTT
char *message;
char *topic;
bool msg_received = false;

void mqtt_cb(char* tpc, byte* payload, unsigned int length){
  Serial.println("\n\n");
  Serial.println("MQTT RECEIVED!");
  Serial.print("Topic: ");
  Serial.println(tpc);
  Serial.print("Payload: ");
  payload[length] = '\0';
  message = (char*)payload;
  Serial.println(message);
  Serial.println("\n\n");

  topic = tpc;
  msg_received = true;
}

WiFiClient espClient;
PubSubClient mqtt(MQTT_SERVER, MQTT_PORT, mqtt_cb, espClient);

void setup() {
  Serial.begin(115200);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("\nEstabelecendo conexão WiFi...");
  }
  Serial.print("Endereço de IP: ");
  Serial.println(WiFi.localIP());

  ArduinoOTA.begin();

  pinMode(0, OUTPUT);
}

void loop() {
  ArduinoOTA.handle();

  if(!mqtt.connected()){
    //Serial.println("Conectado ao broker MQTT\n");
    mqtt.connect("ESP01", MQTT_USER, MQTT_PASSWD);
    mqtt.subscribe("teste");
  }

  // Tratamento mensagem MQTT
  if(msg_received){
    if(strcmp(message, "LIGAR") == 0)
      digitalWrite(0, LOW);
    else if(strcmp(message, "DESLIGAR") == 0)
      digitalWrite(0, HIGH);

    msg_received = false;
  }

  mqtt.loop();
}