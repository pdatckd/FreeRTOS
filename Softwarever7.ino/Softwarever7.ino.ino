#include <PDLib.h>
#include <String.h>
PDLib HC01;
String RxData;
String RxDataA9G;

bool stringComplete = false;
void setup() {
  Serial.begin(250000);
  Serial1.begin(57600);
  Serial.println(F("In Setup function"));
  HC01.ConfigGPIO();
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  digitalWrite(11,HIGH);
  digitalWrite(10,HIGH);
  RxDataA9G.reserve(1000);
}

void loop(){
    if(stringComplete){
      Serial1.flush();
      Serial1.println(RxData);
      delay(1000);
      SerialEvent1();
      Serial.flush();
      for(int i = 0; i < RxDataA9G.length();i++){
        Serial.write(RxDataA9G[i]);
      }
      for(int i =  0; i <RxDataA9G.length();i++){
        if (RxDataA9G[i] == 'O'){
            if (RxDataA9G[i +1] == 'K'){
              stringComplete = false;
              Serial.print("\r\n");
              };
        }
      }
      }
}
void SerialEvent1(){
  RxDataA9G = "";
  while(Serial1.available()){
    char c = Serial1.read();
    if((c == '\n')||(c == '\r')){}
    else{
    RxDataA9G += c;
    }
  }
}
void serialEvent() {
  RxData = "";
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    RxData += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

