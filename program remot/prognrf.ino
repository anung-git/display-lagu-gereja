#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#define buzer 7
#include "Keypad.h"
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','K'},
  {'4','5','6','M'},
  {'7','8','9','P'},
  {'-','0','+','F'}
};
byte rowPins[ROWS] ={ 3, 2, A1,A2};//connect to the row pinouts of the keypad
byte colPins[COLS] =  {A5, A4, A3,4};  //connect to the column pinouts of the keypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

RF24 radio(A0, 9); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
boolean power = 0;
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 2000;           // interval at which to blink (milliseconds)

void setup() {
  pinMode(buzer, OUTPUT);
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  digitalWrite(buzer, HIGH);
  delay(150);
  digitalWrite(buzer, LOW);
}
void loop() {
  char customKey = customKeypad.getKey();
  if (customKey){
      if (millis() - previousMillis >= interval) {
        if(customKey>='0' &&customKey<='9'){
          char nol[]="0";
          radio.openWritingPipe(addresses[0]); // 00001
          delay(10);
          radio.write(&nol, 1);
          radio.write(&nol, 1);
          radio.write(&nol, 1);
          radio.write(&nol, 1);
          radio.openWritingPipe(addresses[1]); // 00001
          delay(10);
          radio.write(&nol, 1);
          radio.write(&nol, 1);
          radio.write(&nol, 1);
          radio.write(&nol, 1);        }
      }
      if(customKey>='0' &&customKey<='9')previousMillis=millis();
      if (customKey=='F'){
          if(power){
            customKey='F';
            power=0;
          }
          else {
            customKey='N';
            power=1;
          }
      }
      else power=1;
      radio.openWritingPipe(addresses[0]); // 00001
      delay(10);
      radio.write(&customKey, 1);
      radio.openWritingPipe(addresses[1]); // 00001
      delay(10);
      radio.write(&customKey, 1);
      digitalWrite(buzer, HIGH);
      delay(150);
      digitalWrite(buzer, LOW);
    }
}
