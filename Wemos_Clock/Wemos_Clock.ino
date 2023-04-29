#include "secrets.h"
#include "Seg_Display.h"


const int dot1 = D7; //column dot 1
const int dot2 = D8; //column dot 2

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
    g_digits2 [0] = 1 + 8 + 4 + 16;                     //1 + 2 + 64 + 32;           // Degree dot
    g_digits2 [1] = 8 + 1 + 32 + 64;                    //1 + 8 + 16 + 32;           // Capital C
    g_digits2 [2] = 32 + 16;                            //64 + 16;                   //  r, 80
    g_digits2 [3] = 1 + 32 + 16 + 2;                    //4 + 16 + 32 + 64;           //  h, 116
    g_digits [99] = 0;
    */


void setup(){

}
void loop(){
  Seg_Display();
}
