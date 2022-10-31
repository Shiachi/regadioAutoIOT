#include "DHT.h"

#define DHTPIN 8     // Pin donde está conectado el sensor

#define DHTTYPE DHT22   // Sensor DHT22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciando...");
  dht.begin();
}
void loop() {
  delay(6000);
  float h = dht.readHumidity(); //leer humedad
  float t = dht.readTemperature(); //Leer temperatura en grados celcius
  //muestreo de datos en puerto serial 9600
  Serial.print("Humedad ");
  Serial.print(h);
  Serial.print(" %t");
  Serial.print("\t");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print('\n'); //salto de linea
}