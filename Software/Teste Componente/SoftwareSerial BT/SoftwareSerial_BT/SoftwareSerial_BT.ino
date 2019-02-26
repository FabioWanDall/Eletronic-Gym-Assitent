#include <SoftwareSerial.h>
SoftwareSerial BT(10, 11); //RX, TX

void setup() {
  Serial.begin(9600);
  Serial.println("Mensagem via serial...");
  BT.begin(9600); //HC-05 usually default baud-rate

}

void loop() {
  // put your main code here, to run repeatedly:
  BT.println("A");

}
