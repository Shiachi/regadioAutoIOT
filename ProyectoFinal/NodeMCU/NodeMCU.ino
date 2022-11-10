#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

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
bool RIOTL = true;
//Sistema de regadio automatico (activado por defecto)

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
    Serial.print("Temp: ");
    Serial.print(temA);
    client.publish("RIOT/DHT22T",String(temA).c_str()); 
    Serial.println(" *C ");
    Serial.print("Humedad: ");
    Serial.print(humA);
    client.publish("RIOT/DHT22H",String(humA).c_str()); 
    Serial.println(" %");
    Serial.print("CO: ");
    Serial.println(CO);
    client.publish("RIOT/MQ7",String(CO).c_str()); 
    Serial.print("CO2: ");
    Serial.println(CO2);
    client.publish("RIOT/MQ135",String(CO2).c_str()); 
    Serial.print("Humedad de la tierra: ");
    Serial.println(humT);
    client.publish("RIOT/humT",String(humT).c_str()); 
    
    Serial.print(" \n");
    enviar = false;
    delay (5000);
    //camiar enviar a "false" y agregar un delay para sincronizar los datos recibidos con los que envia el arduino.
  }
  //Si la conexion falla imprimir error y desactivar sistema inteligente
  else{
    Serial.println("Error");
    conexion = false;
    RIOTL = false;
    delay(5000);
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