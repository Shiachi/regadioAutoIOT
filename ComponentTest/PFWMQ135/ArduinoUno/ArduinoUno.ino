#include <SoftwareSerial.h>
#include "DHT.h"

//definir uso de pines digitales
#define DHTPin 13     //Pin dth
#define RelePin 12    //Pin rele

//pines de comunicacion
#define rxPin 2       
#define txPin 3 //salida

//definir uso de sensores analogicos
int MQ7Pin = A5; //Sensor MQ7
int HumPin = A4; //Sensor humedad tierra


#define DHTTYPE DHT22   // Sensor DHT22

DHT dht(DHTPin, DHTTYPE);
//Declarar variables 
float h,t, c, e;
 
//
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

    //Revisar sensor humedad y temperatura DHT22 revisa  que no este vacio e imprime datos
    if (isnan(h) || isnan(t))                                                                 
    {
      Serial.println("Failed to read from DHT sensor!");
    }else
    {
      Serial.println("Uno");
      Serial.print("Temperatura: ");
      Serial.print(t);
      Serial.println(" *C ");
      Serial.print("Humedad: ");
      Serial.print(h);
      Serial.println(" %");
    }

    //Revisa sensor de humedad de la tierra revisa  que no este vacio e imprime datos
    if (isnan(e))
    {
      Serial.println("Failed to read from Earth sensor!");
    }
    else
    {
      Serial.print("Humedad de la tierra: ");
      Serial.print(e);
      Serial.println(" %");
    }

    //Revisa sensor MQ7 encargador de analizar CO revisa  que no este vacio e imprime datos
    if (isnan(c))
    {
      Serial.println("Failed to read from MQ7 sensor!");
    }
    else
    {
      Serial.print("CO: ");
      Serial.println(c);
    }

    Serial.print(" \n");
    
    //transfiere los datos con un char identificador
    Trans.println('T'+ String(t)+'H'+ String(h) + 'C' + String(c) + 'E' + String(e)); //enviar datos en una cadena
}