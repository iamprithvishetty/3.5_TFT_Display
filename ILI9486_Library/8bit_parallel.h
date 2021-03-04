#define CS_MASK B	//define CS_MASK HEX VALUE here
#define CD_MASK	B	//define CD_MASK HEX VALUE here
#define WR_MASK	B	//define WR_MASK HEX VALUE here
#define RD_MASK	B	//define RD_MASK HEX VALUE here

#define CONTROL_MASK CS_MASK|CD_MASK|WR_MASK|RD_MASK
#define SET_CNTL_BITS {write_word(GPIO_DIRECTIONAL_REG, *GPIO_DIRECTIONAL_REG | CONTROL_MASK)}

#define CS_ACTIVE write_word(GPIO_DATA_REG, *GPIO_DATA_REG & ~CS_MASK)
#define CS_IDLE write_word(GPIO_DATA_REG, *GPIO_DATA_REG | CS_MASK)
#define CD_COMMAND write_word(GPIO_DATA_REG, *GPIO_DATA_REG & ~CD_MASK)
#define CD_DATA write_word(GPIO_DATA_REG, *GPIO_DATA_REG | CD_MASK)
#define WR_ACTIVE write_word(GPIO_DATA_REG, *GPIO_DATA_REG & ~WR_MASK)
#define WR_IDLE write_word(GPIO_DATA_REG, *GPIO_DATA_REG | WR_MASK)
#define RD_ACTIVE write_word(GPIO_DATA_REG, *GPIO_DATA_REG & ~RD_MASK)
#define RD_IDLE write_word(GPIO_DATA_REG, *GPIO_DATA_REG | RD_MASK)
#define WR_STROBE {WR_ACTIVE; WR_IDLE;}
#define RD_STROBE {RD_ACTIVE; RD_IDLE;}

#define DELAY { for(int i=0; i<1000;i++){}}

#define WRITE_MASK 0x000003FC
#define write8(d) {uint_32 hi,low; hi = d >> 8;low = d; GPIO_DATA_REG, write_word(GPIO_DATA_REG,*GPIO_DATA_REG | (WRITE_MASK & (hi | low)); WR_STROBE; }
#define read8(d) {RD_IDLE;}