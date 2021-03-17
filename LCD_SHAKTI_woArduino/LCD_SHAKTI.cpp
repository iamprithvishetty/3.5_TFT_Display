#include "LCD_SHAKTI.h"
#include "lcd_registers.h"
#include "8bit_parallel.h"
#include "stdbool.h"
//#include "Arduino.h"

#define XC ILI9341_COLADDRSET
#define YC ILI9341_PAGEADDRSET
#define CC ILI9341_MEMORYWRITE
#define RC HX8357_RAMRD
#define SC1 0x33
#define SC2 0x37
#define MD ILI9341_MEMORYACCESS
#define VL 0
#define R24BIT 0

#define LEFT 0
#define RIGHT 9999
#define CENTER 9998

LCD_SHAKTI::LCD_SHAKTI( uint16_t LCD_ID,uint16_t wid, uint16_t heigh)
{
    width = wid;
    height = heigh;
    WIDTH = width;
    HEIGHT = height;
    lcd_model = LCD_ID;
    //Serial.println("Hey");

}

// LCD_SHAKTI::LCD_SHAKTI( uint16_t wid, uint16_t heigh)
// {
//     width = wid;
//     height = heigh;
//     WIDTH = width;
//     HEIGHT = height;
//     lcd_model = 0xFFFF;
//     Serial.println("Hey");

// }


void LCD_SHAKTI::Reset(void)
{
    CS_IDLE;
    RD_IDLE;
    WR_IDLE;
    RES_IDLE;
    //delay(2);
    RES_ACTIVE;
    CS_ACTIVE;
    CD_COMMAND;
    setWriteDir();
    write8(0x00);
    for(uint8_t i=0; i<3; i++)
    {
      WR_STROBE; // Three extra 0x00s
    }
    CS_IDLE;
}

//read LCD controller chip ID 
uint16_t LCD_SHAKTI::Read_ID(void)
{
  uint16_t ID;
  setWriteDir();
  CS_ACTIVE;
  writeCmd8(0xD3);
  setReadDir();
  read16(ID);
  //Serial.println(dst); //To Debug
  read16(ID);
  //Serial.println(dst); //To Debug
  setWriteDir();
  return ID;
}

void LCD_SHAKTI::Init_LCD(void)
{
  SET_CNTL_BITS; // SET Control Bits to OUTPUT
  RES_ACTIVE;  // RESET pin HIGH
  Reset();
  if(lcd_model == 0xFFFF)
  {
    lcd_model = Read_ID(); 
  }
  Start(0x9486);

}

void LCD_SHAKTI::Start(uint16_t ID)
{
  Reset();
  //delay(200);
  switch(ID)
  {
    case 0x9486:
      uint8_t data_buf[] = {0x55};
      Push_Command(0x3A,data_buf,1);
      uint8_t data_buf1[] = {};
      Push_Command(0x11,data_buf1,0);
      uint8_t data_buf2[] = {0x28};
      Push_Command(0x36,data_buf2,1);
      uint8_t data_buf3[] = {};
      Push_Command(0x29,data_buf3,0);
      break;
    default:
      break;    
  }
  //Set_Rotation(rotation); 
  //Invert_Display(0);
}

void LCD_SHAKTI::Push_Command(uint16_t cmd, uint8_t *block, int8_t N)
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

void LCD_SHAKTI::Set_Rotation(uint8_t r)
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

void LCD_SHAKTI::Set_Addr_Window(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
  CS_ACTIVE;
  uint8_t x_buf[] = {x1>>8,x1&0xFF,x2>>8,x2&0xFF}; 
  uint8_t y_buf[] = {y1>>8,y1&0xFF,y2>>8,y2&0xFF}; 

  Push_Command(XC, x_buf, 4); //set x address
  Push_Command(YC, y_buf, 4); //set y address
  CS_IDLE;    
}

void LCD_SHAKTI::Vert_Scroll(int16_t top, int16_t scrollines, int16_t offset)
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
uint16_t LCD_SHAKTI::Get_Width()
{
  return width;
}

//get lcd height
uint16_t LCD_SHAKTI::Get_Height()
{
  return height;
}

void LCD_SHAKTI::Invert_Display(bool i)
{
  CS_ACTIVE;
  uint8_t val = VL^i;
  writeCmd8(val ? 0x21 : 0x20);
  CS_IDLE;
}