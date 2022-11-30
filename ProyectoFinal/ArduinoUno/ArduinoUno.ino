  //Incluir librerias
  #include "DHT.h"  
  #include <SoftwareSerial.h>


  //definir pines y tipo de sensores
  #define DHTPin 13 
  #define rxPin 2       
  #define txPin 3 //salida
  #define DHTTYPE DHT22

  DHT dht(DHTPin, DHTTYPE);
  SoftwareSerial Trans(rxPin, txPin);               //Pines para Rx y Tx.

  //definicion de pines con valores para ocupar en logica
  float SensorMQ135 = A3;
  int SensorHum = A4;
  float SensorMQ7 = A5;

  //declarar variables para logica
  float t, h;  //t=Dht22 temperatura //h=dht22 humedad
  int a, b, c; //a=MQ135  //b=Humedad de tierra //c=MQ7


  void setup(){
      Serial.begin(9600);
      Trans.begin(9600);
      dht.begin();
      a,b,c,t,h = 0;

  }
  void loop(){
      delay(2000);   

      a = analogRead(SensorMQ135);
      b = analogRead(SensorHum);  //lectura de la humedad de la tierra
      c = analogRead(SensorMQ7);

      t = dht.readTemperature(); //Leer temperatura en grados Celsius dht22
      h = dht.readHumidity(); //Leer humedad dht22

      //Revisa sensor humedad y temperatura DHT22 revisa que no este vacío e imprime datos
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
      //Revisa sensor de humedad de la tierra revisa que no este vacío e imprime datos
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

      //Revisa sensor MQ7 encargado de analizar CO revisa que no esté vacío e imprime datos
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
  }
