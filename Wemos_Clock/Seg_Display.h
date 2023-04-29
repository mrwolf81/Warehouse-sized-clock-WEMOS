#ifndef SEG_DISPLAY_H
#define SEG_DISPLAY_H
#include "Arduino.h"

class Seg_Display{
  
  public:
  Seg_Display();
  Seg_Display(int dataPin, int clockPin, int latchPin);
  void Display(byte registerCount, byte *pValueArray); //compiling errors
  void g_digits(int a = 1,int b = 2,int c = 4,int d = 8,int e = 16,int f = 32,int g = 64);
  
  
  private:
  int _dataPin,_clockPin, _latchPin;
  void init(int dataPin, int clockPin, int latchPin);
  int a = 1, b = 2, c = 4, d = 8, e = 16, f = 32, g = 64;

};
#endif
