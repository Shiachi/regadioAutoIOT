#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

SoftwareSerial mySerial(13, 15); // RX, TX
// longitud del buffer
const int lonbuffer = 10;
// buffer para almacenar el comando
char buffer[lonbuffer];
// valor del primer parámetro
float temperatura;
// valor del segundo parámetro
float humedad;
bool mostrar = false;

void setup() 
{
  Serial.begin(9600);
  Serial.println("Iniciando.");

 }

 
void loop()
{
 
  if (mySerial.available() > 0)
  {
     
    mySerial.readBytesUntil('T', buffer, lonbuffer);
    temperatura = mySerial.parseFloat();

     
    mySerial.readBytesUntil('H', buffer, lonbuffer);
    humedad = mySerial.parseFloat();

    mostrar = true;
  }

  if (mostrar)
  {
     
     
    Serial.print("temp: ");
    Serial.println(temperatura);
    Serial.print("humedad: ");
    Serial.println(humedad);
    mostrar = false;
    delay (1000);
  }
  
}
