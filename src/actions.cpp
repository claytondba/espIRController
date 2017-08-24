#include "actions.h"

void connect_mqtt()
{

}
void Action::publicar()
{

}
void Action::execute()
{

}
void Action::con_mysql()
{

}

void Action::send_ircmd(String cmd)
{
  if(cmd == "gof")
  {

  }
  else
    Serial.println("Comando não reconhecido! " + cmd);


}
void Action::inscrever()
{
  char str[topico.length()];
  topico.toCharArray(str, topico.length());

  MQTT.subscribe(str);
}
