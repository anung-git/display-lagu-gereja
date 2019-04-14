#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "DMD.h"
#include "Arial_black_16.h"
DMD dmd(2, 1);
RF24 radio(A0, 8); // CE, CSN
const byte address[6] = "00001";
char charBuf[4]= {'0', '0', '0', '0'};
char tp='K';

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 2000;           // interval at which to blink (milliseconds)

void setup() {
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  dmd.clearScreen( true );  
  terima_data('K');
  radio.startListening();
  pinMode(9, OUTPUT);
  analogWrite(9,100);  
  }
void loop() {
  if (radio.available()){
    char radio_in;
    radio.read(&radio_in, sizeof(radio_in));
    terima_data(radio_in);
  }
  if (millis() - previousMillis >= interval) {
      for (byte scn=0; scn<8; scn++) dmd.scanDisplayBySPI();
  }
  else {
      int kdp =millis()%200;
      if (kdp<100) for (byte scn=0; scn<8; scn++) dmd.scanDisplayBySPI();
      else OE_DMD_ROWS_OFF();
  }
}
void terima_data(char masuk){
     if(masuk>='0' && masuk<='9'){
       charBuf[0]=charBuf[1];
       charBuf[1]=charBuf[2];
       charBuf[2]=charBuf[3];
       charBuf[3]=masuk;
     }
     if (masuk=='K' || masuk=='M' || masuk=='P' ) tp=masuk;
     if (masuk=='+'){
        charBuf[3]++;
        if(charBuf[3]>'9'){
          charBuf[3]='0';
          charBuf[2]++;
        }
        if(charBuf[2]>'9'){
          charBuf[2]='0';
          charBuf[1]++;
        }
        if(charBuf[1]>'9'){
          charBuf[1]='0';
          charBuf[0]++;
        }
        if(charBuf[0]>'9')charBuf[0]='0';
     }
     if (masuk=='-'){
        charBuf[3]--;
        if(charBuf[3]<'0'){
          charBuf[3]='9';
          charBuf[2]--;
        }
        if(charBuf[2]<'0'){
          charBuf[2]='9';
          charBuf[1]--;
        }
        if(charBuf[1]<'0'){
          charBuf[1]='9';
          charBuf[0]--;
        }
        if(charBuf[0]<'0')charBuf[0]='9';
     }
     dmd.clearScreen( true );
     dmd.selectFont(Arial_Black_16);
     if (tp=='P')dmd.drawString(0 , 0, "PJ", 2 ,GRAPHICS_NORMAL);
     if (tp=='K')dmd.drawString(0 , 0, "KA", 2 ,GRAPHICS_NORMAL);
     if (tp=='M')dmd.drawString(0 , 0, "MD", 2 ,GRAPHICS_NORMAL);  
     dmd.drawString(25 , 0, charBuf, 4 ,GRAPHICS_NORMAL);    
     if (masuk=='F')dmd.clearScreen( true );
     previousMillis=millis();  
}

