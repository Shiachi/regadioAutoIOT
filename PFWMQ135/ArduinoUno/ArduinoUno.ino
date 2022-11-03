#include <SoftwareSerial.h>
#include "DHT.h"

//definir uso de pines digitales
#define DHTPin 13     //Pin dth
#define RelePin 12    //Pin rele
#define rxPin 2 
#define txPin 3 
//definir uso de sensores analogicos
int MQ7Pin = A5; //Sensor MQ7
int HumPin = A4; //Sensor humedad tierra


#define DHTTYPE DHT22   // Sensor DHT22

DHT dht(DHTPin, DHTTYPE);
//Declarar variables 
float h,t, c, e;
 
SoftwareSerial Trans(rxPin, txPin);               //Pines para Rx y Tx.

void setup()
{
  pinMode (txPin , OUTPUT);
  Serial.begin(9600);
  Trans.begin(9600);                            //Velocidad de envío para la transmisión al NodeMCU.
  dht.begin();
}

void loop()
{
  
    delay(2000);                                  
    h = dht.readHumidity();                      //Leemos humedad y temperatura respectivamente.
    t = dht.readTemperature();
    c = analogRead(A5);    //leer CO
    e = analogRead(A4);    //Leer humedad de tierra
    if (isnan(h) || isnan(t))                         //Si no se lee valor numérico indicamos el fallo y volvemos a 
                                                              //comprobar.
    {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    if (isnan(e))
    {
        Serial.println("Failed to read from Earth sensor!");
      return;
    }
    if (isnan(c))
    {
        Serial.println("Failed to read from MQ7 sensor!");
      return;
    }
    Serial.println("Uno");
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.println(" *C ");
    Serial.print("Humedad: ");
    Serial.print(h);
    Serial.println(" %");
    Serial.print("CO: ");
    Serial.println(c);
    Serial.print("Humedad de la tierra: ");
    Serial.print(e);
    Serial.println(" %");
    
    Serial.print(" \n");



    Trans.println('T'+ String(t)+'H'+ String(h) + 'C' + String(c) + 'E' + String(e)); //enviar datos en una cadena
}