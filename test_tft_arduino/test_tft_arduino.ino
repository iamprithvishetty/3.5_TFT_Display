#include "8bitparallel.h"

#define XC 0x2A
#define YC 0x2B
#define CC 0x2C

void setup() {
Serial.begin(9600);
//SET_CNTL_BITS;
//RES_ACTIVE; // RESET pin HIGH
//setWriteDir();
//RD_IDLE;
//WR_IDLE;
//CD_DATA;
//CS_ACTIVE;
//CD_COMMAND;
//write8(0x00);
//write8(0x00);
//write8(0x00);
//write8(0x00);
//CD_DATA;
//setWriteDir();
//CS_IDLE;
Init();
delay(1000);
}

void loop() 
{
  
  //Serial.println(Read_Power());
  for(int i=25;i<55;i++)
  {
    Draw_Pixe(50,i,0x0000);
  }
  delay(2000);
}

void Init()
{
 SET_CNTL_BITS;
 CS_IDLE;
 RD_IDLE;
 WR_IDLE;
 CD_DATA;
 RES_ACTIVE;
 setWriteDir();
 reset();
 Start();
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

void Start()
{
  reset();
  delay(200);
  uint8_t data_buf[] = {0x55};
  Push_Command(0x3A,data_buf,1);
  uint8_t data_buf1[] = {};
  Push_Command(0x11,data_buf,0);
  uint8_t data_buf2[] = {0x28};
  Push_Command(0x36,data_buf,1);
  uint8_t data_buf3[] = {};
  Push_Command(0x29,data_buf,0);
  
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
  if((x < 0) || (y < 0) || (x > 320) || (y > 480))
  {
    return;
  }
  Set_Addr_Window(x, y, x, y);
  CS_ACTIVE;
  writeCmdData16(CC, color);
  CS_IDLE;
}

uint16_t Read_ID()
{
  uint16_t dst;
  CS_ACTIVE;
  CD_COMMAND;
  RD_IDLE;
  WR_IDLE; 
  write8(0xD3);
  setReadDir();
  CD_DATA;
  read16(dst);
  dst=0;
  //Serial.println(dst); //To Debug
  read16(dst);
  //Serial.println(dst); //To Debug
  return dst;
}

uint16_t Read_Power()
{
  uint8_t dst;
  CS_ACTIVE;
  CD_COMMAND;
  RD_IDLE;
  WR_IDLE; 
  write8(0x0A);
  setReadDir();
  CD_DATA;
  read8(dst);
  dst=0;
  //Serial.println(dst); //To Debug
  read8(dst);
  //Serial.println(dst); //To Debug
  return dst;
}
