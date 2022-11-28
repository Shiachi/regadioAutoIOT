#include <SoftwareSerial.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> // https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>
#include <DHT.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Valores del rele
#define Rele 4
const int ONF = 1000;    //Tiempo encendido
int delayTime = 2000;

SoftwareSerial mySerial(13, 15); // RX, TX
// longitud del buffer
const int lonbuffer = 500;
// buffer para almacenar el comando
char buffer[lonbuffer];

//Declarar variables int horaUsuario;
float humA, temA, humAT, temAT ;  //humAT, tempT variables de prueba
//humA = humedad ambiental (dht22)    //temA = temperatura ambiental (dht22)
int CO, CO2, humT, horaUsuario, COT, CO2T, humTT;   //COT, CO2T, humTT variables de prueba
// CO = Monóxido de carbono (MQ7)    //CO2 = Dióxido de carbono (MQ135) //humT humedad de tierra

//Iniciar conexion con bot de telegram 
#define BOTtoken "5804652166:AAEG3bdPwIZlUhwQWfAHqJ9TbTIfmSH3THY"  // your Bot Token (Get from Botfather)

//ID a la cual el bot enviara un mensaje
#define CHAT_ID "1065469951"

//Conexion con internet
const char *ssid = "Kira";
const char *password = "652-CoAt-76";
//Conexion con mqtt server
const char* mqtt_server = "68.183.119.177"; // Este es el address en mqtt dash
const char* keyDevice = "";
//Iniciar cliente
WiFiClient espClient;
PubSubClient client(espClient);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
bool conexion = false;
bool enviar = false;
// RIOTL = Sistema de regadio automatico (activado por defecto)
bool RIOTL = true;

//iniciar cliente seguro
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure clientSecure;
UniversalTelegramBot bot(BOTtoken, clientSecure);
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

//configurar conexion a internet y mqqt
void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("OK");
}
void callback(char* topic, byte* payload, unsigned int length) 
{
  String topico = topic;
  // Subscripcion de botones
  if(topico=="RIOT/01")
  {
      if (((char)payload[0] == '1')) { // 1 para prender
        digitalWrite(Rele, HIGH);//Enciende el rele
        delay(delayTime);
        digitalWrite(Rele, LOW);//Apaga el rele
    } else if (((char)payload[0] == '0')) { // 0 para apagar
        digitalWrite(Rele,LOW);
        Serial.println("Bomba apagada");}
  }
  else if(topico=="RIOT/02")
  {
      if (((char)payload[0] == '0')) { // 0 para prender (activada por defecto)
        RIOTL = true;
        Serial.println("Sistema de riego automatizado apagado");
    } else if (((char)payload[0] == '1')) { // 1 para apagar
        RIOTL = false;
        Serial.println("Sistema de riego automatizado encendido");}
  }
  if(topico=="RIOT/BT")
  {

    String content = "";   
    for (size_t i = 0; i < length; i++)   
    {
      content.concat((char)payload[i]);
    }
    horaUsuario = content.toInt();
    Serial.println(horaUsuario);
  }

  
}
//Reconeccion y subscripcion de botones
void reconnect() 
{
  if (!client.connected()) {
    if (client.connect(keyDevice,"","")){//Servidor broker es el usuario y prueba12 la contraseña en mqtt dash
      Serial.println("connected");
      client.subscribe("RIOT/01"); // Topico al momento de crear el boton
      client.subscribe("RIOT/02"); // Topico al momento de crear el boton
      client.subscribe("RIOT/BT");
    } else {
      delay(delayTime);
    }
  }
}
void handleNewMessages(int numNewMessages) 
{
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");  //Autentificar usuario a traves de su id
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;
    //zona de comandos
    if (text == "/readings") {  //Comando para obtener lecturas de los sensores
      String readings = obtenerValores();
      bot.sendMessage(chat_id, readings, "");
    }  
  }
}
String obtenerValores(){
  //Si los valores recibidos por la comunicacion serial son mayores a 0 recuperar datos
  if (mySerial.available() > 0)
  {
    mySerial.readBytesUntil('A', buffer, lonbuffer);
    CO2T = mySerial.parseInt();
    if(CO2T == 1){
      CO2 = CO2;
    }
    else if (CO2T > 1)
    {
      CO2 = CO2T;
    }
     
    mySerial.readBytesUntil('E', buffer, lonbuffer);
    humTT = mySerial.parseInt();
    if(humTT == 1){
      humT = humT;
    }
    else if (humTT > 1)
    {
      humT = humTT;
    }
    
    mySerial.readBytesUntil('C', buffer, lonbuffer);
    COT = mySerial.parseInt();
    if(COT == 1){
      CO = CO;
    }
    else if (COT > 1)
    {
      CO = COT;
    }

    mySerial.readBytesUntil('H', buffer, lonbuffer);
    humAT = mySerial.parseFloat();
    if(humAT == 1){
      humA = humA;
    }
    else if (humAT > 1)
    {
      humA = humAT;
    }

    mySerial.readBytesUntil('T', buffer, lonbuffer);
    temAT = mySerial.parseFloat();
    if(temAT == 1){
      temA = temA;
    }
    else if (temAT > 1)
    {
      temA = temAT;
    }
    else{
      Serial.println("error de lecutra");
    }

    conexion = true;
    enviar = true;
    //cambiar enviar a true para enviar datos
  }
  //Si enviar es "true" se envian los datos y se imprimen para test
  if (enviar)
  {
     
    Serial.println("Node");
    // Temperatura ambiental
    client.publish("RIOT/DHT22T",String(temA).c_str()); //publicar en topico de mqtt
    String message = "Temperatura ambiental: " + String(temA) + " C \n"; //Variable que almacena los datos para enviar al bot de telegram
    //Humedad ambiental
    client.publish("RIOT/DHT22H",String(humA).c_str()); 
    message += "Humedad ambiental: " + String (humA) + " % \n";
    //Monoxido de carbono (CO)
    client.publish("RIOT/MQ7",String(CO).c_str()); 
    message += "Monoxido de carbono: " + String (CO) + "\n";
    //Dióxido de carbono (CO2)
    client.publish("RIOT/MQ135",String(CO2).c_str()); 
    message += "Dioxido de carbono: " + String (CO2) + "\n";
    //Humedad de la tierra
    client.publish("RIOT/humT",String(humT).c_str());
    message += "Humedad de la tierra: " + String (humT) + "\n"; 

    Serial.println(message);
    return message;
    Serial.print(" \n");
    enviar = false;
    delay(delayTime);
    //Cambiar enviar a "false" y agregar un delay para sincronizar los datos recibidos con los que envía el Arduino.
  }
  //Si la conexion falla imprimir error y desactivar sistema inteligente
  else{
    Serial.println("Error");
    String message = "Error de conexion \n";
    conexion = false;
    RIOTL = false;
    return message;
    delay(delayTime);
  }

}
void setup() 
{
  Serial.begin(115200);
  mySerial.begin(115200);
  Serial.println("Iniciando.");
  setup_wifi();
  // Configuracion del mqtt
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);  
  // Agregar certificado obtenido del api de telegram
  clientSecure.setTrustAnchors(&cert); 
  //definir uso de pines
  pinMode(Rele, OUTPUT);//Define el pin RELE como salida
  digitalWrite(Rele, LOW);//Rele inicia apagado
  //Iniciar conexion con ntp para tomar tiempo
  timeClient.begin();
  timeClient.setTimeOffset(-10800); //UTC * 60 *60

  //Suscripcion de topicos

 }
void logicaRegadio()
{
  timeClient.update();
  int cH = timeClient.getHours();
   Serial.println(cH);
   
    if(RIOTL == true && conexion == true && cH == horaUsuario)
    {
        Serial.println("Sistema de riego automatizado encendido");
    if (humT >  500){
        Serial.println("Sistema de riego automatizado activado");
        digitalWrite(Rele, HIGH);//Enciende el rele
        delay(delayTime);
        digitalWrite(Rele, LOW);//Apaga el rele
    }
    else
    {
        digitalWrite(Rele, LOW);//Apaga el rele
        Serial.println("Sistema de riego automatizado desactivado");
    }
    }
    if(RIOTL == false || conexion == false){
        Serial.print(RIOTL);
        Serial.print(conexion);
        Serial.println("Sistema de riego automatizado apagado");
        delay(delayTime);
    }
}
void loop()
{
    obtenerValores();
    logicaRegadio();
    //Si cliente esta desconectado activar funcion reconectar
    if (!client.connected())
    {
      reconnect();
    }
    client.loop();
    if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}