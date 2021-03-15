#include "8bit_parallel.h"
#include <math.h> //Math Library

#define MAX_VALUE 60
#define PI 3.141592653589793238

// DECLARATION CONSTANT

#define XC 0x2A
#define YC 0x2B
#define CC 0x2C
#define RC 0x2E
#define SC1 0x33
#define SC2 0x37
#define MD 0x36
#define VL 0
#define R24BIT 0

#define LEFT 0
#define RIGHT 9999
#define CENTER 9998

#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04

uint8_t rotation = 0;
uint16_t lcd_model = 0x9486;
const uint16_t wid = 320;
const uint16_t heg = 480;
const uint16_t WIDTH = wid;
const uint16_t HEIGHT = heg;
uint16_t width = WIDTH;
uint16_t height = HEIGHT;

int16_t text_x, text_y;
uint16_t text_color = 0x07E0;
uint16_t text_bgcolor = 0xF800;
uint16_t draw_color = 0xF800;
uint8_t text_size = 1;
boolean text_mode = 0; //if set,text_bgcolor is invalid

static const unsigned char lcd_font[] = 
{
    0x00, 0x00, 0x00, 0x00, 0x00,   
  0x3E, 0x5B, 0x4F, 0x5B, 0x3E,   
  0x3E, 0x6B, 0x4F, 0x6B, 0x3E,   
  0x1C, 0x3E, 0x7C, 0x3E, 0x1C, 
  0x18, 0x3C, 0x7E, 0x3C, 0x18, 
  0x1C, 0x57, 0x7D, 0x57, 0x1C, 
  0x1C, 0x5E, 0x7F, 0x5E, 0x1C, 
  0x00, 0x18, 0x3C, 0x18, 0x00, 
  0xFF, 0xE7, 0xC3, 0xE7, 0xFF, 
  0x00, 0x18, 0x24, 0x18, 0x00, 
  0xFF, 0xE7, 0xDB, 0xE7, 0xFF, 
  0x30, 0x48, 0x3A, 0x06, 0x0E, 
  0x26, 0x29, 0x79, 0x29, 0x26, 
  0x40, 0x7F, 0x05, 0x05, 0x07, 
  0x40, 0x7F, 0x05, 0x25, 0x3F, 
  0x5A, 0x3C, 0xE7, 0x3C, 0x5A, 
  0x7F, 0x3E, 0x1C, 0x1C, 0x08, 
  0x08, 0x1C, 0x1C, 0x3E, 0x7F, 
  0x14, 0x22, 0x7F, 0x22, 0x14, 
  0x5F, 0x5F, 0x00, 0x5F, 0x5F, 
  0x06, 0x09, 0x7F, 0x01, 0x7F, 
  0x00, 0x66, 0x89, 0x95, 0x6A, 
  0x60, 0x60, 0x60, 0x60, 0x60, 
  0x94, 0xA2, 0xFF, 0xA2, 0x94, 
  0x08, 0x04, 0x7E, 0x04, 0x08, 
  0x10, 0x20, 0x7E, 0x20, 0x10, 
  0x08, 0x08, 0x2A, 0x1C, 0x08, 
  0x08, 0x1C, 0x2A, 0x08, 0x08, 
  0x1E, 0x10, 0x10, 0x10, 0x10, 
  0x0C, 0x1E, 0x0C, 0x1E, 0x0C, 
  0x30, 0x38, 0x3E, 0x38, 0x30, 
  0x06, 0x0E, 0x3E, 0x0E, 0x06, 
  0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x5F, 0x00, 0x00, 
  0x00, 0x07, 0x00, 0x07, 0x00, 
  0x14, 0x7F, 0x14, 0x7F, 0x14, 
  0x24, 0x2A, 0x7F, 0x2A, 0x12, 
  0x23, 0x13, 0x08, 0x64, 0x62, 
  0x36, 0x49, 0x56, 0x20, 0x50, 
  0x00, 0x08, 0x07, 0x03, 0x00, 
  0x00, 0x1C, 0x22, 0x41, 0x00, 
  0x00, 0x41, 0x22, 0x1C, 0x00, 
  0x2A, 0x1C, 0x7F, 0x1C, 0x2A, 
  0x08, 0x08, 0x3E, 0x08, 0x08, 
  0x00, 0x80, 0x70, 0x30, 0x00, 
  0x08, 0x08, 0x08, 0x08, 0x08, 
  0x00, 0x00, 0x60, 0x60, 0x00, 
  0x20, 0x10, 0x08, 0x04, 0x02, 
  0x3E, 0x51, 0x49, 0x45, 0x3E, 
  0x00, 0x42, 0x7F, 0x40, 0x00, 
  0x72, 0x49, 0x49, 0x49, 0x46, 
  0x21, 0x41, 0x49, 0x4D, 0x33, 
  0x18, 0x14, 0x12, 0x7F, 0x10, 
  0x27, 0x45, 0x45, 0x45, 0x39, 
  0x3C, 0x4A, 0x49, 0x49, 0x31, 
  0x41, 0x21, 0x11, 0x09, 0x07, 
  0x36, 0x49, 0x49, 0x49, 0x36, 
  0x46, 0x49, 0x49, 0x29, 0x1E, 
  0x00, 0x00, 0x14, 0x00, 0x00, 
  0x00, 0x40, 0x34, 0x00, 0x00, 
  0x00, 0x08, 0x14, 0x22, 0x41, 
  0x14, 0x14, 0x14, 0x14, 0x14, 
  0x00, 0x41, 0x22, 0x14, 0x08, 
  0x02, 0x01, 0x59, 0x09, 0x06, 
  0x3E, 0x41, 0x5D, 0x59, 0x4E, 
  0x7C, 0x12, 0x11, 0x12, 0x7C, 
  0x7F, 0x49, 0x49, 0x49, 0x36, 
  0x3E, 0x41, 0x41, 0x41, 0x22, 
  0x7F, 0x41, 0x41, 0x41, 0x3E, 
  0x7F, 0x49, 0x49, 0x49, 0x41, 
  0x7F, 0x09, 0x09, 0x09, 0x01, 
  0x3E, 0x41, 0x41, 0x51, 0x73, 
  0x7F, 0x08, 0x08, 0x08, 0x7F, 
  0x00, 0x41, 0x7F, 0x41, 0x00, 
  0x20, 0x40, 0x41, 0x3F, 0x01, 
  0x7F, 0x08, 0x14, 0x22, 0x41, 
  0x7F, 0x40, 0x40, 0x40, 0x40, 
  0x7F, 0x02, 0x1C, 0x02, 0x7F, 
  0x7F, 0x04, 0x08, 0x10, 0x7F, 
  0x3E, 0x41, 0x41, 0x41, 0x3E, 
  0x7F, 0x09, 0x09, 0x09, 0x06, 
  0x3E, 0x41, 0x51, 0x21, 0x5E, 
  0x7F, 0x09, 0x19, 0x29, 0x46, 
  0x26, 0x49, 0x49, 0x49, 0x32, 
  0x03, 0x01, 0x7F, 0x01, 0x03, 
  0x3F, 0x40, 0x40, 0x40, 0x3F, 
  0x1F, 0x20, 0x40, 0x20, 0x1F, 
  0x3F, 0x40, 0x38, 0x40, 0x3F, 
  0x63, 0x14, 0x08, 0x14, 0x63, 
  0x03, 0x04, 0x78, 0x04, 0x03, 
  0x61, 0x59, 0x49, 0x4D, 0x43, 
  0x00, 0x7F, 0x41, 0x41, 0x41, 
  0x02, 0x04, 0x08, 0x10, 0x20, 
  0x00, 0x41, 0x41, 0x41, 0x7F, 
  0x04, 0x02, 0x01, 0x02, 0x04, 
  0x40, 0x40, 0x40, 0x40, 0x40, 
  0x00, 0x03, 0x07, 0x08, 0x00, 
  0x20, 0x54, 0x54, 0x78, 0x40, 
  0x7F, 0x28, 0x44, 0x44, 0x38, 
  0x38, 0x44, 0x44, 0x44, 0x28, 
  0x38, 0x44, 0x44, 0x28, 0x7F, 
  0x38, 0x54, 0x54, 0x54, 0x18, 
  0x00, 0x08, 0x7E, 0x09, 0x02, 
  0x18, 0xA4, 0xA4, 0x9C, 0x78, 
  0x7F, 0x08, 0x04, 0x04, 0x78, 
  0x00, 0x44, 0x7D, 0x40, 0x00, 
  0x20, 0x40, 0x40, 0x3D, 0x00, 
  0x7F, 0x10, 0x28, 0x44, 0x00, 
  0x00, 0x41, 0x7F, 0x40, 0x00, 
  0x7C, 0x04, 0x78, 0x04, 0x78, 
  0x7C, 0x08, 0x04, 0x04, 0x78, 
  0x38, 0x44, 0x44, 0x44, 0x38, 
  0xFC, 0x18, 0x24, 0x24, 0x18, 
  0x18, 0x24, 0x24, 0x18, 0xFC, 
  0x7C, 0x08, 0x04, 0x04, 0x08, 
  0x48, 0x54, 0x54, 0x54, 0x24, 
  0x04, 0x04, 0x3F, 0x44, 0x24, 
  0x3C, 0x40, 0x40, 0x20, 0x7C, 
  0x1C, 0x20, 0x40, 0x20, 0x1C, 
  0x3C, 0x40, 0x30, 0x40, 0x3C, 
  0x44, 0x28, 0x10, 0x28, 0x44, 
  0x4C, 0x90, 0x90, 0x90, 0x7C, 
  0x44, 0x64, 0x54, 0x4C, 0x44, 
  0x00, 0x08, 0x36, 0x41, 0x00, 
  0x00, 0x00, 0x77, 0x00, 0x00, 
  0x00, 0x41, 0x36, 0x08, 0x00, 
  0x02, 0x01, 0x02, 0x04, 0x02, 
  0x3C, 0x26, 0x23, 0x26, 0x3C, 
  0x1E, 0xA1, 0xA1, 0x61, 0x12, 
  0x3A, 0x40, 0x40, 0x20, 0x7A, 
  0x38, 0x54, 0x54, 0x55, 0x59, 
  0x21, 0x55, 0x55, 0x79, 0x41, 
  0x21, 0x54, 0x54, 0x78, 0x41, 
  0x21, 0x55, 0x54, 0x78, 0x40, 
  0x20, 0x54, 0x55, 0x79, 0x40, 
  0x0C, 0x1E, 0x52, 0x72, 0x12, 
  0x39, 0x55, 0x55, 0x55, 0x59, 
  0x39, 0x54, 0x54, 0x54, 0x59, 
  0x39, 0x55, 0x54, 0x54, 0x58, 
  0x00, 0x00, 0x45, 0x7C, 0x41, 
  0x00, 0x02, 0x45, 0x7D, 0x42, 
  0x00, 0x01, 0x45, 0x7C, 0x40, 
  0xF0, 0x29, 0x24, 0x29, 0xF0, 
  0xF0, 0x28, 0x25, 0x28, 0xF0, 
  0x7C, 0x54, 0x55, 0x45, 0x00, 
  0x20, 0x54, 0x54, 0x7C, 0x54, 
  0x7C, 0x0A, 0x09, 0x7F, 0x49, 
  0x32, 0x49, 0x49, 0x49, 0x32, 
  0x32, 0x48, 0x48, 0x48, 0x32, 
  0x32, 0x4A, 0x48, 0x48, 0x30, 
  0x3A, 0x41, 0x41, 0x21, 0x7A, 
  0x3A, 0x42, 0x40, 0x20, 0x78, 
  0x00, 0x9D, 0xA0, 0xA0, 0x7D, 
  0x39, 0x44, 0x44, 0x44, 0x39, 
  0x3D, 0x40, 0x40, 0x40, 0x3D, 
  0x3C, 0x24, 0xFF, 0x24, 0x24, 
  0x48, 0x7E, 0x49, 0x43, 0x66, 
  0x2B, 0x2F, 0xFC, 0x2F, 0x2B, 
  0xFF, 0x09, 0x29, 0xF6, 0x20, 
  0xC0, 0x88, 0x7E, 0x09, 0x03, 
  0x20, 0x54, 0x54, 0x79, 0x41, 
  0x00, 0x00, 0x44, 0x7D, 0x41, 
  0x30, 0x48, 0x48, 0x4A, 0x32, 
  0x38, 0x40, 0x40, 0x22, 0x7A, 
  0x00, 0x7A, 0x0A, 0x0A, 0x72, 
  0x7D, 0x0D, 0x19, 0x31, 0x7D, 
  0x26, 0x29, 0x29, 0x2F, 0x28, 
  0x26, 0x29, 0x29, 0x29, 0x26, 
  0x30, 0x48, 0x4D, 0x40, 0x20, 
  0x38, 0x08, 0x08, 0x08, 0x08, 
  0x08, 0x08, 0x08, 0x08, 0x38, 
  0x2F, 0x10, 0xC8, 0xAC, 0xBA, 
  0x2F, 0x10, 0x28, 0x34, 0xFA, 
  0x00, 0x00, 0x7B, 0x00, 0x00, 
  0x08, 0x14, 0x2A, 0x14, 0x22, 
  0x22, 0x14, 0x2A, 0x14, 0x08, 
  0xAA, 0x00, 0x55, 0x00, 0xAA, 
  0xAA, 0x55, 0xAA, 0x55, 0xAA, 
  0x00, 0x00, 0x00, 0xFF, 0x00, 
  0x10, 0x10, 0x10, 0xFF, 0x00, 
  0x14, 0x14, 0x14, 0xFF, 0x00, 
  0x10, 0x10, 0xFF, 0x00, 0xFF, 
  0x10, 0x10, 0xF0, 0x10, 0xF0, 
  0x14, 0x14, 0x14, 0xFC, 0x00, 
  0x14, 0x14, 0xF7, 0x00, 0xFF, 
  0x00, 0x00, 0xFF, 0x00, 0xFF, 
  0x14, 0x14, 0xF4, 0x04, 0xFC, 
  0x14, 0x14, 0x17, 0x10, 0x1F, 
  0x10, 0x10, 0x1F, 0x10, 0x1F, 
  0x14, 0x14, 0x14, 0x1F, 0x00, 
  0x10, 0x10, 0x10, 0xF0, 0x00, 
  0x00, 0x00, 0x00, 0x1F, 0x10, 
  0x10, 0x10, 0x10, 0x1F, 0x10, 
  0x10, 0x10, 0x10, 0xF0, 0x10, 
  0x00, 0x00, 0x00, 0xFF, 0x10, 
  0x10, 0x10, 0x10, 0x10, 0x10, 
  0x10, 0x10, 0x10, 0xFF, 0x10, 
  0x00, 0x00, 0x00, 0xFF, 0x14, 
  0x00, 0x00, 0xFF, 0x00, 0xFF, 
  0x00, 0x00, 0x1F, 0x10, 0x17, 
  0x00, 0x00, 0xFC, 0x04, 0xF4, 
  0x14, 0x14, 0x17, 0x10, 0x17, 
  0x14, 0x14, 0xF4, 0x04, 0xF4, 
  0x00, 0x00, 0xFF, 0x00, 0xF7, 
  0x14, 0x14, 0x14, 0x14, 0x14, 
  0x14, 0x14, 0xF7, 0x00, 0xF7, 
  0x14, 0x14, 0x14, 0x17, 0x14, 
  0x10, 0x10, 0x1F, 0x10, 0x1F, 
  0x14, 0x14, 0x14, 0xF4, 0x14, 
  0x10, 0x10, 0xF0, 0x10, 0xF0, 
  0x00, 0x00, 0x1F, 0x10, 0x1F, 
  0x00, 0x00, 0x00, 0x1F, 0x14, 
  0x00, 0x00, 0x00, 0xFC, 0x14, 
  0x00, 0x00, 0xF0, 0x10, 0xF0, 
  0x10, 0x10, 0xFF, 0x10, 0xFF, 
  0x14, 0x14, 0x14, 0xFF, 0x14, 
  0x10, 0x10, 0x10, 0x1F, 0x00, 
  0x00, 0x00, 0x00, 0xF0, 0x10, 
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
  0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 
  0xFF, 0xFF, 0xFF, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xFF, 0xFF, 
  0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 
  0x38, 0x44, 0x44, 0x38, 0x44, 
  0x7C, 0x2A, 0x2A, 0x3E, 0x14, 
  0x7E, 0x02, 0x02, 0x06, 0x06, 
  0x02, 0x7E, 0x02, 0x7E, 0x02, 
  0x63, 0x55, 0x49, 0x41, 0x63, 
  0x38, 0x44, 0x44, 0x3C, 0x04, 
  0x40, 0x7E, 0x20, 0x1E, 0x20, 
  0x06, 0x02, 0x7E, 0x02, 0x02, 
  0x99, 0xA5, 0xE7, 0xA5, 0x99, 
  0x1C, 0x2A, 0x49, 0x2A, 0x1C, 
  0x4C, 0x72, 0x01, 0x72, 0x4C, 
  0x30, 0x4A, 0x4D, 0x4D, 0x30, 
  0x30, 0x48, 0x78, 0x48, 0x30, 
  0xBC, 0x62, 0x5A, 0x46, 0x3D, 
  0x3E, 0x49, 0x49, 0x49, 0x00, 
  0x7E, 0x01, 0x01, 0x01, 0x7E, 
  0x2A, 0x2A, 0x2A, 0x2A, 0x2A, 
  0x44, 0x44, 0x5F, 0x44, 0x44, 
  0x40, 0x51, 0x4A, 0x44, 0x40, 
  0x40, 0x44, 0x4A, 0x51, 0x40, 
  0x00, 0x00, 0xFF, 0x01, 0x03, 
  0xE0, 0x80, 0xFF, 0x00, 0x00, 
  0x08, 0x08, 0x6B, 0x6B, 0x08,
  0x36, 0x12, 0x36, 0x24, 0x36, 
  0x06, 0x0F, 0x09, 0x0F, 0x06, 
  0x00, 0x00, 0x18, 0x18, 0x00, 
  0x00, 0x00, 0x10, 0x10, 0x00, 
  0x30, 0x40, 0xFF, 0x01, 0x01, 
  0x00, 0x1F, 0x01, 0x01, 0x1E, 
  0x00, 0x19, 0x1D, 0x17, 0x12, 
  0x00, 0x3C, 0x3C, 0x3C, 0x3C, 
  0x00, 0x00, 0x00, 0x00, 0x00, 
};

//-----------------------------------

uint8_t cps=10;
uint8_t previous_cps_bar=0;
uint8_t previous_cps_curve=0;
bool flag_init=false;
bool flag_curve=false;
int time_now = millis();
char temporary[20];


void setup() {
  Serial.begin(9600);
  setWriteDir();
  Init_LCD();

  Set_Rotation(1);
  Fill_Screen(0, 0, 0);

}

void loop() {
  // put your main code here, to run repeatedly:
if(millis()-time_now>1000)
{
  time_now = millis();
  if(cps>=60)
  {
    cps=0;
  }
  else
  {
    cps=cps+1;
  }
}
Bar_Create_Update(15, 50, 65, 305, MAX_VALUE);
//Curve_Create_Update(280,300,MAX_VALUE,150);

}



void Bar_Create_Update(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t max_val)
{
  if(flag_init == false)
  {
  Set_Draw_color(198, 144, 245);
  Draw_Rectangle(x1, y1, x2,y2);
  Set_Draw_color(59, 33, 82);
  Fill_Rectangle(x1+1, y1+1, x2-1,y2-1);

  uint8_t count_x;
  uint8_t count_y;
  
  for(count_y=0;count_y<11;count_y++)
  {   
    for(count_x=0;count_x<8;count_x++)
    {
    Set_Draw_color(255, 255, 255);
    Draw_Pixel( x2+count_x, y2-((y2-y1)*0.1*count_y));
    }

    Set_Text_colour(255, 255, 255);
    Set_Text_Size(1);
    Set_Text_Mode(1);
    //Print_String("8", x2+count_x+2, y2-((y2-y1)*0.1*count_y)-4);
    
  }
  flag_init = true;
  }

  uint8_t threshold1 = max_val/3;
  uint8_t threshold2 = (max_val*2)/3;

 if(cps != previous_cps_bar)
 {
  previous_cps_bar =cps; 
  if(cps<threshold1)
  {
    Set_Draw_color(59, 33, 82);
    Fill_Rectangle(x1+1, y1+1, x2-1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val))));
    Set_Draw_color(84, 209, 91);
    Fill_Rectangle(x1+1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val))), x2-1,y2-1);
    
  }

  else if(cps<threshold2)
  {
    Set_Draw_color(59, 33, 82);
    Fill_Rectangle(x1+1, y1+1, x2-1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val))));
    Set_Draw_color(222, 115, 33);
    Fill_Rectangle(x1+1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val))), x2-1,y2-1);
  }

  else if(cps<=max_val)
  {
    Set_Draw_color(59, 33, 82);
    Fill_Rectangle(x1+1, y1+1, x2-1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val))));
    Set_Draw_color(204, 30, 24);
    Fill_Rectangle(x1+1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val))), x2-1,y2-1);
  }

    Set_Text_colour(255, 255, 255);
    Set_Text_Size(3);
    Set_Text_Mode(0);
    Set_Text_Back_colour(0,0,0);
    //Print_String("8", x1+9, y1-30);

 }
  
}

void Curve_Create_Update(uint16_t x1,uint16_t y1,uint8_t max_val, uint8_t magnitude)
{
  uint16_t angle;
  uint16_t x_val,y_val;
  uint8_t magnitude_change;
  uint8_t width = 7;
  if(flag_curve == false)
  {
  for(angle =180; angle>=180-60; angle--)
  {
  for(magnitude_change=magnitude;magnitude_change>magnitude-width;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    Set_Draw_color(84, 209, 91);
    Draw_Pixel(x1+x_val,y1-y_val);
  }
  
  }

  for(angle =120; angle>=180-120; angle--)
  {
  for(magnitude_change=magnitude;magnitude_change>magnitude-width;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    Set_Draw_color(222, 115, 33);
    Draw_Pixel(x1+x_val,y1-y_val);
  }
  
  }

  for(angle =60; angle>0; angle--)
  {
  for(magnitude_change=magnitude;magnitude_change>magnitude-width;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    Set_Draw_color(204, 30, 24);
    Draw_Pixel(x1+x_val,y1-y_val);
  }
  
  }
  flag_curve=true;
  }

   uint8_t threshold1 = max_val/3;
   uint8_t threshold2 = (max_val*2)/3;

 if(cps != previous_cps_curve)
 {
   previous_cps_curve = cps;
   if(cps<threshold1)
  {
  for(angle =180; angle>int(180-(float(cps)/float(max_val))*180); angle--)
  {
  for(magnitude_change=magnitude-width-10;magnitude_change>0;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    Set_Draw_color(84, 209, 91);
    Draw_Pixel(x1+x_val,y1-y_val);
  } 
  }  

  for(angle =int(180-(float(cps)/float(max_val))*180); angle>0; angle--)
  {
  for(magnitude_change=magnitude-width-10;magnitude_change>0;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    Set_Draw_color(0, 0, 0);
    Draw_Pixel(x1+x_val,y1-y_val);
  } 
  } 
  
  }

  else if(cps<threshold2)
  {
   for(angle =120; angle>int(180-(float(cps)/float(max_val))*180); angle--)
  {
  for(magnitude_change=magnitude-width-10;magnitude_change>0;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    Set_Draw_color(222, 115, 33);
    Draw_Pixel(x1+x_val,y1-y_val);
  } 
  }  

  for(angle =int(180-(float(cps)/float(max_val))*180); angle>0; angle--)
  {
  for(magnitude_change=magnitude-width-10;magnitude_change>0;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    Set_Draw_color(0, 0, 0);
    Draw_Pixel(x1+x_val,y1-y_val);
  } 
  } 
  
  }

  else if(cps<=max_val)
  {
   for(angle =60; angle>int(180-(float(cps)/float(max_val))*180); angle--)
  {
  for(magnitude_change=magnitude-width-10;magnitude_change>0;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    Set_Draw_color(204, 30, 24);
    Draw_Pixel(x1+x_val,y1-y_val);
  } 
  }  

  for(angle =int(180-(float(cps)/float(max_val))*180); angle>0; angle--)
  {
  for(magnitude_change=magnitude-width-10;magnitude_change>0;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    Set_Draw_color(0, 0, 0);
    Draw_Pixel(x1+x_val,y1-y_val);
  } 
  }
  }
 }
   
 } 
  

















//LCD KBV ---------------------------------------------------------

void Init_LCD()
{
  SET_CNTL_BITS; // SET Control Bits to OUTPUT
  RES_ACTIVE;  // RESET pin HIGH
  reset();
  if(lcd_model == 0xFFFF)
  {
    lcd_model = Read_ID(); 
  }
  start(lcd_model);

}

void reset()
{
    CS_IDLE;
    RD_IDLE;
    WR_IDLE;
    RES_IDLE;
    delay(2);
    RES_ACTIVE;
    CS_ACTIVE;
    CD_COMMAND;
    write8(0x00);
    for(uint8_t i=0; i<3; i++)
    {
      WR_STROBE; // Three extra 0x00s
    }
    CS_IDLE;
}

void Write_Cmd(uint16_t cmd)
{
  writeCmd16(cmd);
}

void Write_Data(uint16_t data)
{
  writeData16(data);
}

void Write_Cmd_Data(uint16_t cmd, uint16_t data)
{
  writeCmdData16(cmd,data);
}

//Write a command and N datas
void Push_Command(uint16_t cmd, uint8_t *block, int8_t N)
{
    CS_ACTIVE;
    writeCmd16(cmd);
    while (N-- > 0) 
  {
        uint8_t u8 = *block++;
        writeData8(u8);
    }
    CS_IDLE;
}

// Sets the LCD address window 
void Set_Addr_Window(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  CS_ACTIVE;
  uint8_t x_buf[] = {x1>>8,x1&0xFF,x2>>8,x2&0xFF}; 
  uint8_t y_buf[] = {y1>>8,y1&0xFF,y2>>8,y2&0xFF}; 

  Push_Command(XC, x_buf, 4); //set x address
  Push_Command(YC, y_buf, 4); //set y address
  CS_IDLE;    
}

//Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Color_To_565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

//read graph RAM data
int16_t Read_GRAM(int16_t x, int16_t y, uint16_t *block, int16_t w, int16_t h)
{
  uint16_t ret, dummy;
    int16_t n = w * h;
    uint8_t r, g, b, tmp;
    Set_Addr_Window(x, y, x + w - 1, y + h - 1);
    while (n > 0) 
  {
        CS_ACTIVE;
    writeCmd16(RC);
        setReadDir();
    read8(r);
    while (n) 
    {
      if(R24BIT == 1)
      {
        read8(r);
        read8(g);
        read8(b);
        ret = Color_To_565(r, g, b);
      }
      else if(R24BIT == 0)
      {
        read16(ret);
      }
      *block++ = ret;
      n--;
    }
        
//        RD_IDLE;
        CS_IDLE;
        setWriteDir();
    }
  return 0;
}

//read LCD controller chip ID 
uint16_t Read_ID(void)
{
  uint16_t dst;
  setWriteDir();
  CS_ACTIVE;
  writeCmd8(0xD3);
  setReadDir();
  read16(dst);
  //Serial.println(dst); //To Debug
  read16(dst);
  //Serial.println(dst); //To Debug
  setWriteDir();
  return dst;
}

//set x,y  coordinate and color to draw a pixel point 
void Draw_Pixe(int16_t x, int16_t y, uint16_t color)
{
  if((x < 0) || (y < 0) || (x > Get_Width()) || (y > Get_Height()))
  {
    return;
  }
  Set_Addr_Window(x, y, x, y);
  CS_ACTIVE;
  writeCmdData16(CC, color);
  CS_IDLE;
}

//fill area from x to x+w,y to y+h
void Fill_Rect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  int16_t end;
  if (w < 0) 
  {
        w = -w;
        x -= w;
    }                           //+ve w
    end = x + w;
    if (x < 0)
    {
        x = 0;
    }
    if (end > Get_Width())
    {
        end = Get_Width();
    }
    w = end - x;
    if (h < 0) 
  {
        h = -h;
        y -= h;
    }                           //+ve h
    end = y + h;
    if (y < 0)
    {
        y = 0;
    }
    if (end > Get_Height())
    {
        end = Get_Height();
    }
    h = end - y;
    Set_Addr_Window(x, y, x + w - 1, y + h - 1);//set area
  CS_ACTIVE;
  writeCmd8(CC);  
  if (h > w) 
  {
        end = h;
        h = w;
        w = end;
    }
  while (h-- > 0) 
  {
    end = w;
    do 
    {
        writeData16(color);//set color data
        } while (--end != 0);
  }
  CS_IDLE;
}

//Scroll display 
void Vert_Scroll(int16_t top, int16_t scrollines, int16_t offset)
{
    int16_t bfa = HEIGHT - top - scrollines; 
    int16_t vsp;
    int16_t sea = top;
    if (offset <= -scrollines || offset >= scrollines)
    {
    offset = 0; //valid scroll
    }
  vsp = top + offset; // vertical start position
    if (offset < 0)
    {
        vsp += scrollines;          //keep in unsigned range
    }
    sea = top + scrollines - 1;

  uint8_t d[6];           // for multi-byte parameters
  d[0] = top >> 8;        //TFA
  d[1] = top;
  d[2] = scrollines >> 8; //VSA
  d[3] = scrollines;
  d[4] = bfa >> 8;        //BFA
  d[5] = bfa;
  Push_Command(SC1, d, 6);
  d[0] = vsp >> 8;        //VSP
  d[1] = vsp;
  Push_Command(SC2, d, 2);
  Push_Command(0x13, NULL, 0);
}

//get lcd width
int16_t Get_Width()
{
  return width;
}

//get lcd height
int16_t Get_Height()
{
  return height;
}

//set clockwise rotation
void Set_Rotation(uint8_t r)
{
    rotation = r & 3;           // just perform the operation ourselves on the protected variables
    width = (rotation & 1) ? HEIGHT : WIDTH;
    height = (rotation & 1) ? WIDTH : HEIGHT;
  CS_ACTIVE;
  uint8_t val;
  switch (rotation) 
  {
    case 0:
      val = ILI9341_MADCTL_MX | ILI9341_MADCTL_BGR; //0 degree 
      break;
    case 1:
      val = ILI9341_MADCTL_MV | ILI9341_MADCTL_ML | ILI9341_MADCTL_BGR ; //90 degree 
      break;
    case 2:
      val = ILI9341_MADCTL_MY |ILI9341_MADCTL_BGR; //180 degree 
      break;
    case 3:
      val = ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | ILI9341_MADCTL_MV | ILI9341_MADCTL_BGR; //270 degree
      break;
  }
    writeCmdData8(MD, val); 

  Set_Addr_Window(0, 0, width - 1, height - 1);
  Vert_Scroll(0, HEIGHT, 0);
  CS_IDLE;
}

//get current rotation
//0  :  0 degree 
//1  :  90 degree
//2  :  180 degree
//3  :  270 degree
uint8_t Get_Rotation()
{
  return rotation;
}

//Anti color display 
void Invert_Display(boolean i)
{
  CS_ACTIVE;
  uint8_t val = VL^i;
  writeCmd8(val ? 0x21 : 0x20);
  CS_IDLE;
}

void start(uint16_t ID)
{
  reset();
  delay(200);
  switch(ID)
  {
    case 0x9486:
      const uint8_t data_buf[] = {0x55};
      Push_Command(0x3A,data_buf,1);
      const uint8_t data_buf1[] = {};
      Push_Command(0x11,data_buf,0);
      const uint8_t data_buf2[] = {0x28};
      Push_Command(0x36,data_buf,1);
      const uint8_t data_buf3[] = {};
      Push_Command(0x29,data_buf,0);
      break;
    default:
      break;    
  }
  Set_Rotation(rotation); 
  Invert_Display(false);
}

//LCD_KBV_END

//LCD_GUI

#define swap(a, b) { int16_t t = a; a = b; b = t; }


//set 16bits draw color
void Set_Draw_color(uint16_t color)
{
  draw_color = color;
}

//set 8bits r,g,b color
void Set_Draw_color(uint8_t r, uint8_t g, uint8_t b)
{
  draw_color = Color_To_565(r, g, b);
}

//get draw color
uint16_t Get_Draw_color() 
{
  return draw_color;
}

//draw a pixel point
void Draw_Pixel(int16_t x, int16_t y)
{
  Draw_Pixe(x, y, draw_color);
}

//read color data for point(x,y)
uint16_t Read_Pixel(int16_t x, int16_t y)
{
  uint16_t colour;
  Read_GRAM(x, y, &colour, 1, 1);
  return colour;
} 

//fill a rectangle
void Fill_Rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  int w = x2 - x1 + 1, h = y2 - y1 + 1;
    if (w < 0) 
  { 
    x1 = x2; 
    w = -w; 
  }
  if (h < 0) 
  { 
    y1 = y2; 
    h = -h; 
  }
  Fill_Rect(x1, y1, w, h, draw_color);
}

//draw a vertical line 
void Draw_Fast_VLine(int16_t x, int16_t y, int16_t h)
{
  Fill_Rect(x, y, 1, h, draw_color);
}

//draw a horizontal line
void Draw_Fast_HLine(int16_t x, int16_t y, int16_t w)
{
  Fill_Rect(x, y, w, 1, draw_color);
}

//Fill the full screen with color
void Fill_Screen(uint16_t color)
{
  Fill_Rect(0, 0, Get_Width(), Get_Height(), color);
}

//Fill the full screen with r,g,b
void Fill_Screen(uint8_t r, uint8_t g, uint8_t b)
{
  uint16_t color = Color_To_565(r, g, b);
  Fill_Rect(0, 0, Get_Width(), Get_Height(), color);
}

//draw an arbitrary line from (x1,y1) to (x2,y2)
void Draw_Line(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  int16_t steep = abs(y2 - y1) > abs(x2 - x1);
    if (steep) 
  {
      swap(x1, y1);
      swap(x2, y2);
  }
  if (x1 > x2) 
  {
      swap(x1, x2);
      swap(y1, y2);
    }
  
    int16_t dx, dy;
    dx = x2 - x1;
    dy = abs(y2 - y1);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y1 < y2) 
  {
      ystep = 1;
    } 
  else 
  {
      ystep = -1;
  }

  for (; x1<=x2; x1++) 
  {
      if (steep) 
    {
          Draw_Pixel(y1, x1);
      } 
    else 
    {
          Draw_Pixel(x1, y1);
      }
      err -= dy;
      if (err < 0) 
    {
      y1 += ystep;
      err += dx;
      }
    }
}

//draw a rectangle
void Draw_Rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{ 
  int16_t w = x2 - x1 + 1, h = y2 - y1 + 1;
  if (w < 0) 
  { 
    x1 = x2; 
    w = -w; 
  }
  if (h < 0) 
  { 
    y1 = y2; 
    h = -h; 
  }
  Draw_Fast_HLine(x1, y1, w);
    Draw_Fast_HLine(x1, y2, w);
  Draw_Fast_VLine(x1, y1, h);
  Draw_Fast_VLine(x2, y1, h);
}

//draw a round rectangle
void Draw_Round_Rectangle(int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint8_t radius)
{
  int w = x2 - x1 + 1, h = y2 - y1 + 1;
  if (w < 0) 
  { 
    x1 = x2; 
    w = -w; 
  }
  if (h < 0) 
  { 
    y1 = y2; 
    h = -h; 
  }
  Draw_Fast_HLine(x1+radius, y1, w-2*radius); 
  Draw_Fast_HLine(x1+radius, y1+h-1, w-2*radius); 
  Draw_Fast_VLine(x1, y1+radius, h-2*radius); 
    Draw_Fast_VLine(x1+w-1, y1+radius, h-2*radius);
  Draw_Circle_Helper(x1+radius, y1+radius, radius, 1);
  Draw_Circle_Helper(x1+w-radius-1, y1+radius, radius, 2);
  Draw_Circle_Helper(x1+w-radius-1, y1+h-radius-1, radius, 4);
  Draw_Circle_Helper(x1+radius, y1+h-radius-1, radius, 8);
}

//fill a round rectangle
void Fill_Round_Rectangle(int16_t x1, int16_t y1, int16_t x2,int16_t y2, int16_t radius)
{
  int w = x2 - x1 + 1, h = y2 - y1 + 1;
  if (w < 0) 
  { 
    x1 = x2; 
    w = -w; 
  }
  if (h < 0) 
  { 
    y1 = y2; 
    h = -h; 
  }
  Fill_Rect(x1+radius, y1, w-2*radius, h, draw_color);
  Fill_Circle_Helper(x1+w-radius-1, y1+radius, radius, 1, h-2*radius-1);
  Fill_Circle_Helper(x1+radius, y1+radius, radius, 2, h-2*radius-1);  
}

//draw a circle
void Draw_Circle(int16_t x, int16_t y, int16_t radius)
{
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t x1= 0;
  int16_t y1= radius;

  Draw_Pixel(x, y+radius);
  Draw_Pixel(x, y-radius);
  Draw_Pixel(x+radius, y);
  Draw_Pixel(x-radius, y);

  while (x1<y1) 
  {
      if (f >= 0) 
    {
          y1--;
          ddF_y += 2;
          f += ddF_y;
      }
      x1++;
      ddF_x += 2;
      f += ddF_x;
  
    Draw_Pixel(x + x1, y + y1);
      Draw_Pixel(x - x1, y + y1);
    Draw_Pixel(x + x1, y - y1);
    Draw_Pixel(x - x1, y - y1);
    Draw_Pixel(x + y1, y + x1);
    Draw_Pixel(x - y1, y + x1);
    Draw_Pixel(x + y1, y - x1);
    Draw_Pixel(x - y1, y - x1);
  }
}

//draw a circular bead
void Draw_Circle_Helper(int16_t x0, int16_t y0, int16_t radius, uint8_t cornername)
{
  int16_t f     = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t x     = 0;
  int16_t y     = radius;
  while (x<y) 
  {
      if (f >= 0) 
    {
          y--;
          ddF_y += 2;
          f += ddF_y;
      }
      x++;
      ddF_x += 2;
      f += ddF_x;
      if (cornername & 0x4) 
    {
      Draw_Pixel(x0 + x, y0 + y);
      Draw_Pixel(x0 + y, y0 + x);
      } 
      if (cornername & 0x2) 
    {
      Draw_Pixel(x0 + x, y0 - y);
      Draw_Pixel(x0 + y, y0 - x);
      }
      if (cornername & 0x8) 
    {
      Draw_Pixel(x0 - y, y0 + x);
      Draw_Pixel(x0 - x, y0 + y);
      }
      if (cornername & 0x1)
    {
      Draw_Pixel(x0 - y, y0 - x);
      Draw_Pixel(x0 - x, y0 - y);
      }
    }
}

//fill a circle
void Fill_Circle(int16_t x, int16_t y, int16_t radius)
{
  Draw_Fast_VLine(x, y-radius, 2*radius+1);
  Fill_Circle_Helper(x, y, radius, 3, 0);
}

//fill a semi-circle
void Fill_Circle_Helper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,int16_t delta)
{
  int16_t f     = 1 - r;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * r;
  int16_t x     = 0;
  int16_t y     = r;

  while (x<y) 
  {
      if (f >= 0) 
    {
          y--;
          ddF_y += 2;
          f += ddF_y;
      }
      x++;
      ddF_x += 2;
      f += ddF_x;

      if (cornername & 0x1) 
    {
          Draw_Fast_VLine(x0+x, y0-y, 2*y+1+delta);
          Draw_Fast_VLine(x0+y, y0-x, 2*x+1+delta);
      }
      if (cornername & 0x2) 
    {
          Draw_Fast_VLine(x0-x, y0-y, 2*y+1+delta);
          Draw_Fast_VLine(x0-y, y0-x, 2*x+1+delta);
      }
    }
}

//draw a triangle
void Draw_Triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2)
{
  Draw_Line(x0, y0, x1, y1);
  Draw_Line(x1, y1, x2, y2);
    Draw_Line(x2, y2, x0, y0);
}

//fill a triangle
void Fill_Triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2)
{
  int16_t a, b, y, last;
    if (y0 > y1) 
  {
      swap(y0, y1); 
    swap(x0, x1);
    }
    if (y1 > y2) 
  {
      swap(y2, y1); 
    swap(x2, x1);
    }
    if (y0 > y1) 
  {
      swap(y0, y1); 
    swap(x0, x1);
    }

  if(y0 == y2) 
  { 
      a = b = x0;
      if(x1 < a)
      {
      a = x1;
      }
      else if(x1 > b)
      {
      b = x1;
      }
      if(x2 < a)
      {
      a = x2;
      }
      else if(x2 > b)
      {
      b = x2;
      }
      Draw_Fast_HLine(a, y0, b-a+1);
      return;
  }
    int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0, dx12 = x2 - x1, dy12 = y2 - y1;
  int32_t sa = 0, sb = 0;
  if(y1 == y2)
  {
    last = y1; 
  }
    else
    {
    last = y1-1; 
    }

    for(y=y0; y<=last; y++) 
  {
      a   = x0 + sa / dy01;
      b   = x0 + sb / dy02;
      sa += dx01;
      sb += dx02;
      if(a > b)
      {
      swap(a,b);
      }
      Draw_Fast_HLine(a, y, b-a+1);
  }
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) 
  {
      a   = x1 + sa / dy12;
      b   = x0 + sb / dy02;
      sa += dx12;
      sb += dx02;
      if(a > b)
      {
      swap(a,b);
      }
    Draw_Fast_HLine(a, y, b-a+1);
  }
}


//set text coordinate
void Set_Text_Cousur(int16_t x, int16_t y)
{
  text_x = x;
  text_y = y;
}

//get text x coordinate
int16_t Get_Text_X_Cousur() 
{
  return text_x;
}

//get text y coordinate
int16_t Get_Text_Y_Cousur() 
{
  return text_y;
}

//set text colour with 16bit color
void Set_Text_colour(uint16_t color)
{
  text_color = color;
}

//set text colour with 8bits r,g,b
void Set_Text_colour(uint8_t r, uint8_t g, uint8_t b)
{
  text_color = Color_To_565(r, g, b);
}

//get text colour
uint16_t Get_Text_colour()
{
  return text_color;
}

//set text background colour with 16bits color
void Set_Text_Back_colour(uint16_t color)
{
  text_bgcolor = color; 
}

//set text background colour with 8bits r,g,b
void Set_Text_Back_colour(uint8_t r, uint8_t g, uint8_t b)
{
  text_bgcolor = Color_To_565(r, g, b);
}

//get text background colour
uint16_t Get_Text_Back_colour()
{
  return text_bgcolor;
}

//set text size
void Set_Text_Size(uint8_t s)
{
  text_size = s;
}

//get text size
uint8_t Get_Text_Size()
{
  return text_size;
}

//set text mode
void Set_Text_Mode(boolean mode)
{
  text_mode = mode;
}

//get text mode
boolean Get_Text_Mode()
{
  return text_mode;
}

//draw a char
void Draw_Char(int16_t x, int16_t y, uint8_t c, uint16_t color,uint16_t bg, uint8_t size, boolean mode)
{
  if((x >= Get_Width()) || (y >= Get_Height()) || ((x + 6 * size - 1) < 0) || ((y + 8 * size - 1) < 0))
  {
      return;
  }   
    if(c >= 176)
    {
    c++; 
    }
  for (int8_t i=0; i<6; i++) 
  {
      uint8_t line;
      if (i == 5)
      {
          line = 0x0;
      }
      else
      {
          line = lcd_font+(c*5)+i;
      }
      for (int8_t j = 0; j<8; j++) 
    {
          if (line & 0x1) 
      {
            if (size == 1)
            {
              Draw_Pixe(x+i, y+j, color);
            }
            else 
        {  
          Fill_Rect(x+(i*size), y+(j*size), size, size, color);
            }
          } 
      else if (bg != color)         
      {
        if(!mode)
        {
              if (size == 1) 
              {
                Draw_Pixe(x+i, y+j, bg);
              }
              else 
          {  
            Fill_Rect(x+i*size, y+j*size, size, size, bg);
          }
        }
      }
          line >>= 1;
      }
    }
}

//print string
size_t Print(uint8_t *st, int16_t x, int16_t y)
{
  int16_t pos;
  uint16_t len;
  const char * p = (const char *)st;
  size_t n = 0;
  if (x == CENTER || x == RIGHT) 
  {
    len = mystrlen((const char *)st) * 6 * text_size;   
    pos = (Get_Display_Width() - len); 
    if (x == CENTER)
    {
      x = pos/2;
    }
    else
    {
      x = pos - 1;
    }
  }
    Set_Text_Cousur(x, y);
  while(1)
  {
    unsigned char ch = *(p++);
    if(ch == 0)
    {
      break;
    }
    if(write(ch))
    {
      n++;
    }
    else
    {
      break;
    }
  } 
  return n;
}

int mystrlen(char *str)
{
    int len = 0;
    int i;
    
    for (i=0; str[i] != 0; i++) 
    {
        len++;
    }
    return(len-1);
}

//print string
void Print_String(const uint8_t *st, int16_t x, int16_t y)
{
  Print((uint8_t *)st, x, y);
}

//print string
void Print_String(uint8_t *st, int16_t x, int16_t y)
{
  Print(st, x, y);
}

//print int number
void Print_Number_Int(long num, int16_t x, int16_t y, int16_t length, uint8_t filler, int16_t system)
{
  uint8_t st[27] = {0};
  uint8_t *p = st+26;
  boolean flag = false;
  int16_t len = 0,nlen = 0,left_len = 0,i = 0;
  *p = '\0';
  if(0 == num)
  {
    *(--p) = '0';
    len = 1;
  }
  else
  {
    if(num < 0)
    {
      num = -num;
      flag = true;
    }   
  }
  while((num > 0) && (len < 10))
  {
    if(num%system > 9)
    {
      *(--p) = 'A' + (num%system-10);
    }
    else
    {
      *(--p) = '0' + num%system;
    }
    num = num/system;
    len++;
  }
  if(flag)
  {
    *(--p) = '-';
  }
  if(length > (len + flag + 1))
  {
    if(length > sizeof(st))
    {
      nlen = sizeof(st) - len - flag - 1;
    }
    else
    {
      nlen = length - len - flag - 1;
    }
    for(i = 0;i< nlen;i++)
    {
      *(--p) = filler;
    }
    left_len = sizeof(st) - nlen - len - flag - 1;
  } 
  else
  {
    left_len = sizeof(st) - len - flag - 1;
  }
  for(i = 0; i < (sizeof(st) - left_len);i++)
  {
    st[i] = st[left_len + i];
  }
  st[i] = '\0';
  Print(st, x, y);
}


//write a char
size_t write(uint8_t c) 
{
  if (c == '\n') 
  {
      text_y += text_size*8;
      text_x  = 0;
  } 
  else if(c == '\r')
  {
  }
  else 
  {
      Draw_Char(text_x, text_y, c, text_color, text_bgcolor, text_size,text_mode);
      text_x += text_size*6;    
    } 
    return 1; 
}

//get lcd width
int16_t Get_Display_Width()
{
  return Get_Width();
}

//get lcd height 
int16_t Get_Display_Height() 
{
  return Get_Height();
}

//LCD_GUI_END
