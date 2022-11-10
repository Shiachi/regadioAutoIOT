/*
      cosas para tener en cuenta
      1. al subir el codigo los cables RX y TX tienen que estar desconectados.
      2. El nodemcu no es capaz de conectarse a internet si los cables RX y TX estan conectados
      espere a que el nodemcu se conecte a internet antes de conectar los cables RX y TX.
*/

#include <SoftwareSerial.h>   // Libreria necesaria para la comunicacion.
#include <ESP8266WiFi.h>

SoftwareSerial mySerial(13, 15); // RX(13), TX(15).
// Se evitan los pines RX0 y TX0 del nodemcu para evitar bugs.

// Espacio de buffer que se utilizara.
const int lonbuffer = 10;   // Longitud del buffer.
char buffer[lonbuffer];   // Buffer para almacenar el comando.


float temperatura, humedad; // Definir variables que en este caso recibiran datos.

bool mostrar = false;

void setup() 
{
  Serial.begin(9600);
  Serial.println("Iniciando.");

 }

 
void loop()
{
  // Si se reciven datos.
  if (mySerial.available() > 0)
  {

    mySerial.readBytesUntil('T', buffer, lonbuffer);    // Buscar en los datos recibidos el char 'T'.
    temperatura = mySerial.parseFloat();    // Almacenar los datos en una variable y transformarlos a float con una funcion del mySerial.

     
    mySerial.readBytesUntil('H', buffer, lonbuffer);    // Buscar en los datos recibidos el char 'H'.
    humedad = mySerial.parseFloat();    // Almacenar los datos en una variable y transformarlos a float con una funcion del mySerial.

  // Cambiar el estado de mostrar a true.
    mostrar = true;
  }

  // Si mostrar es true.
  if (mostrar)
  {
     
    // Imprimir los datos en el serial monitor del nodemcu.
    Serial.print("temp: ");
    Serial.println(temperatura);
    Serial.print("humedad: ");
    Serial.println(humedad);


    // Cambiar estado de mostrar y agregar un delay para evitar sobrecargar al dispositivo con datos.
    mostrar = false;
    delay (1000);
  }
  
}
