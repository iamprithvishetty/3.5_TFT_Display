
#define CD_COMMAND digitalWrite(GPIO9,LOW)
#define CD_DATA digitalWrite(GPIO9,HIGH)
#define CS_ACTIVE digitalWrite(GPIO8,LOW)
#define CS_IDLE digitalWrite(GPIO8,HIGH)
#define WR_ACTIVE digitalWrite(GPIO10,LOW)
#define WR_IDLE digitalWrite(GPIO10,HIGH)
#define RD_ACTIVE digitalWrite(GPIO11,LOW)
#define RD_IDLE digitalWrite(GPIO11,HIGH)
#define WR_STROBE { WR_ACTIVE; WR_IDLE; }
#define RD_STROBE {RD_IDLE; RD_ACTIVE;RD_ACTIVE;RD_ACTIVE;}

// Delay
//#define DELAY7        \
//  asm volatile(       \
//    "rjmp .+0" "\n\t" \
//    "rjmp .+0" "\n\t" \
//    "rjmp .+0" "\n\t" \
//    "nop"      "\n"   \
//    ::);

#define DELAY7 {int i=1;}

#define write8(d){ write_word(GPIO_DATA_REG,*GPIO_DATA_REG|d); WR_STROBE;};
#define read8(dst) { RD_ACTIVE; DELAY7; dst = *GPIO_DATA_REG; RD_IDLE; }

#define write16(d) { uint8_t h = (d)>>8, l = d; write8(h); write8(l); }
#define read16(dst) { uint8_t hi; read8(hi); read8(dst); dst |= (hi << 8); }
#define writeCmd8(x){ CD_COMMAND; write8(x); CD_DATA;  }
#define writeData8(x){  write8(x) }
#define writeCmd16(x){ CD_COMMAND; write16(x); CD_DATA; }
#define writeData16(x){ write16(x) }

// Set value of TFT register: 8-bit address, 8-bit value
#define writeCmdData8(a, d) { CD_COMMAND; write8(a); CD_DATA; write8(d); }    

#define writeCmdData16(a, d) { \
  uint8_t hi, lo; \
  hi = (a) >> 8; lo = (a); CD_COMMAND; write8(hi); write8(lo); \
  hi = (d) >> 8; lo = (d); CD_DATA   ; write8(hi); write8(lo); }

// To set digital pins to OUTPUT mode
void setWriteDir()
{
  write_word(GPIO_DIRECTION_CNTRL_REG,*GPIO_DIRECTION_CNTRL_REG|0x00FF);
}

// To set digital pins to INPUT mode
void setReadDir()
{
  write_word(GPIO_DIRECTION_CNTRL_REG,*GPIO_DIRECTION_CNTRL_REG & 0xFF00);
}

//Read ID of the TFT Display
uint16_t Read_ID(void)
{
  uint16_t ret;
  ret = Read_Reg(0xD3,1); //0x9341 0x9486
  if(ret == 0x9341)
  {
    return 0x9341;
  }
  else if(ret == 0x9486)
  {
    return 0x9486;
  }
  else if(ret == 0x9488)
  {
    return 0x9488;
  }
  else
  {
    return Read_Reg(0, 0); //others
  }
}


uint16_t Read_Reg(uint16_t reg, int8_t index)
{
  uint16_t ret,high;
  uint8_t low;
  
    CS_ACTIVE;
    writeCmd16(reg);
    setReadDir();
    delay(1); 
  do 
  { 
    read16(ret);  //read 16bits
  }while (--index >= 0);   
    CS_IDLE;
    setWriteDir();
    return ret;
}



void setup() {
  Serial.begin(19200);
  Serial.println(Read_ID());
}

void loop() {
  
}
