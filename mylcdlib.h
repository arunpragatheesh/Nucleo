#ifndef MYLCDLIB_H
#define MYLCDLIB_H

#include <Arduino.h>
#include <Wire.h>
#include "DFRobot_RGBLCD1602.h"

enum ParamIndex {
    IDX_OP_FREQ = 0,  // 0
    IDX_DUTY,         // 1 (automatically)
    IDX_A,            // 2
    IDX_B,            // 3
    IDX_C,            // 4
    IDX_D,            // 5
    IDX_E,            // 6
    IDX_F,            // 7
    IDX_SETTINGS,     //8
    // ... add all 40 here ...
    IDX_MAX_PARAMS = 40
};

enum Lcd_Page_Controls{
NEGATIVE_EDITING_VALUE,
INCREASE_EDITING_VALUE,
DECREASE_EDITING_VALUE,
DISPLAY_CONTROL_PAGE_UP,
DISPLAY_CONTROL_PAGE_DN,
DISPLAY_CONTROL_ON_OFF,
DISPLAY_CONTROL_ENTER,
DISPLAY_CONTROL_ESCAPE,
DISPLAY_CONTROL_CURSOR
};

#define    KEY_PRE_UP    1
#define    KEY_PRE_DN    2
#define    KEY_PRE_LT    3
#define    KEY_PRE_RT    4
#define    KEY_SELECT    5
#define    KEY_RESET     6
#define    KEY_UNKNOWN   7

class MyLcdLib {
  public:
    // Constructor
    MyLcdLib(uint8_t rgbAddr = 0x2D, uint8_t cols = 16, uint8_t rows = 2);

    void assignDataBuffer(float* externalArray);

    // Your exact API names
    void begin();
    void manageUI();

    void lcdPrintDoublePage(int PageNo, float v1, float v2);

  private:
    // The driver instance is now a private member of the class
    DFRobot_RGBLCD1602 _lcd;
    

    byte readButtons();

    // Constants moved inside the class
    const int _colorR = 255;
    const int _colorG = 255;
    const int _colorB = 255;

    // Bit positions
    const byte B_SELECT = 0;
    const byte B_UP     = 1;
    const byte B_DOWN   = 2;
    const byte B_LEFT   = 3;
    const byte B_RIGHT  = 4;

    const int _selectButton = A5;
    const int _btnUp     = A4; 
    const int _btnDown   = A3; 
    const int _btnLeft   = A2; 
    const int _btnRight  = A1;


    // Buffers and tracking
    char _LCD_Buffer[21];
    int _prevPage = -1;
    byte _buttonState;
    int _idxMenu=0;

    const int _firstPage[4] ={0, 10, 15, 25};
    const int _lastPage[4] = {3, 10, 15, 25};
    int _idxPage=0,  labelIdx1, labelIdx2;

    float* _data = nullptr; // Pointer to the data in your .ino
    int _activePage=0;
    byte Key_Prev=-1, EditingActive=0;
};

#endif