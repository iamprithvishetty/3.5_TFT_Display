#define RES_MASK 0x00001000 //define RES_MASK HEX VALUE here  RES : GPIO12
#define CS_MASK 0x00000800  //define CS_MASK HEX VALUE here   CS : GPIO11
#define CD_MASK 0x00000400  //define CD_MASK HEX VALUE here   CD : GPIO10
#define WR_MASK 0x00000200  //define WR_MASK HEX VALUE here   WRITE : GPIO9
#define RD_MASK 0x00000100  //define RD_MASK HEX VALUE here   READ : GPIO8

#define CONTROL_MASK CS_MASK|CD_MASK|WR_MASK|RD_MASK|RES_MASK
#define SET_CNTL_BITS write_word(GPIO_DIRECTION_CNTRL_REG, *GPIO_DIRECTION_CNTRL_REG | CONTROL_MASK)



#define CS_ACTIVE write_word(GPIO_DATA_REG, *GPIO_DATA_REG & ~CS_MASK)
#define CS_IDLE write_word(GPIO_DATA_REG, *GPIO_DATA_REG | CS_MASK)
#define CD_COMMAND write_word(GPIO_DATA_REG, *GPIO_DATA_REG & ~CD_MASK)
#define CD_DATA write_word(GPIO_DATA_REG, *GPIO_DATA_REG | CD_MASK)
#define WR_ACTIVE write_word(GPIO_DATA_REG, *GPIO_DATA_REG & ~WR_MASK) 
#define WR_IDLE write_word(GPIO_DATA_REG, *GPIO_DATA_REG | WR_MASK)
#define RD_ACTIVE write_word(GPIO_DATA_REG, *GPIO_DATA_REG & ~RD_MASK)
#define RD_IDLE write_word(GPIO_DATA_REG, *GPIO_DATA_REG | RD_MASK)
#define RES_ACTIVE write_word(GPIO_DATA_REG, *GPIO_DATA_REG | RES_MASK)
#define WR_STROBE {WR_ACTIVE; WR_IDLE;}
#define RD_STROBE {RD_ACTIVE; RD_IDLE;}

#define DELAY for(int i=0; i<1;i++){/*for(int j=0;j<1;j++){}*/}

#define DATA_MASK 0x000000FF

#define write8(d) {write_word(GPIO_DATA_REG,*GPIO_DATA_REG | d); WR_STROBE; }
#define read8(dst) {RD_IDLE;DELAY;dst=read_word(GPIO_DATA_REG);RD_ACTIVE;}

#define write16(d) { uint8_t h = (d)>>8, l = d; write8(h); write8(l); }
#define read16(dst) { uint8_t hi; read8(hi); read8(dst); dst |= (hi << 8); }
#define writeCmd8(x){ CD_COMMAND; write8(x); CD_DATA;  }
#define writeData8(x){  write8(x) }
#define writeCmd16(x){ CD_COMMAND; write16(x); CD_DATA; }
#define writeData16(x){ write16(x) }

#define setWriteDir() { write_word(GPIO_DIRECTION_CNTRL_REG,*GPIO_DIRECTION_CNTRL_REG|0x000000FF);}
#define setReadDir()  { write_word(GPIO_DIRECTION_CNTRL_REG,*GPIO_DIRECTION_CNTRL_REG & ~DATA_MASK); }

// Set value of TFT register: 8-bit address, 8-bit value
#define writeCmdData8(a, d) { CD_COMMAND; write8(a); CD_DATA; write8(d); }

// Set value of TFT register: 16-bit address, 16-bit value
// See notes at top about macro expansion, hence hi & lo temp vars
#define writeCmdData16(a, d) { \
  uint8_t hi, lo; \
  hi = (a) >> 8; lo = (a); CD_COMMAND; write8(hi); write8(lo); \
  hi = (d) >> 8; lo = (d); CD_DATA   ; write8(hi); write8(lo); }
