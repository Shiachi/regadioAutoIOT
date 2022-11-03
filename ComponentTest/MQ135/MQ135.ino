int sensorValue;

void setup(){
Serial.begin(9600);                            // sets the serial port to 9600
 }
void loop(){sensorValue = analogRead(A3);       // read analog input pin 0
Serial.print("AirQua=");
Serial.print(sensorValue, DEC);               // prints the value read
Serial.println(" PPM");
delay(2000);
}