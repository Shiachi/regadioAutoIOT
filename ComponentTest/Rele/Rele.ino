#define RELE 7
const int ONF = 2000;    // Tiempo encendido
const int CD = 30000;    // Tiempo apagado

void setup() {
  pinMode(RELE, OUTPUT);// Define el pin RELE como salida
  digitalWrite(RELE, HIGH);//Rele inicia apagado

}

void loop() {
  digitalWrite(RELE, LOW);// Enciende el rele
  delay(ONF);// Espera el tiempo de llenado 
  digitalWrite(RELE, HIGH);// Apaga el rele
  delay(CD);// Tiempo de espera antes de repetir el proceso
}