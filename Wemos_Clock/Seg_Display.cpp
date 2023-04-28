#include "Seg_Display.h"
#include <Arduino.h>
#include "DHT.h"
#include <TimeLib.h>

#define DHTPIN D5
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

byte g_digits [10];                  // Definitions of the 7-bit values for displaying digits
byte g_digits2 [10];                // 2nd set Definitions of the 7-bit values for displaying digits
int g_numberToDisplay = 0;          // default number being displayed, 0 
const int g_registers = 4;          // Number of shift registers in use, 4
byte g_registerArray [g_registers]; //send data array through register count
int temp = 0;
int humid = 0;
Seg_Display Display(byte registerCount, byte *pValueArray);

//this section for pin and bit mask of digits accoring to my wiring setup (currently errors in compiling)

int a = 1, b = 2, c = 4, d = 8, e = 16, f = 32, g = 64;

    g_digits [0] = 8 + 4 + 2 + 64 + 32 + 1;             //1 + 2 + 4 + 8 + 16 + 32;
    g_digits [1] = 4 + 2;                               //2 + 4;
    g_digits [2] = 8 + 4 + 16 + 32 + 64;                //1 + 2 + 64 + 16 + 8;
    g_digits [3] = 8 + 4 + 16 + 2 + 64;                 //1 + 2 + 4 + 8 + 64;
    g_digits [4] = 1 + 16 + 4 + 2;                      //32 + 64 + 2 + 4;
    g_digits [5] = 8 + 1 + 16 + 2 + 64;                 //1 + 32 + 64 + 4 + 8;
    g_digits [6] = 8 + 1 + 16 + 2 + 64 + 32;            //1 + 32 + 16 + 8 + 4 + 64;
    g_digits [7] = 8 + 4 + 2;                           //1 + 2 + 4;
    g_digits [8] = 1 + 2 + 4 + 8 + 16 + 32 + 64;        //1 + 2 + 4 + 8 + 16 + 32 + 64;
    g_digits [9] = 1 + 8 + 4 + 16 + 2 + 64;             //8 + 4 + 2 + 1 + 32 + 64;
    g_digits [99] = 0;                                  // blank digits
    g_digits2 [0] = 1 + 8 + 4 + 16;                     //1 + 2 + 64 + 32;           // Degree dot
    g_digits2 [1] = 8 + 1 + 32 + 64;                    //1 + 8 + 16 + 32;           // Capital C
    g_digits2 [2] = 32 + 16;                            //64 + 16;                   //  r, 80
    g_digits2 [3] = 1 + 32 + 16 + 2;                    //4 + 16 + 32 + 64;           //  h, 116
    


//this section for sending information to the registers

void Seg_Display::Display(byte registerCount, byte *pValueArray){
  digitalWrite (_latchPin, LOW);
  for (byte reg = registerCount; reg > 0; reg--)
  {
    byte value = pValueArray [reg - 1];
    for (byte bitMask = 128; bitMask > 0; bitMask >>= 1)
    {
      digitalWrite (_clockPin, LOW);
      digitalWrite (_dataPin, value & bitMask ? HIGH : LOW);
      digitalWrite (_clockPin, HIGH);
    }
  }
  digitalWrite (_latchPin, HIGH);
}


//this section to initialise data, clock and latch pins as outputs

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


//this section to get time information and update "Display" data to send to registers

void Seg_Display(){

time_t prevDisplay = 0;
  
  if (timeStatus() != timeNotSet) {
    if (now() != prevDisplay) { //update the display only if time has changed
      prevDisplay = now();
      if ((second() == 00) || (second() == 15) ||(second() == 30) ||(second() == 45)){
        
//        display temperature

        g_registerArray [0] = g_digits [temp / 10];
        g_registerArray [1] = g_digits [temp % 10];
        g_registerArray [2] = g_digits2 [0];
        g_registerArray [3] = g_digits2 [1];
        Display;
        delay (100);
        
//        display humidity
        
        g_registerArray [0] = g_digits [humid / 10];
        g_registerArray [1] = g_digits [humid % 10];
        g_registerArray [2] = g_digits2 [2];
        g_registerArray [3] = g_digits2 [3];
        Display;
        delay (100);
      
      }else{
        
//        display time
        
        g_registerArray [0] = g_digits [hourFormat12() / 10];
        g_registerArray [1] = g_digits [hourFormat12() % 10];
        g_registerArray [2] = g_digits [minute() / 10];
        g_registerArray [3] = g_digits [minute() % 10];
      }
    }
  }
}

 
