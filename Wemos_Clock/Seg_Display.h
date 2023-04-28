#ifndef SEG_DISPLAY_H
#define SEG_DISPLAY_H
#include "Arduino.h"

class Seg_Display{
  
  public:
  Seg_Display();
  Seg_Display(int dataPin, int clockPin, int latchPin);
  void Display(byte registerCount, byte *pValueArray); //compiling errors
  
  private:
  int _dataPin,_clockPin, _latchPin;
  void init(int dataPin, int clockPin, int latchPin);

};
#endif
