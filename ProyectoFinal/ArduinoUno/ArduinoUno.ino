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
int MQ135Pin = A3; //Sensor MQ135


#define DHTTYPE DHT22   // Sensor DHT22

DHT dht(DHTPin, DHTTYPE);
//Declarar variables 
float h,t;
int ht, CO2, CO;
 
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
    ht = analogRead(A4);    //Leer humedad de tierra
    CO2 = analogRead(A3);   //leer CO2
    CO = analogRead(A5);    //leer CO
    if (isnan(h) || isnan(t))                         //Si no se lee valor numérico indicamos el fallo y volvemos a 
                                                              //comprobar.
    {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    else if (isnan(ht))
    {
        Serial.println("Failed to read from Earth sensor!");
      return;
    }
    else if (isnan(CO2))
    {
        Serial.println("Failed to read from MQ135 sensor!");
      return;
    }
    else if (isnan(CO))
    {
        Serial.println("Failed to read from MQ7 sensor!");
      return;
    }
    


    Trans.println('T'+ String(t)+'H'+ String(h) + 'CO' + String(CO) + 'CO2' String(CO2) + 'Humedad tierra' + String(ht)); //enviar datos en una cadena

    /*
    Serial.print("Temperatura: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print("Humedad: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print(" \n");
    */
    
    
                        
                                                           
}
