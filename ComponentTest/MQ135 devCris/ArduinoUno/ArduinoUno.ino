#include <SoftwareSerial.h>

//Declarar variables 
float m; //float to read mq135

#define rxPin 2       
#define txPin 3 //salida
 
//
SoftwareSerial Trans(rxPin, txPin);               //Pines para Rx y Tx.

void setup()
{
  Serial.begin(9600);
  Trans.begin(9600);
  m=0;                           //Velocidad de envío para la transmisión al NodeMCU.
}

void loop()
{
    delay(2000);   
    //Normal air returns approximately 100-150 ppm
    //bajo es peligroso para las plantas ya que se ayudan con este gas a producir la fotosintesis  
    m = analogRead(A0);

    if (isnan(m))
    {
      Serial.println("Failed to read from MQ135 sensor!");
    }
    else
    {
      Serial.print("calidad de aire: ");
      Serial.println(m);
    }

    Serial.print(" \n");
    
    //transfiere los datos con un char identificador
    Trans.println('M'+ String(m)); //enviar datos en una cadena
}