/*
      cosas para tener en cuenta
      1. al subir el codigo los cables RX y TX tienen que estar desconectados.
*/

#include <SoftwareSerial.h>   // Libreria necesaria para la comunicacion.
#include "DHT.h"

#define DHTPIN 13   // Pin donde está conectado el sensor dht22.
#define rxPin 2   // Establecer a pin 2 como rx.
#define txPin 3   // Establecer a pin 3 como tx.
// Evitar ocupar pines 0 y 1 para prevenir bugs.

#define DHTTYPE DHT22   // Definir tipo de sensor.

DHT dht(DHTPIN, DHTTYPE);


float h,t;    // Definir variables que ocupara el sensor para almacenar datos.
 
SoftwareSerial Trans(rxPin, txPin);   // Pines para Rx y Tx.

void setup()
{
  pinMode (txPin , OUTPUT);   // Declarar txPin como salida.
  Serial.begin(9600);
  Trans.begin(9600);    //Velocidad de transferencia de datos.
  dht.begin();
}

void loop()
{
  
    delay(2000);    // Tiempo de espera
    h = dht.readHumidity();   // Leer humedad y temperatura y almacenar los datos en una variable.
    t = dht.readTemperature();
    if (isnan(h) || isnan(t))   //Si no se lee valor numérico indicamos el fallo y volvemos a comprobar.
    {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print("Humedad: ");
    Serial.print(h);
    Serial.print(" %\t");
    
    Serial.print(" \n");
    // Se imprimen los datos en monitor serial del arduino para pruebas.
    
    
    Trans.println('T'+ String(t)+'H'+ String(h)); 
    // Enviar datos en una cadena.
    // 'T' es el char identificador para que el receptor busque el dato en la memoria.
    // t es el parametro que recibe la funcion para enviar el dato (este se pasa a string).
    // Para evitar bugs es importante ocupar el mismo formato ('T' + String(t)).
    // Para concatenar se ocupa el simbolo +
}
