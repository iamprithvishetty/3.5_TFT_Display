#include "8bit_parallel.h"

#define ILI9341_COLADDRSET 0x2A
#define ILI9341_PAGEADDRSET 0x2B
#define ILI9341_MEMORYWRITE 0x2C

#define CC ILI9341_MEMORYWRITE
#define XC ILI9341_COLADDRSET
#define YC ILI9341_PAGEADDRSET
#define TFTLCD_DELAY8 0x7F

uint16_t dest;

void setup() 
{
SET_CNTL_BITS; // SET Control Bits to OUTPUT
RES_ACTIVE; // RESET pin HIGH    
CS_ACTIVE;
RD_IDLE;
WR_IDLE;
setWriteDir();
//writeCmd8(0x00);
writeCmd8(0x29);
writeCmd8(0x2A);
writeData16(20);
writeData16(300);
writeCmd8(0x2B);
writeData16(20);
writeData16(300);
writeCmd8(0x2E);
}

void loop() 
{
  read16(dest);
  Serial.println(dest);

}

uint16_t Read_ID()
{
  uint16_t dst;
  SET_CNTL_BITS; // SET Control Bits to OUTPUT
  RES_ACTIVE; // RESET pin HIGH
  setWriteDir(); 
  writeCmd8(0xD3);
  setReadDir();
  read16(dst);
  //Serial.println(dst); //To Debug
  read16(dst);
  //Serial.println(dst); //To Debug
  return dst;
}

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

void Set_Addr_Window(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  CS_ACTIVE;
  uint8_t x_buf[] = {x1>>8,x1&0xFF,x2>>8,x2&0xFF}; 
  uint8_t y_buf[] = {y1>>8,y1&0xFF,y2>>8,y2&0xFF}; 

  Push_Command(XC, x_buf, 4); //set x address
  Push_Command(YC, y_buf, 4); //set y address
  CS_IDLE;    
}

void Draw_Pixe(int16_t x, int16_t y, uint16_t color)
{
  if((x < 0) || (y < 0) || (x > 480) || (y > 320))
  {
    return;
  }
  Set_Addr_Window(x, y, x, y);
  CS_ACTIVE;
  writeCmdData16(CC, color);
  CS_IDLE;
}
void init_table8(const void *table, int16_t size)
{
  uint8_t i;
    uint8_t *p = (uint8_t *) table, dat[24];            //R61526 has GAMMA[22] 
    while (size > 0) 
  {
        uint8_t cmd = pgm_read_byte(p++);
        uint8_t len = pgm_read_byte(p++);
        if (cmd == TFTLCD_DELAY8) 
    {
            delay(len);
            len = 0;
        } 
    else 
    {
            for (i = 0; i < len; i++)
            {
                dat[i] = pgm_read_byte(p++);
            }
      Push_Command(cmd,dat,len);
        }
        size -= len + 2;
    }
}
 void Init_LCD(void)
{ 
  uint16_t lcd_model =0xFFFF;
  if(lcd_model == 0xFFFF)
  {
    lcd_model = Read_ID(); 
  }
//  uint16_t ID = Read_ID(); 
  start();
//  Set_Rotation(0);
}

void start()
{

      //lcd_driver = ID_9486;
      //WIDTH = 320,HEIGHT = 480;
      //width = WIDTH, height = HEIGHT;
      //XC=ILI9341_COLADDRSET,YC=ILI9341_PAGEADDRSET,CC=ILI9341_MEMORYWRITE,RC=HX8357_RAMRD,SC1=0x33,SC2=0x37,MD=ILI9341_MADCTL,VL=0,R24BIT=0;
      static const uint8_t ILI9486_regValues[] PROGMEM = 
      {
        0xF1, 6, 0x36, 0x04, 0x00, 0x3C, 0x0F, 0x8F,
        0xF2, 9, 0x18, 0xA3, 0x12, 0x02, 0xB2, 0x12, 0xFF, 0x10, 0x00, 
        0xF8, 2, 0x21, 0x04,
        0xF9, 2, 0x00, 0x08,
        0x36, 1, 0x08, 
        0xB4, 1, 0x00,
        0xC1, 1, 0x41,
        0xC5, 4, 0x00, 0x91, 0x80, 0x00,
        0xE0, 15, 0x0F, 0x1F, 0x1C, 0x0C, 0x0F, 0x08, 0x48, 0x98, 0x37, 0x0A, 0x13, 0x04, 0x11, 0x0D, 0x00,
        0xE1, 15, 0x0F, 0x32, 0x2E, 0x0B, 0x0D, 0x05, 0x47, 0x75, 0x37, 0x06, 0x10 ,0x03, 0x24, 0x20, 0x00,       
        0x3A, 1, 0x55,
        0x11,0,
        0x36, 1, 0x28,
        TFTLCD_DELAY8, 120,
        0x29,0
/*
        0x01, 0,            //Soft Reset
              TFTLCD_DELAY8, 150,  // .kbv will power up with ONLY reset, sleep out, display on
              0x28, 0,            //Display Off
              0x3A, 1, 0x55,      //Pixel read=565, write=565.
              0xC0, 2, 0x0d, 0x0d,        //Power Control 1 [0E 0E]
              0xC1, 2, 0x43, 0x00,        //Power Control 2 [43 00]
              0xC2, 1, 0x00,      //Power Control 3 [33]
              0xC5, 4, 0x00, 0x48, 0x00, 0x48,    //VCOM  Control 1 [00 40 00 40]
              0xB4, 1, 0x00,      //Inversion Control [00]
              0xB6, 3, 0x02, 0x02, 0x3B,  // Display Function Control [02 02 3B] 
              0xE0, 15,0x0F, 0x24, 0x1C, 0x0A, 0x0F, 0x08, 0x43, 0x88, 0x32, 0x0F, 0x10, 0x06, 0x0F, 0x07, 0x00,
              0xE1, 15,0x0F, 0x38, 0x30, 0x09, 0x0F, 0x0F, 0x4E, 0x77, 0x3C, 0x07, 0x10, 0x05, 0x23, 0x1B, 0x00,
              0x11, 0,            //Sleep Out
              TFTLCD_DELAY8, 150,
              0x29, 0         //Display On
*/
      };
      init_table8(ILI9486_regValues, sizeof(ILI9486_regValues));
  
  
}
