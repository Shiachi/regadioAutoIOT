#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

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
    
    mySerial.readBytesUntil('CO', buffer, lonbuffer);
    MoCa = mySerial.parseInt();

    mySerial.readBytesUntil('ht', buffer, lonbuffer);
    humt = mySerial.parseInt();
    mostrar = true;
  }

  if (mostrar)
  {
     
     
    Serial.print("temp: ");
    Serial.println(tem);

    Serial.print("humedad: ");
    Serial.println(hum);

    Serial.print("humedad tierra: ");
    Serial.println(humt);

    Serial.print("CO: ");
    Serial.println(MoCa);
    
    mostrar = false;
    delay (1000);
  }
  else{
    Serial.println("Error");
    delay(1000);

  }
  
}
