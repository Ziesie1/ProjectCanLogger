#include <Arduino.h>
#include "can.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0){
    char x=Serial.read();
    Serial.print("char recieved: ");
    Serial.println(x);
    Canmsg msg{};
    msg.PrintSerial();
    //Serial.println(static_cast<String>(msg)); //soll später PrintSerial ersetzen
  }
}