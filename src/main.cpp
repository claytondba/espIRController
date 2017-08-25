#include "actions.h"


//ID do dispositivo !!!
#define ID_DIPO "ir-remote-01"

#define SSID "ClayAirport"
#define PASSWORD  "pass"

#define BAUD_RATE 115200

#define BROKER_MQTT "m13.cloudmqtt.com"
#define BROKER_PORT  16585
#define BROKER_USER "DSDSDSDSD"
#define BROKER_PASS "DSDSDSDSD"
#define SUB_TOPIC "home/sala/ircontroll/cmd"

//Configurcao do IR
#define RECV_PIN 12
#define SEND_PIN 14

#define DEBUG //Depuracao!!


WiFiClient espClient;
PubSubClient MQTT(espClient);
Action act;

IPAddress server_addr(10,0,1,35); // IP mysql
char user[] = "root";              // MySQL user login username
char password[] = "secret"; // MySQL user login password
MySQL_Connection conn((Client *)&espClient);

bool MODO_COPIA = false; //Modo de cópia de codigos IR

IRrecv irrecv(RECV_PIN); //VARIÁVEL DO TIPO IRrecv
IRsend irsend(SEND_PIN);
decode_results results;

//Protótipos
void initWiFi();
void initSerial();
void dump(decode_results *results);
void mqtt_callback(char* topic, byte* payload, unsigned int length);
void recconectWiFi();
void reconnectMQTT();
void initMQTT();


void setup()
{

  act.MQTT = MQTT;
  act.topico = SUB_TOPIC;
  act.cur = MySQL_Cursor(&conn);

  initSerial();

  if(MODO_COPIA)
  {
    Serial.println("Atencao!! Modo de copia de comandos ativado!!");
    irrecv.enableIRIn();
  }
  else
  {
    initWiFi();
    #ifdef DEBUG
      Serial.println("Passou WiFi no setup....");
    #endif

    initMQTT();
    #ifdef DEBUG
      Serial.println("Passou MQTT no setup....");
    #endif
    if (conn.connect(server_addr, 3306, user, password))
    {
      Serial.println("Entrou!!!!!!");
      // You would add your code here to run a query once on startup.
    }
    else
    {
      Serial.println("Connection failed.");
      conn.close();
    }
    #ifdef DEBUG
      Serial.println("Passou Mysql no setup....");
    #endif
  }
}

void loop()
{
  if(MODO_COPIA)
  {

    if (irrecv.decode(&results))
    {
      Serial.println(results.value, HEX);
      dump(&results);
      irrecv.resume(); //RECEBE O PRÓXIMO VALOR
    }
  }
  else
  {
    if (!MQTT.connected())
    {
      reconnectMQTT();
    }

    recconectWiFi();
    MQTT.loop();
  }
}

void initSerial()
{
  Serial.begin(BAUD_RATE);
}

void initWiFi()
{
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

void reconnectMQTT()
{
  while (!MQTT.connected())
  {
    Serial.println("Tentando se conectar ao Broker MQTT: " + String(BROKER_MQTT));

    if (MQTT.connect(ID_DIPO, BROKER_USER, BROKER_PASS))
    {
      Serial.println("Conectado");
      act.inscrever();
    }
    else
    {
      Serial.println("Falha ao Reconectar :(");
      Serial.println("Tentando se reconectar em 2 segundos");
      delay(2000);
    }
  }
}

void recconectWiFi()
{
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(100);
    Serial.print(".");
  }
}

void initMQTT()
{
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(mqtt_callback);
}

//Método para tratar menssagens recebidas
void mqtt_callback(char* topic, byte* payload, unsigned int length)
{

  String message;
  for (int i = 0; i < length; i++)
  {
    char c = (char)payload[i];
    message += c;
  }

  Serial.println("Tópico => " + String(topic) + " | Valor => " + String(message));
  Serial.flush();

  act.cmd = message;
  act.execute();

}

void dump(decode_results *results) {
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  }
  else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");
  }
  else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  }
  else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  }
  else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  }
  else if (results->decode_type == PANASONIC) {
    Serial.print("Decoded PANASONIC - Address: ");
    Serial.print(results->panasonicAddress, HEX);
    Serial.print(" Value: ");
  }
  else if (results->decode_type == LG) {
    Serial.print("Decoded LG: ");
  }
  else if (results->decode_type == JVC) {
    Serial.print("Decoded JVC: ");
  }
  else if (results->decode_type == AIWA_RC_T501) {
    Serial.print("Decoded AIWA RC T501: ");
  }
  else if (results->decode_type == WHYNTER) {
    Serial.print("Decoded Whynter: ");
  }
  Serial.print(results->value, HEX);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");

  for (int i = 1; i < count; i++)
  {
    if (i & 1)
    {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    }
    else
    {
      Serial.write('-');
      Serial.print((unsigned long) results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println();
}
