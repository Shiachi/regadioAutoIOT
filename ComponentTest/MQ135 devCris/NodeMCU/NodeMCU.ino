#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

//antes se deben configurar los parametros: placa, com
//Visual estudio compilar ctr + shift + P -> upload


SoftwareSerial mySerial(13, 15); // RX, TX
// longitud del buffer
const int lonbuffer = 10;
// buffer para almacenar el comando
char buffer[lonbuffer];
//Declarar variables 
float hum, tem;
int MoCa, humt;

bool mostrar = false;

void setup() 
{
  Serial.begin(9600);
  mySerial.begin(9600);
  Serial.println("Iniciando.");

 }

 
void loop()
{
 
  if (mySerial.available() > 0)
  {
     
    mySerial.readBytesUntil('T', buffer, lonbuffer);
    tem = mySerial.parseFloat();

     
    mySerial.readBytesUntil('H', buffer, lonbuffer);
    hum = mySerial.parseFloat();
    
    mySerial.readBytesUntil('C', buffer, lonbuffer);
    MoCa = mySerial.parseInt();

    mySerial.readBytesUntil('E', buffer, lonbuffer);
    humt = mySerial.parseInt();

    
    mostrar = true;
  }

  if (mostrar)
  {
     
    Serial.println("Node");
    Serial.print("Temp: ");
    Serial.print(tem);
    Serial.println(" *C ");
    Serial.print("Humedad: ");
    Serial.print(hum);
    Serial.println(" %");
    Serial.print("CO: ");
    Serial.println(MoCa);
    Serial.print("Humedad de la tierra: ");
    Serial.println(humt);
    
    Serial.print(" \n");
    
    mostrar = false;
    delay (2000);
  }
  else{
    Serial.println("Error");
    delay(2000);

  }
}