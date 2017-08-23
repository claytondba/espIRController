#include <Arduino.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

const char* SSID = "ssid";
const char* PASSWORD = "pass";

const int BAUD_RATE = 115200;

const char* BROKER_MQTT = "DSDSDSDSD";
const int BROKER_PORT = 8080;

WiFiClient espClient;
PubSubClient MQTT(espClient);

void initWiFi();
void initSerial();

void setup()
{

 //Teste
}

void loop()
{

}

void initSerial() {
  Serial.begin(BAUD_RATE);
}

void initWiFi() {
  delay(10);
  Serial.println("Conectando-se em: " + String(SSID));

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado na Rede " + String(SSID) + " | IP => ");
  Serial.println(WiFi.localIP());
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {

  String message;
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    message += c;
  }
  Serial.println("Tópico => " + String(topic) + " | Valor => " + String(message));
  if (message == "1") {
    Serial.println("1");
  }
  else 
  {
    Serial.println(message);
  }

  Serial.flush();
}
