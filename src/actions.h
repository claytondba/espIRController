#include <Arduino.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>

class Action
{
  void send_ircmd(String cmd);
  
public:
  PubSubClient MQTT;
  String cmd;
  String topico;
  void execute();
  void publicar();
  void inscrever();


};
