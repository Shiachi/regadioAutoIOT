//Incluir librerias
#include "DHT.h"  
#include <SoftwareSerial.h>


//definir pines y tipo de sensores
#define DHTPin 13 
#define RELE 12
#define rxPin 2       
#define txPin 3 //salida
#define DHTTYPE DHT22

DHT dht(DHTPin, DHTTYPE);
SoftwareSerial Trans(rxPin, txPin);               //Pines para Rx y Tx.

//definicion de pines con valores  para ocupar en logica
float SensorMQ135 = A3;
int SensorHum = A4;
float SensorMQ7 = A5;

//declarar variables para logica
const int ONF = 1000;    //Tiempo encendido
float a, c, t, h; //   a=MQ135 //c=MQ7 //t=Dht22 temperatura //h=dht22 humedad
int b; //b=Humedad de tierra


void setup(){
    Serial.begin(9600);
    pinMode(RELE, OUTPUT);//Define el pin RELE como salida
    digitalWrite(RELE, HIGH);//Rele inicia apagado

    Trans.begin(9600);
    dht.begin();
    a,b,c,t,h = 0;

}
void loop(){
    delay(2000);   

    a = analogRead(SensorMQ135); //lectura de la humedad de la tierra
    b = analogRead(SensorHum);
    c = analogRead(SensorMQ7);

    t = dht.readTemperature(); //Leer temperatura en grados celcius dht22
    h = dht.readHumidity(); //leer humedad dht22

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
    if (isnan(b))
    {
      Serial.println("Failed to read from Earth sensor!");
    }
    else
    {
      Serial.print("Humedad de la tierra: ");
      Serial.print(b);
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
    //lectura de MQ135
    if (isnan(a))
    {
      Serial.println("Failed to read from MQ135 sensor!");
    }
    else
    {
      Serial.print("calidad de aire: ");
      Serial.println(a);
    }

    Serial.print(" \n");

    Trans.println('A'+ String(a)+'E'+ String(b) + 'C' + String(c) + 'H' + String(h) + 'T' + String(t)); 
    //A = calidad del aire  // E = Humedad de la tierra // C = CO   // H = Humedad ambiente // T = Temperatura ambiente

    /*Logica de riego desactivada para que no se active
    if (humedad <  500){
        digitalWrite(RELE, LOW);//Enciende el rele
        delay(ONF);//Espera el tiempo de llenado 
        digitalWrite(RELE, HIGH);//Apaga el rele
    }
    else
    {
        digitalWrite(RELE, HIGH);//Apaga el rele
        delay(ONF);
    }
    */
}
