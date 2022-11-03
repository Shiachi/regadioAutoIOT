int SensorPin = A4;
void setup(){
    Serial.begin(9600);
}
void loop(){
    int humedad = analogRead(SensorPin);
    Serial.print(humedad);
    delay(5000);
}