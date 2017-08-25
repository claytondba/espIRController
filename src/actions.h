#include <Arduino.h>
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>



class Action
{
  void send_ircmd(String cmd);
  void con_mysql();


public:

  MySQL_Cursor cur = 0;
  WiFiClient netcli;
  PubSubClient MQTT;
  String cmd;
  String topico;
  void execute();
  void publicar();
  void inscrever();

};
