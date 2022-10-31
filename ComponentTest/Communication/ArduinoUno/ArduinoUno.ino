#include <SoftwareSerial.h>
#include "DHT.h"

#define DHTPIN 13     // Pin donde está conectado el sensor
#define rxPin 2 
#define txPin 3 

#define DHTTYPE DHT22   // Sensor DHT22

DHT dht(DHTPIN, DHTTYPE);
float h,t;
 
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
    if (isnan(h) || isnan(t))                         //Si no se lee valor numérico indicamos el fallo y volvemos a 
                                                              //comprobar.
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
    
    
    Trans.println('T'+ String(t)+'H'+ String(h)); //enviar datos en una cadena
                        
                                                           
}
