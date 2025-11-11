#ifndef screen__H
#define screen__H

#include <Arduino.h>
#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_ST7796S.h>
#include <Adafruit_ST77xx.h>
#include <stream.h>
#include <stdint.h> 

#define green ST7735_GREEN
#define black ST7735_BLACK
#define red ST7735_RED
#define white ST7735_WHITE
extern int _trap;
extern int  tmp; 
extern int check ;
extern int TFT_CS;
extern int TFT_RST;
extern int TFT_DC;
class screen{
    public : 
    static void init(void);
    static void _solve(void);
    static void __int_to_binary(int n, int a[], int *k) ;
    static void gach_chan(int x, int y);
};

#endif