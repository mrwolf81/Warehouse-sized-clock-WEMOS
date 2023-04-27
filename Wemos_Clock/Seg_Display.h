#ifndef SEG_DISPLAY_H
#define SEG_DISPLAY_H
#include "Arduino.h"

byte g_digits [10];                  // Definitions of the 7-bit values for displaying digits
byte g_digits2 [10];
int g_numberToDisplay = 0;          // default number being displayed, 0 
const int g_registers = 4;          // Number of shift registers in use, 4
byte g_registerArray [g_registers];
/*
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
    g_digits [99] = 0;
    g_digits2 [0] = 1 + 8 + 4 + 16;                     //1 + 2 + 64 + 32;           // Degree dot
    g_digits2 [1] = 8 + 1 + 32 + 64;                    //1 + 8 + 16 + 32;           // Capital C
    g_digits2 [2] = 32 + 16;                            //64 + 16;                   //  r, 80
    g_digits2 [3] = 1 + 32 + 16 + 2;                    //4 + 16 + 32 + 64;           //  h, 116
*/
class Seg_Display{
  
  public:
  Seg_Display();
  Seg_Display(int dataPin, int clockPin, int latchPin);
  void sendSerialData();
  
  private:
  int _dataPin,_clockPin, _latchPin;
  void init(int dataPin, int clockPin, int latchPin);
 
};
#endif
