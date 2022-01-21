#include <Wire.h>
  
#define DEVICE (0x53)    //ADXL345 device address
#define TO_READ (6)        //num of bytes we are going to read each time (two bytes for each axis)
  
byte buff[TO_READ] ;    //6 bytes buffer for saving data read from the device
char str[512];                      //string buffer to transform data before sending it to the serial port
  
/*  OLED          arduino
     D0-----------10
     D1-----------9
     RST----------13
     DC-----------11
     CS-----------12
     VCC----------5V
     GND----------GND*/
int SCL_PIN=10;//D0
int SDA_PIN=9; //D1
int RST_PIN=13;//RST
int DC_PIN=11; //DC
int CS_PIN=12; //DC

int lectura;
int lecturaAnt;

 void LED_CLS(void);
 void LED_Set_Pos(unsigned char x,unsigned char y);//Set the coordinate
 void LED_WrDat(unsigned char data);   //Write Data
 void LED_P6x8Char(unsigned char x,unsigned char y,unsigned char ch);
 void LED_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
 void LED_P8x16Str(unsigned char x,unsigned char y,unsigned char ch[]);

 void LED_PrintBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char bmp[]); 
 void LED_Fill(unsigned char dat);
 void LED_PrintValueC(unsigned char x, unsigned char y,char data);
 void LED_PrintValueI(unsigned char x, unsigned char y, int data);
 void LED_PrintValueF(unsigned char x, unsigned char y, float data, unsigned char num);
 void LED_PrintEdge(void);
 void LED_Cursor(unsigned char cursor_column, unsigned char cursor_row);
 void LED_PrintLine(void);
 
static const unsigned char wearable[] PROGMEM =
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xFC, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0x80, 0x80,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0x80, 0x80, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0x80,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0xC0, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0xFC, 0xFC, 0x04, 0x04, 0x04, 0x04, 0x0C, 0x0C, 0x18, 0xF0, 0xC0, 0x00, 0x00,
0x00, 0x00, 0xC0, 0xF0, 0x18, 0x0C, 0x0C, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0,
0xF0, 0x18, 0x0C, 0x0C, 0x04, 0x0C, 0x0C, 0x18, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01,
0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x03, 0x01, 0x80, 0x80, 0x80, 0x80, 0x81, 0xFF,
0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFE, 0x02, 0x03,
0x00, 0x00, 0x00, 0x00, 0x00, 0x1E, 0x03, 0x01, 0x80, 0x80, 0x80, 0x80, 0x81, 0xFF, 0xFC, 0x00,
0x00, 0x80, 0x80, 0xBF, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0xC0, 0x7F, 0x1F, 0x00, 0x00,
0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F,
0x7F, 0xC0, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x10, 0x10, 0x10, 0x10, 0x10, 0x18, 0x08,
0x0F, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x06, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0xFF,
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x06, 0x03, 0x01, 0x01, 0x01, 0x01, 0x01, 0xFF, 0xFF, 0x00,
0x00, 0x01, 0x01, 0xFD, 0xFF, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x06, 0xFE, 0xF8, 0x00, 0x00,
0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8,
0xFE, 0x03, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0,
0x07, 0x1F, 0x10, 0x30, 0x20, 0x20, 0x30, 0x10, 0x1C, 0x0F, 0x1C, 0x30, 0x30, 0x20, 0x20, 0x30,
0x18, 0x0F, 0x3F, 0x30, 0x20, 0x20, 0x20, 0x30, 0x1F, 0x0C, 0x10, 0x30, 0x20, 0x20, 0x20, 0x20,
0x20, 0x20, 0x20, 0x20, 0x30, 0x30, 0x18, 0x0F, 0x18, 0x10, 0x30, 0x20, 0x20, 0x30, 0x18, 0x0F,
0x1F, 0x30, 0x20, 0x20, 0x20, 0x30, 0x3F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x1F, 0x30, 0x20,
0x20, 0x20, 0x20, 0x20, 0x30, 0x1F, 0x18, 0x30, 0x30, 0x20, 0x20, 0x30, 0x18, 0x0F, 0x1F, 0x30,
0x20, 0x20, 0x20, 0x3F, 0x3F, 0x20, 0x20, 0x20, 0x20, 0x30, 0x30, 0x18, 0x0F, 0x03, 0x00, 0x00,
0x20, 0x38, 0x3F, 0x27, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x30, 0x18, 0x0F,
0x0F, 0x18, 0x30, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x30, 0x10, 0x18, 0x0C, 0x07, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char level[] PROGMEM =
{
0x00, 0x00, 0x00, 0x80, 0x40, 0xC0, 0x40, 0xC0, 0x40, 0xC0, 0x40, 0xC0, 0x40, 0xC0, 0xC0, 0xC0,
0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0,
0xC0, 0xC0, 0xC0, 0x40, 0xC0, 0x40, 0xC0, 0x40, 0xC0, 0x40, 0xC0, 0x40, 0x80, 0x00, 0x00, 0x00,
0x00, 0xE0, 0x1C, 0x03, 0x1D, 0xEA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAB, 0x5C, 0xE0,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xE0, 0x5C, 0xAB, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xEA, 0x1D, 0x03, 0x1C, 0xE0, 0x00,
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA,
0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF,
0x00, 0x0F, 0x30, 0xC0, 0x70, 0xAF, 0x5F, 0xAE, 0x5F, 0xAE, 0x5F, 0xAE, 0x5F, 0xEE, 0x3F, 0x0F,
0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A,
0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0xFF, 0x0A,
0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A,
0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A,
0x04, 0xFF, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A,
0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A, 0x04, 0x0A,
0x07, 0x3F, 0xEE, 0x5F, 0xAE, 0x5F, 0xAE, 0x5F, 0xAE, 0x5F, 0xAF, 0x78, 0xC0, 0x38, 0x07, 0x00,
0x00, 0x00, 0x00, 0x01, 0x03, 0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x03, 0x02, 0x02,
0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 0x02,
0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
0x02, 0x03, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
0x02, 0x02, 0x03, 0x03, 0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x02, 0x03, 0x01, 0x00, 0x00, 0x00
};

static const unsigned char bubble[] PROGMEM =
{
0xE0, 0x10, 0x08, 0x84, 0xC4, 0x82, 0x32, 0x12, 0x19, 0x19, 0x19, 0x19, 0x09, 0x09, 0x01, 0x01,
0x02, 0x02, 0x02, 0x04, 0x04, 0x08, 0x90, 0xE0, 0x01, 0x02, 0x04, 0x08, 0x09, 0x18, 0x18, 0x18,
0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x18, 0x18, 0x0C, 0x0C, 0x06, 0x03, 0x01
};

static const unsigned char bubble_black[] PROGMEM =
{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static const unsigned char lines[] PROGMEM =
{
0xFF, 0xFF
};

const unsigned char F6x8[][6] =
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
	{ 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
	{ 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
	{ 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
	{ 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
	{ 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
	{ 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
	{ 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
	{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
	{ 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
	{ 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
	{ 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
	{ 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
	{ 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
	{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
	{ 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
	{ 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
	{ 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
	{ 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
	{ 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
	{ 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
	{ 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
	{ 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
	{ 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
	{ 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
	{ 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
	{ 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
	{ 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
	{ 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
	{ 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
	{ 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
	{ 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
	{ 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
	{ 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
	{ 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
	{ 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
	{ 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
	{ 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
	{ 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
	{ 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
	{ 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
	{ 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
	{ 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
	{ 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
	{ 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
	{ 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
	{ 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
	{ 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
	{ 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
	{ 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
	{ 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
	{ 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
	{ 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
	{ 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [ 91
	{ 0x00, 0x02, 0x04 ,0x08, 0x10, 0x20 },    // \92
	{ 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
	{ 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
	{ 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
	{ 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
	{ 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
	{ 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
	{ 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
	{ 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
	{ 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
	{ 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
	{ 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
	{ 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
	{ 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
	{ 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
	{ 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
	{ 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
	{ 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
	{ 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
	{ 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
	{ 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
	{ 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
	{ 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
	{ 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
	{ 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
	{ 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
	{ 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
	{ 0x14, 0x14, 0x14, 0x14, 0x14, 0x14 }    // horiz lines
};

void LEDPIN_Init(void)    
{
 /* LED_SCL_Init;
  LED_SDA_Init;
  LED_RST_Init;
  LED_DC_Init;*/
  pinMode(SCL_PIN,OUTPUT);
  pinMode(SDA_PIN,OUTPUT);
  pinMode(RST_PIN,OUTPUT);
  pinMode(DC_PIN,OUTPUT);
  pinMode(CS_PIN,OUTPUT);
}
void LED_WrDat(unsigned char data)   
{
	unsigned char i = 8;
	//LED_CS=0;
        digitalWrite(CS_PIN,LOW);
	//LED_DCH;;;
        digitalWrite(DC_PIN,HIGH);
	//LED_SCLL;;; 
        digitalWrite(SCL_PIN,LOW);
	while (i--)
	{
		if (data & 0x80)
		{
			digitalWrite(SDA_PIN,HIGH);;;;
		}
		else
		{
			digitalWrite(SDA_PIN,LOW);;;
		}
		//LED_SCLH;;; 
                digitalWrite(SCL_PIN,HIGH);;;
		asm("nop");;;     
		//LED_SCLL;;;
                digitalWrite(SCL_PIN,LOW);
		data <<= 1;
	}
	//LED_CS=1;
        digitalWrite(CS_PIN,HIGH);
}
void LED_WrCmd(unsigned char cmd) 
{
	unsigned char i = 8;
	//LED_CS = 0;
        digitalWrite(CS_PIN,LOW);
	//LED_DCL;;;
        digitalWrite(DC_PIN,LOW);;;
	//LED_SCLL;;;
	digitalWrite(SCL_PIN,LOW);;;
         while (i--)
	{
		if (cmd & 0x80)
		{
			//LED_SDAH;;;
                      digitalWrite(SDA_PIN,HIGH);;;
		}
		else
		{
			//LED_SDAL;;;
                      digitalWrite(SDA_PIN,LOW);;;
		}
		//LED_SCLH;;;
		digitalWrite(SCL_PIN,HIGH);;;
                asm("nop");;;           
		//LED_SCLL;;;
                digitalWrite(SCL_PIN,LOW);;;
		cmd <<= 1;   
	} 	
	//LED_CS = 1;
        digitalWrite(CS_PIN,HIGH);
}
void LED_Set_Pos(unsigned char x, unsigned char y)
{ 
	LED_WrCmd(0xb0+y);
	LED_WrCmd(((x&0xf0)>>4)|0x10);
	LED_WrCmd((x&0x0f)|0x00); 
} 

void LED_Set_Pos2(unsigned char x, unsigned char y)
{ 
	LED_WrCmd(0xb0+y);
	LED_WrCmd(((x&0xf0)>>4)|0x10);
	LED_WrCmd((x&0x0f)|0x00); 
} 


void LED_Fill(unsigned char bmp_data)
{
	unsigned char y,x;
	
	for(y=0;y<8;y++)
	{
		LED_WrCmd(0xb0+y);  
		LED_WrCmd(0x00);     
		LED_WrCmd(0x10);	     
		for(x=0;x<128;x++)
			LED_WrDat(bmp_data);
	}
} 



void LED_CLS(void) 
{
	unsigned char y,x;	
	for(y=0;y<8;y++)
	{
		LED_WrCmd(0xb0+y);
		LED_WrCmd(0x00);
		LED_WrCmd(0x10); 
		for(x=0;x<128;x++)
			LED_WrDat(0);
	}
}

void LED_DLY_ms(unsigned int ms)
{                         
  unsigned int a;
  while(ms)
  {
    a=6675;
    while(a--);
    ms--;
  }
  return;
//  time_delay_ms(ms);
}

void SetStartColumn(unsigned char d)
{
	LED_WrCmd(0x00+d%16);	// Set Lower Column Start Address for Page Addressing Mode
							// Default => 0x00
	LED_WrCmd(0x10+d/16);	// Set Higher Column Start Address for Page Addressing Mode
							// Default => 0x10
}

void SetAddressingMode(unsigned char d)
{
	LED_WrCmd(0x20);			// Set Memory Addressing Mode
	LED_WrCmd(d);			// Default => 0x02
							// 0x00 => Horizontal Addressing Mode
							// 0x01 => Vertical Addressing Mode
							// 0x02 => Page Addressing Mode
}

void SetColumnAddress(unsigned char a, unsigned char b)
{
	LED_WrCmd(0x21);			// Set Column Address
	LED_WrCmd(a);			// Default => 0x00 (Column Start Address)
	LED_WrCmd(b);			// Default => 0x7F (Column End Address)
}

void SetPageAddress(unsigned char a, unsigned char b)
{
	LED_WrCmd(0x22);			// Set Page Address
	LED_WrCmd(a);			// Default => 0x00 (Page Start Address)
	LED_WrCmd(b);			// Default => 0x07 (Page End Address)
}

void SetStartLine(unsigned char d)
{
	LED_WrCmd(0x40|d);		// Set Display Start Line
							// Default => 0x40 (0x00)
}

void SetContrastControl(unsigned char d)
{
	LED_WrCmd(0x81);			// Set Contrast Control
	LED_WrCmd(d);			// Default => 0x7F
}

void Set_Charge_Pump(unsigned char d)
{
	LED_WrCmd(0x8D);			// Set Charge Pump
	LED_WrCmd(0x10|d);		// Default => 0x10
							// 0x10 (0x00) => Disable Charge Pump
							// 0x14 (0x04) => Enable Charge Pump
}

void Set_Segment_Remap(unsigned char d)
{
	LED_WrCmd(0xA0|d);		// Set Segment Re-Map
							// Default => 0xA0
							// 0xA0 (0x00) => Column Address 0 Mapped to SEG0
							// 0xA1 (0x01) => Column Address 0 Mapped to SEG127
}

void Set_Entire_Display(unsigned char d)
{
	LED_WrCmd(0xA4|d);		// Set Entire Display On / Off
							// Default => 0xA4
							// 0xA4 (0x00) => Normal Display
							// 0xA5 (0x01) => Entire Display On
}

void Set_Inverse_Display(unsigned char d)
{
	LED_WrCmd(0xA6|d);		// Set Inverse Display On/Off
							// Default => 0xA6
							// 0xA6 (0x00) => Normal Display
							// 0xA7 (0x01) => Inverse Display On
}

void Set_Multiplex_Ratio(unsigned char d)
{
	LED_WrCmd(0xA8);			// Set Multiplex Ratio
	LED_WrCmd(d);			// Default => 0x3F (1/64 Duty)
}

void Set_Display_On_Off(unsigned char d)
{
	LED_WrCmd(0xAE|d);		// Set Display On/Off
							// Default => 0xAE
							// 0xAE (0x00) => Display Off
							// 0xAF (0x01) => Display On
}

void SetStartPage(unsigned char d)
{
	LED_WrCmd(0xB0|d);		// Set Page Start Address for Page Addressing Mode
							// Default => 0xB0 (0x00)
}

void Set_Common_Remap(unsigned char d)
{
	LED_WrCmd(0xC0|d);		// Set COM Output Scan Direction
							// Default => 0xC0
							// 0xC0 (0x00) => Scan from COM0 to 63
							// 0xC8 (0x08) => Scan from COM63 to 0
}

void Set_Display_Offset(unsigned char d)
{
	LED_WrCmd(0xD3);			// Set Display Offset
	LED_WrCmd(d);			// Default => 0x00
}

void Set_Display_Clock(unsigned char d)
{
	LED_WrCmd(0xD5);			// Set Display Clock Divide Ratio / Oscillator Frequency
	LED_WrCmd(d);			// Default => 0x80
							// D[3:0] => Display Clock Divider
							// D[7:4] => Oscillator Frequency
}

void Set_Precharge_Period(unsigned char d)
{
	LED_WrCmd(0xD9);			// Set Pre-Charge Period
	LED_WrCmd(d);			// Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
							// D[3:0] => Phase 1 Period in 1~15 Display Clocks
							// D[7:4] => Phase 2 Period in 1~15 Display Clocks
}

void Set_Common_Config(unsigned char d)
{
	LED_WrCmd(0xDA);			// Set COM Pins Hardware Configuration
	LED_WrCmd(0x02|d);		// Default => 0x12 (0x10)
							// Alternative COM Pin Configuration
							// Disable COM Left/Right Re-Map
}

void Set_VCOMH(unsigned char d)
{
	LED_WrCmd(0xDB);			// Set VCOMH Deselect Level
	LED_WrCmd(d);			// Default => 0x20 (0.77*VCC)
}

void Set_NOP(void)
{
	LED_WrCmd(0xE3);			// Command for No Operation
}

void LED_Init(void)        
{
	unsigned char i;
        LEDPIN_Init();
//	LED_PORT=0X0F;
	//LED_SCLH;;;		
	//LED_RSTL;;;
        digitalWrite(SCL_PIN,HIGH);;;
        digitalWrite(RST_PIN,LOW);;;
//	for(i=0;i<100;i++)asm("nop");     	
        LED_DLY_ms(50);
	//LED_RSTH;;;
        digitalWrite(RST_PIN,HIGH);
	Set_Display_On_Off(0x00);		  // Display Off (0x00/0x01)
	Set_Display_Clock(0x80);		  // Set Clock as 100 Frames/Sec
	Set_Multiplex_Ratio(0x3F);	  // 1/64 Duty (0x0F~0x3F)
	Set_Display_Offset(0x00);		  // Shift Mapping RAM Counter (0x00~0x3F)
	SetStartLine(0x00);			  // Set Mapping RAM Display Start Line (0x00~0x3F)
	Set_Charge_Pump(0x04);		  // Enable Embedded DC/DC Converter (0x00/0x04)
	SetAddressingMode(0x02);	  // Set Page Addressing Mode (0x00/0x01/0x02)
	Set_Segment_Remap(0x01);	  // Set SEG/Column Mapping  
	Set_Common_Remap(0x08);	  // Set COM/Row Scan Direction 
	Set_Common_Config(0x10);	  // Set Sequential Configuration (0x00/0x10)
	SetContrastControl(0xC7); // Set SEG Output Current
	Set_Precharge_Period(0xF1);	  // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	Set_VCOMH(0x40);			  // Set VCOM Deselect Level
	Set_Entire_Display(0x00);		  // Disable Entire Display On (0x00/0x01)
	Set_Inverse_Display(0x00);	  // Disable Inverse Display On (0x00/0x01)  
	Set_Display_On_Off(0x01);		  // Display On (0x00/0x01)
	LED_Fill(0x00);                               //clear all
	LED_Set_Pos(0,0); 	
} 
 

void LED_P6x8Char(unsigned char x,unsigned char y,unsigned char ch)
{
	 unsigned char c=0,i=0,j=0;     
	   
	c =ch-32;
	if(x>122)
	{
		x=0;
		y++;
	}
	LED_Set_Pos(x,y);    
	for(i=0;i<6;i++)
	{     
		LED_WrDat(F6x8[c][i]);  
	}
}

void LED_P6x8Str(unsigned char x,unsigned char y,char ch[])
{
	unsigned char c=0,i=0,j=0;      
	while (ch[j]!='\0')
	{    
		c =ch[j]-32;
		if(x>126)
		{
			x=0;
			y++;
		}
		LED_Set_Pos(x,y);    
		for(i=0;i<6;i++)
		{     
			LED_WrDat(F6x8[c][i]);  
		}
		x+=6;
		j++;
	}
}

void LED_PrintBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char bmp[])
{ 	
	int ii=0;
	unsigned char x,y;
	for(y=y0;y<=y1;y++)
	{
		LED_Set_Pos(x0+1,y);				
		for(x=x0;x<x1;x++)
		{      
			LED_WrDat(pgm_read_word(&bmp[ii++]));
                         //delay(10);	    	
		}
	}
}

void LED_PrintBMPnorm(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char bmp[])
{ 	
	int ii=0;
	unsigned char x,y;
	for(y=y0;y<=y1;y++)
	{
		LED_Set_Pos(x0,y);				
		for(x=x0;x<x1;x++)
		{      
			LED_WrDat(bmp[ii++]);
                         //delay(10);	    	
		}
	}
}

void LED_PrintValueC(unsigned char x, unsigned char y, char data)
{
	unsigned char i,j,k;
	if(data < 0)
	{
		LED_P6x8Char(x,y,'-');
		data = - data;	
	}
	else
	{
		LED_P6x8Char(x,y,'+');
	}
	i = data/100;
	j = (data%100)/10;
	k = data%10;
	LED_P6x8Char(x+6,y,i+48);
	LED_P6x8Char(x+12,y,j+48);
	LED_P6x8Char(x+18,y,k+48);		
}


void LED_PrintValueI(unsigned char x, unsigned char y, int data)
{
	unsigned char i,j,k,l,m;  
	/*if(data < 0)
	{
		LED_P6x8Char(x,y,'-');
		data = - data;	
	}
	else
	{
		LED_P6x8Char(x,y,'+');
	}
*/
	
	//l  = data/10000;
	m= (data%10000)/1000;
	i = (data%1000)/100;
	j = (data%100)/10;
	k = data%10;
	//LED_P6x8Char(x+6,y,l+48);
	LED_P6x8Char(x+12,y,m+48);
	LED_P6x8Char(x+18,y,i+48);
	LED_P6x8Char(x+24,y,j+48);
	LED_P6x8Char(x+30,y,k+48);		
}



 void LED_Cursor(unsigned char cursor_column, unsigned char cursor_row)
 {	
 	if(cursor_row != 0) 
 	{
 		if(cursor_column == 1) LED_Set_Pos(0, cursor_row + 2);
 		else  LED_Set_Pos(80 + (cursor_column - 2)*6, cursor_row + 2);
 		LED_WrDat(0xFF);
 		LED_WrDat(0xFF);
 		LED_WrDat(0xFF);
 		LED_WrDat(0xFF);
 		LED_WrDat(0xFF);
 		LED_WrDat(0xFF);					 
 	}
 }
 
 
 void setup()
 {
   Serial.begin(9600);
     Wire.begin();        // join i2c bus (address optional for master)
  
  //Turning on the ADXL345
  writeTo(DEVICE, 0x2D, 0);      
  writeTo(DEVICE, 0x2D, 16);
  writeTo(DEVICE, 0x2D, 8);
   LEDPIN_Init();
   LED_Init();
   LED_PrintBMP(1,0,129,7,(unsigned char *)wearable);
   delay(3000);
   LED_CLS();
   LED_PrintBMP(1,1,129,5,(unsigned char *)level);
 }
 void loop(){
  int regAddress = 0x32;    //first axis-acceleration-data register on the ADXL345
  int x, y, z;
  
  readFrom(DEVICE, regAddress, TO_READ, buff); //read the acceleration data from the ADXL345
  
  //each axis reading comes in 10 bit resolution, ie 2 bytes.  Least Significat Byte first!!
  //thus we are converting both bytes in to one int
  x = (((int)buff[1]) << 8) | buff[0];   
  y = (((int)buff[3])<< 8) | buff[2];
  z = (((int)buff[5]) << 8) | buff[4];
  
  //we send the x y z values as a string to the serial port
  
  //It appears that delay is needed in order not to clog the port
   lectura = map(y,-200,200,104,0);//218 para 3.3v--150 para 5v
   if(lectura>87) lectura=87;
   if(lectura<17) lectura=17;
   LED_PrintBMP(lectura+1,2,lectura+25,3,(unsigned char *)bubble);
   LED_PrintBMP(47,2,48,3,(unsigned char *)lines);
   LED_PrintBMP(82,2,83,3,(unsigned char *)lines);
delay(60);
   LED_PrintBMP(lectura+1,2,lectura+25,3,(unsigned char *)bubble_black);
      
}

//---------------- Functions
//Writes val to address register on device
void writeTo(int device, byte address, byte val) {
  Wire.beginTransmission(device); //start transmission to device 
  Wire.write(address);        // send register address
  Wire.write(val);        // send value to write
  Wire.endTransmission(); //end transmission
}
  
//reads num bytes starting from address register on device in to buff array
void readFrom(int device, byte address, int num, byte buff[]) {
  Wire.beginTransmission(device); //start transmission to device 
  Wire.write(address);        //nds address to read from
  Wire.endTransmission(); //end transmission
  Wire.beginTransmission(device); //start transmission to device
  Wire.requestFrom(device, num);    // request 6 bytes from device
  
  int i = 0;
  while(Wire.available())    //device may send less than requested (abnormal)
  { 
    buff[i] = Wire.read(); // receive a byte
    i++;
  }
  Wire.endTransmission(); //end transmission
}
 
