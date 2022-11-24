#include <SoftwareSerial.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h> // https://github.com/witnessmenow/Universal-Arduino-Telegram-Bot
#include <ArduinoJson.h>
#include <DHT.h>

// Valores del rele
#define Rele 4
const int ONF = 1000;    //Tiempo encendido


SoftwareSerial mySerial(13, 15); // RX, TX
// longitud del buffer
const int lonbuffer = 20;
// buffer para almacenar el comando
char buffer[lonbuffer];

//Declarar variables 
float humA, temA;
//humA = humedad ambiental (dht22)    //temA = temperatura ambiental (dht22)
int CO, CO2, humT; 
// CO = Monóxido de carbono (MQ7)    //CO2 = Dióxido de carbono (MQ135) //humT humedad de tierra

//Conexion a internet y mqtt
const char *ssid = "Kira";
const char *password = "652-CoAt-76";
const char* mqtt_server = "68.183.119.177"; // Este es el address en mqtt dash
const char* keyDevice = "";
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
bool conexion = false;
bool enviar = false;
// RIOTL = Sistema de regadio automatico (activado por defecto)
bool RIOTL = true;

//Id de contacto
#define CHAT_ID "1065469951"

// ID de bot de telegram
#define BOTtoken "5804652166:AAFfRRmba72V4iaZ6B77mfx-9UN29Ia_Dyc"

//iniciar cliente seguro
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure clientSecure;
UniversalTelegramBot bot(BOTtoken, clientSecure);

// Revisar mensajes de telegram cada 1 segundo
int botRequestDelay = 1000;
unsigned long lastTimeBotRan;     

String obtenerValores(){
  //Si los valores recibidos por la comunicaion serial son mayores a 0 recuperar datos
  if (mySerial.available() > 0)
  {
    mySerial.readBytesUntil('A', buffer, lonbuffer);
    CO2 = mySerial.parseInt();

     
    mySerial.readBytesUntil('E', buffer, lonbuffer);
    humT = mySerial.parseInt();
    
    mySerial.readBytesUntil('C', buffer, lonbuffer);
    CO = mySerial.parseInt();

    mySerial.readBytesUntil('H', buffer, lonbuffer);
    humA = mySerial.parseFloat();

    mySerial.readBytesUntil('T', buffer, lonbuffer);
    temA = mySerial.parseFloat();

    conexion = true;
    enviar = true;
    //cambiar enviar a true para enviar datos
  }
  //Si enviar es "true" se envian los datos y se imprimen para test
  if (enviar)
  {
     
    Serial.println("Node");
    // Temperatura ambiental
    Serial.print("Temp: ");
    Serial.print(temA);
    Serial.println(" *C ");
    client.publish("RIOT/DHT22T",String(temA).c_str()); //publicar en topico de mqtt
    String message = "Temperatura ambiental: " + String(temA) + " ºC \n"; //Variable que almacena los datos para enviar al bot de telegram
    //Humedad ambiental
    Serial.print("Humedad: ");
    Serial.print(humA);
    Serial.println(" %");
    client.publish("RIOT/DHT22H",String(humA).c_str()); 
    message += "Humedad ambiental: " + String (humA) + " % \n";
    //Monoxido de carbono (CO)
    Serial.print("CO: ");
    Serial.println(CO);
    client.publish("RIOT/MQ7",String(CO).c_str()); 
    message += "Monoxido de carbono: " + String (CO) + "\n";
    //Dióxido de carbono (CO2)
    Serial.print("CO2: ");
    Serial.println(CO2);
    client.publish("RIOT/MQ135",String(CO2).c_str()); 
    message += "Dioxido de carbono: " + String (CO2) + "\n";
    //Humedad de la tierra
    Serial.print("Humedad de la tierra: ");
    Serial.println(humT);
    client.publish("RIOT/humT",String(humT).c_str());
    message += "Humedad de la tierra: " + String (humT) + "\n"; 
    
    return message;
    Serial.print(" \n");
    enviar = false;
    delay (5000);
    //camiar enviar a "false" y agregar un delay para sincronizar los datos recibidos con los que envia el arduino.
  }
  //Si la conexion falla imprimir error y desactivar sistema inteligente
  else{
    Serial.println("Error");
    String message = "Error de conexion \n";
    conexion = false;
    RIOTL = false;
    return message;
    delay(5000);
  }

}

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i=0; i<numNewMessages; i++) {
    // Chat id of the requester
    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != CHAT_ID){
      bot.sendMessage(chat_id, "Unauthorized user", "");  //Autentificar usuario a travez de su id
      continue;
    }
    
    // Print the received message
    String text = bot.messages[i].text;
    Serial.println(text);

    String from_name = bot.messages[i].from_name;

    if (text == "/start") { //Comando para iniciar la comunicacion con el bot
      String welcome = "Welcome, " + from_name + ".\n";
      welcome += "Use the following command to get current readings.\n\n";
      welcome += "/readings \n";
      bot.sendMessage(chat_id, welcome, "");
    }

    if (text == "/readings") {  //Comando para obtener lecturas de los sensores
      String readings = obtenerValores();
      bot.sendMessage(chat_id, readings, "");
    }  
  }
}


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
void callback(char* topic, byte* payload, unsigned int length) {
 String topico = topic;
if(topico=="RIOT/01")
{
    if (((char)payload[0] == '1')) { // 1 para prender
      digitalWrite(Rele,HIGH);
      Serial.println("Bomba encendida");
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
}
//Reconeccion y subpripcion de botones
void reconnect() {

  if (!client.connected()) {

    if (client.connect(keyDevice,"","")){//Servidor broker es el usuario y prueba12 la contraseña en mqtt dash
      Serial.println("connected");
      client.subscribe("RIOT/01"); // Topico al momento de crear el boton
      client.subscribe("RIOT/02"); // Topico al momento de crear el boton
    } else {
      delay(1000);
    }
  }
}

void setup() 
{

  configTime(0, 0, "pool.ntp.org");      // Obtenr la hora UTC a travez del protocolo NTP
  clientSecure.setTrustAnchors(&cert); // Agregar certificado obtenido de la api de telegram

  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Iniciando.");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); 
  pinMode(Rele, OUTPUT);//Define el pin RELE como salida
  digitalWrite(Rele, LOW);//Rele inicia apagado
  
 }

 
void loop()
{

  //Si cliente esta desconectado activar funcion reconectar
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  obtenerValores();

  if (millis() > lastTimeBotRan + botRequestDelay)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while(numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
  
    if(RIOTL == true && conexion == true)
    {
        Serial.println("Sistema de riego automatizado encendido");
        delay(1000);
    if (humT <  500){
        Serial.println("Sistema de riego automatizado activado");
        digitalWrite(Rele, HIGH);//Enciende el rele
        delay(1000);
    }
    else
    {
        digitalWrite(Rele, LOW);//Apaga el rele
        Serial.println("Sistema de riego automatizado desactivado");
        delay(1000);
    }
    }
    if(RIOTL == false || conexion == false){
        Serial.print(RIOTL);
        Serial.print(conexion);
        Serial.println("Sistema de riego automatizado apagado");
        delay(1000);
    }
}



$ git config --global user.name "Shiachi"
$ git config --global user.email w.leroy@alumnos.santotomas.cl


