#include "MyLcdLib.h"

// PROGMEM tables stay outside the class definition
unsigned char const DoublePageTable[20][17] PROGMEM = {
    "O/P Freq:    kHz",
    "Duty Cyc:     % ",
    " Dummy1 :    kHz",
    " Dummy2 :     % ",
    " Dummy3 :    kHz",
    " Dummy4 :     % ",
    " Dummy5 :    kHz",
    " Dummy6 :     % ",
    "    SETTINGS    ",
    "                "
};

// PROGMEM tables stay outside the class definition
unsigned char const SinglePageTable[100][21]  PROGMEM =
{
"    Dead Time   ""n000",
"  ADC Channel 0 ""0110",
"  ADC Channel 1 ""0110",
"  ADC Channel 2 ""0110",
"  ADC Channel 3 ""0110",
"  ADC Channel 4 ""0110",
"  ADC Channel 5 ""0110"
};

/*
 
n = Numeric
N = Numeric Edit
t = Text
T = Text Edit

if Numeric Number
    0 = 0 digit number
    1 = 1 digit number
    2 = 2 digit number
    3 = binary number
    4 = hexa number
else if Text
    0 = On /Off
    1 = Fwd/Rev

0 = =32767 to 32767
1 = 0 to 100
2 = -100 to 100
3 = 0 to 1

0 = No unit
1 = %
2 = rpm
3 = ms
4 = Nm
*/

unsigned char const UnitTable[4][4]  PROGMEM =
{
"   ",
" % ",
"rpm",
" ms"  
};

unsigned char const ValueText[2][17]  PROGMEM =
{
"   Off  ""   On   ",
"   Rev  ""   Fwd  ",
};


// Constructor: Initializes the DFRobot_RGBLCD1602 instance
MyLcdLib::MyLcdLib(uint8_t rgbAddr, uint8_t cols, uint8_t rows) : _lcd(rgbAddr, cols, rows) {
    
}

void MyLcdLib::begin() {
  Wire.begin();
  delay(100);

  pinMode(_selectButton, INPUT_PULLUP);
  pinMode(_btnUp, INPUT_PULLUP);
  pinMode(_btnDown, INPUT_PULLUP);
  pinMode(_btnLeft, INPUT_PULLUP);
  pinMode(_btnRight, INPUT_PULLUP);

  _lcd.init();
  _lcd.clear();

  _lcd.setRGB(_colorR, _colorG, _colorB);
  _lcd.setCursor(0, 0);
  _lcd.print("     WELCOME    ");  
  delay(1000);
}

void MyLcdLib::assignDataBuffer(float* externalArray) {
    _data = externalArray; // No copying! Just storing the address.
}

void MyLcdLib::lcdPrintDoublePage(int PageNo, float v1, float v2) {
  if (PageNo != _prevPage) {
      _prevPage = PageNo;
      _lcd.clear();     
      
      _lcd.setCursor(0, 0);
      strcpy_P(_LCD_Buffer, (PGM_P)DoublePageTable[labelIdx1]);
      _lcd.print(_LCD_Buffer);
      
      _lcd.setCursor(0, 1);
      strcpy_P(_LCD_Buffer, (PGM_P)DoublePageTable[labelIdx2]);
      _lcd.print(_LCD_Buffer);
  }

  _lcd.setCursor(9, 0);
  _lcd.print(v1, 1);

  _lcd.setCursor(9, 1);
  _lcd.print(v2, 1);
}



byte MyLcdLib::readButtons() {
    byte status = 0;

    // Read each pin and write it to the specific bit
    bitWrite(status, B_SELECT, digitalRead(_selectButton));
    bitWrite(status, B_UP,     digitalRead(_btnUp));
    bitWrite(status, B_DOWN,   digitalRead(_btnDown));
    bitWrite(status, B_LEFT,   digitalRead(_btnLeft));
    bitWrite(status, B_RIGHT,  digitalRead(_btnRight));

    return status; 
    
}

void MyLcdLib::manageUI() {

  byte Key_Command, Key;
  char Lcd_Page_Control = 255;

  byte allButtons = readButtons();

  // Example: Sending over Serial
  //Serial.print("Buttons: ");
  //Serial.println(allButtons, BIN);

  if ( allButtons == 0b1 ) Key_Command = KEY_SELECT;
  else if ( allButtons == 0b10 ) Key_Command = KEY_PRE_UP;
  else if ( allButtons == 0b100 ) Key_Command = KEY_PRE_DN;
  else if ( allButtons == 0b1000 ) Key_Command = KEY_PRE_LT;
  else if ( allButtons == 0b10000 ) Key_Command = KEY_PRE_RT;
  else Key_Command = KEY_UNKNOWN;

  if(Key_Prev != Key_Command) 
  {
      Key = Key_Command;
      Key_Prev = Key_Command;
  }
  else 
  {
      Key = 0;
  }    
  


  switch(Key)
  {
    case KEY_PRE_UP : 
      if(EditingActive)
        Lcd_Page_Control = INCREASE_EDITING_VALUE;
      else
        Lcd_Page_Control = DISPLAY_CONTROL_PAGE_UP; 
      break;
                      
    case KEY_PRE_DN : 
      if(EditingActive)
        Lcd_Page_Control = DECREASE_EDITING_VALUE;
      else
        Lcd_Page_Control = DISPLAY_CONTROL_PAGE_DN; 
      break;     
                                                      
    case KEY_SELECT  : Lcd_Page_Control = DISPLAY_CONTROL_ENTER; break; 
    case KEY_PRE_RT : Lcd_Page_Control = DISPLAY_CONTROL_CURSOR; break;                                     
    case KEY_PRE_LT : Lcd_Page_Control = DISPLAY_CONTROL_ESCAPE; break; 
      
    //case KEY_RESET : Lcd_Page_Control = DISPLAY_CONTROL_ON_OFF; break;                     
    default:       break;
  }

  switch(Lcd_Page_Control)
  {
    case DISPLAY_CONTROL_PAGE_UP: 
      _idxPage++; 
      if(_idxPage>_lastPage[_idxMenu]) _idxPage= _firstPage[_idxMenu]; 

      break;
                    
    case DISPLAY_CONTROL_PAGE_DN: 
      _idxPage--;
      if(_idxPage<_firstPage[_idxMenu]) _idxPage= _lastPage[_idxMenu];
      break;   

    case DISPLAY_CONTROL_ON_OFF: 
      break;
                        
    case DISPLAY_CONTROL_ENTER:  

      break;
                    
    case DISPLAY_CONTROL_ESCAPE:  

      break;
                
    case INCREASE_EDITING_VALUE : 

      break; 
                              
    case DECREASE_EDITING_VALUE : 

      break;

    case DISPLAY_CONTROL_CURSOR:      

      break;     
                    
    default:   break;
  }

  // Calculate string indices: Page 0 uses 0,1; Page 1 uses 2,3; etc.
  


  if(_idxMenu==0){
    labelIdx1 = _idxPage * 2;
    labelIdx2 = (_idxPage * 2) + 1;
  }
  else{
    labelIdx1 = _idxPage * 2;
    labelIdx2 = (_idxPage * 2) + 1; 
  }

  Serial.print("Page: ");
  Serial.println(_idxPage);
  lcdPrintDoublePage(_idxPage, _data[labelIdx1], _data[labelIdx2]);
}


