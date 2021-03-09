#define RD_MASK B00000001
#define WR_MASK B00000010
#define CD_MASK B00000100
#define CS_MASK B00001000
#define RES_MASK B00010000

#define SET_CNTL_BITS DDRC = DDRC|RD_MASK|WR_MASK|CD_MASK|CS_MASK|RES_MASK

#define RD_ACTIVE PORTC = PORTC&~RD_MASK
#define RD_IDLE PORTC = PORTC|RD_MASK
#define WR_ACTIVE PORTC = PORTC&~WR_MASK
#define WR_IDLE PORTC = PORTC|WR_MASK
#define CS_ACTIVE PORTC = PORTC&~CS_MASK
#define CS_IDLE PORTC = PORTC|CS_MASK
#define CD_COMMAND PORTC = PORTC&~CD_MASK
#define CD_DATA PORTC = PORTC|CD_MASK
#define RES_IDLE PORTC = PORTC&~RES_MASK
#define RES_ACTIVE PORTC = PORTC|RES_MASK
#define WR_STROBE {WR_ACTIVE; WR_IDLE;}
#define RD_STROBE {RD_ACTIVE; RD_IDLE;}

#define DELAY for(int i=0; i<5;i++){/*for(int j=0;j<1;j++){}*/}

#define DELAY7        \
  asm volatile(       \
    "rjmp .+0" "\n\t" \
    "rjmp .+0" "\n\t" \
    "rjmp .+0" "\n\t" \
    "nop"      "\n"   \
    ::);

#define B_MASK 0x03             
#define D_MASK 0xFC 

#define write8(d) { PORTD = (PORTD & ~D_MASK) | ((d) & D_MASK); PORTB = (PORTB & ~B_MASK) | ((d) & B_MASK); WR_STROBE; }
#define read8(dst) { RD_ACTIVE; DELAY7; dst = (PIND & D_MASK) | (PINB & B_MASK); RD_IDLE; }

#define write16(d) { uint8_t h = (d)>>8, l = d; write8(h); write8(l); }
#define read16(dst) { uint8_t hi; read8(hi); read8(dst); dst |= (hi << 8); }
#define writeCmd8(x){ CD_COMMAND; write8(x); CD_DATA;  }
#define writeData8(x){  write8(x) }
#define writeCmd16(x){ CD_COMMAND; write16(x); CD_DATA; }
#define writeData16(x){ write16(x) }

#define setWriteDir() {DDRB = DDRB|B_MASK; DDRD = DDRD|D_MASK;}
#define setReadDir() {DDRB = DDRB&~B_MASK; DDRD = DDRD&~D_MASK;}

// Set value of TFT register: 8-bit address, 8-bit value
#define writeCmdData8(a, d) { CD_COMMAND; write8(a); CD_DATA; write8(d); }

// Set value of TFT register: 16-bit address, 16-bit value
// See notes at top about macro expansion, hence hi & lo temp vars
#define writeCmdData16(a, d) { \
  uint8_t hi, lo; \
  hi = (a) >> 8; lo = (a); CD_COMMAND; write8(hi); write8(lo); \
  hi = (d) >> 8; lo = (d); CD_DATA   ; write8(hi); write8(lo); }
