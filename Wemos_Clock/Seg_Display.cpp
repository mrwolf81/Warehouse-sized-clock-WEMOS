#include "Seg_Display.h"
#include <Arduino.h>



Seg_Display::Seg_Display(int dataPin, int clockPin, int latchPin){
  init(dataPin, clockPin, latchPin);
}

void Seg_Display::init(int dataPin, int clockPin, int latchPin){
  _dataPin = dataPin;
  _clockPin = clockPin;
  _latchPin = latchPin;

  pinMode(_dataPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);
  pinMode(_latchPin, OUTPUT);
}

void Seg_Display::sendSerialData((byte registerCount, byte *pValueArray)){
  digitalWrite (g_pinCommLatch, LOW);
  for (byte reg = registerCount; reg > 0; reg--)
  {
    byte value = pValueArray [reg - 1];
    for (byte bitMask = 128; bitMask > 0; bitMask >>= 1)
    {
      digitalWrite (g_pinClock, LOW);
      digitalWrite (g_pinData, value & bitMask ? HIGH : LOW);
      digitalWrite (g_pinClock, HIGH);
    }
  }
  digitalWrite (g_pinCommLatch, HIGH); 
}

/*
void digitalClockDisplay(){
  Serial.print(hour());
  Serial.print(':');
  char buf[2];
  sprintf(buf,"%2d",minute());
  Serial.print(buf);
  Serial.print(':');
  Serial.print(second());
  Serial.println();
  
  // digital clock display of the time
   g_registerArray [0] = g_digits [hourFormat12() / 10];
   g_registerArray [1] = g_digits [hourFormat12() % 10];
   g_registerArray [2] = g_digits [minute() / 10];
   g_registerArray [3] = g_digits [minute() % 10];

   sendSerialData (g_registers, g_registerArray);
   digitalWrite(dot1, HIGH);
   digitalWrite(dot2, HIGH);
   delay(500);
   digitalWrite(dot1, LOW);
   digitalWrite(dot2, LOW);
   delay(500);
}

void digitalDateDisplay(){

  Serial.print(day());
  Serial.print(':');
  Serial.print(month());
  Serial.print(':');
  Serial.print(year());
  Serial.println();

  g_registerArray [0] = g_digits [day() / 10];
  g_registerArray [1] = g_digits [day() % 10];
  g_registerArray [2] = g_digits [month() / 10];
  g_registerArray [3] = g_digits [month() % 10];

  sendSerialData (g_registers, g_registerArray);

  digitalWrite(dot1, HIGH);
  digitalWrite(dot2, HIGH);
  delay(500);
  digitalWrite(dot1, LOW);
  digitalWrite(dot2, LOW);
  delay(500);
}

void digitalYearDisplay(){
  
  g_registerArray [0] = g_digits [year() / 1000];
  g_registerArray [1] = g_digits [(year() % 1000) / 100];
  g_registerArray [2] = g_digits [(year() % 100) / 10];
  g_registerArray [3] = g_digits [year() % 10];

  sendSerialData (g_registers, g_registerArray);

  digitalWrite(dot1, HIGH);
  digitalWrite(dot2, HIGH);
  delay(500);
  digitalWrite(dot1, LOW);
  digitalWrite(dot2, LOW);
  delay(500);
}

void digitalTempDisplay(){
  
  Serial.print(temp);
  Serial.println();
  
  temp = dht.readTemperature();
  
  g_registerArray [0] = g_digits [temp / 10];
  g_registerArray [1] = g_digits [temp % 10];
  g_registerArray [2] = g_digits2 [0];
  g_registerArray [3] = g_digits2 [1];

  sendSerialData (g_registers, g_registerArray);

  digitalWrite(dot1, HIGH);
  digitalWrite(dot2, HIGH);
  delay(500);
  digitalWrite(dot1, LOW);
  digitalWrite(dot2, LOW);
  delay(500);
}

void digitalHumidDisplay(){
  
  Serial.print(humid);
  Serial.println();

  humid = dht.readHumidity();
  
  g_registerArray [0] = g_digits [humid / 10];
  g_registerArray [1] = g_digits [humid % 10];
  g_registerArray [2] = g_digits2 [2];
  g_registerArray [3] = g_digits2 [3];

  sendSerialData (g_registers, g_registerArray);

  digitalWrite(dot1, HIGH);
  digitalWrite(dot2, HIGH);
  delay(500);
  digitalWrite(dot1, LOW);
  digitalWrite(dot2, LOW);
  delay(500);
}
*/



 
