#include "8bit_parallel.h"
void setup() 
{

}

void loop() 
{
  Serial.println(Read_ID());
}

uint16_t Read_ID() //Checked and Confirmed
{
  uint16_t dst;
  SET_CNTL_BITS; // SET Control Bits to OUTPUT
  RES_ACTIVE; // RESET pin HIGH
  setWriteDir();
  CS_ACTIVE;
  CD_COMMAND;
  RD_IDLE;
  WR_IDLE;  
  write8(0xD3);
  setReadDir();
  CD_DATA;
  read16(dst);
  //Serial.println(dst); //To Debug
  read16(dst);
  //Serial.println(dst); //To Debug
  return dst;
}
