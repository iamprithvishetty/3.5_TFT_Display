//#include "lcd_registers.h"
#define CD_COMMAND digitalWrite(GPIO10,LOW)
#define CD_DATA digitalWrite(GPIO10,HIGH)
#define CS_ACTIVE digitalWrite(GPIO11,LOW)
#define CS_IDLE digitalWrite(GPIO11,HIGH)
#define WR_ACTIVE digitalWrite(GPIO9,LOW)
#define WR_IDLE digitalWrite(GPIO9,HIGH)
#define RD_ACTIVE digitalWrite(GPIO8,LOW)
#define RD_IDLE digitalWrite(GPIO8,HIGH)
#define WR_RISE { WR_ACTIVE; WR_IDLE; }
#define RD_RISE {RD_ACTIVE; RD_IDLE;}


void setup() {
  Serial.begin(19200);
  pinMode(GPIO12, OUTPUT);
  pinMode(GPIO10,OUTPUT);
  pinMode(GPIO11,OUTPUT);
  pinMode(GPIO9,OUTPUT);
  pinMode(GPIO8,OUTPUT);
  
  //RESET
  digitalWrite(GPIO12,HIGH);
  digitalWrite(GPIO12,LOW);
  digitalWrite(GPIO12,HIGH);
  //ret = Read_Reg(0xD3,1);
  write_word(GPIO_DIRECTION_CNTRL_REG,*GPIO_DIRECTION_CNTRL_REG|0x000000FF);//output
  
  CS_ACTIVE;
  CD_COMMAND;
  RD_IDLE;
  WR_IDLE;
  write_word(GPIO_DATA_REG,*GPIO_DATA_REG|0x04);
  WR_ACTIVE;
  WR_IDLE;
//  Serial.print(digitalRead(GPIO0));
//  Serial.print(digitalRead(GPIO1));
//  Serial.print(digitalRead(GPIO2));
//  Serial.print(digitalRead(GPIO3));
//  Serial.print(digitalRead(GPIO4));
//  Serial.print(digitalRead(GPIO5));
//  Serial.print(digitalRead(GPIO6));
//  Serial.println(digitalRead(GPIO7));

   write_word(GPIO_DIRECTION_CNTRL_REG,*GPIO_DIRECTION_CNTRL_REG & 0xFFFFFF00);//input mode
   CD_DATA; 
  //delayMicroseconds(1);
//  pinMode(GPIO0,INPUT);
//  pinMode(GPIO1,INPUT);
//  pinMode(GPIO2,INPUT);
//  pinMode(GPIO3,INPUT);
//  pinMode(GPIO4,INPUT);
//  pinMode(GPIO5,INPUT);
//  pinMode(GPIO6,INPUT);
//  pinMode(GPIO7,INPUT);
  
  //Read first byte-garbage
  RD_ACTIVE; 
  
//  delayMicroseconds(2);
//  Serial.print(digitalRead(GPIO0));
//  Serial.print(digitalRead(GPIO1));
//  Serial.print(digitalRead(GPIO2));
//  Serial.print(digitalRead(GPIO3));
//  Serial.print(digitalRead(GPIO4));
//  Serial.print(digitalRead(GPIO5));
//  Serial.print(digitalRead(GPIO6));
//  Serial.println(digitalRead(GPIO7));
  RD_IDLE;
  for(int i=0;i<100;i++){}
  uint32_t tempreg1 = *GPIO_DATA_REG & 0x000000FF;
  //write_word(tempreg1,*GPIO_DATA_REG & 0x000000FF);
  
  //Read 2nd byte-garbage
  RD_ACTIVE;
//  delayMicroseconds(2);
//  Serial.print(digitalRead(GPIO0));
//  Serial.print(digitalRead(GPIO1));
//  Serial.print(digitalRead(GPIO2));
//  Serial.print(digitalRead(GPIO3));
//  Serial.print(digitalRead(GPIO4));
//  Serial.print(digitalRead(GPIO5));
//  Serial.print(digitalRead(GPIO6));
//  Serial.println(digitalRead(GPIO7));
  RD_IDLE;
  for(int i=0;i<1000;i++){}
  uint32_t tempreg2 = *GPIO_DATA_REG & 0x000000FF;
  //write_word(tempreg2,*GPIO_DATA_REG & 0x000000FF);
  //Read 3rd byte-garbage
  RD_ACTIVE;
//  delayMicroseconds(1);
//  Serial.print(digitalRead(GPIO0));
//  Serial.print(digitalRead(GPIO1));
//  Serial.print(digitalRead(GPIO2));
//  Serial.print(digitalRead(GPIO3));
//  Serial.print(digitalRead(GPIO4));
//  Serial.print(digitalRead(GPIO5));
//  Serial.print(digitalRead(GPIO6));
//  Serial.println(digitalRead(GPIO7));
  RD_IDLE;
  for(int i=0;i<10000;i++){}
  uint32_t tempreg3 = *GPIO_DATA_REG & 0x000000FF;
  //Read 3rd byte-garbage
  
  RD_ACTIVE; 
//  delayMicroseconds(1);
//  Serial.print(digitalRead(GPIO0));
//  Serial.print(digitalRead(GPIO1));
//  Serial.print(digitalRead(GPIO2));
//  Serial.print(digitalRead(GPIO3));
//  Serial.print(digitalRead(GPIO4));
//  Serial.print(digitalRead(GPIO5));
//  Serial.print(digitalRead(GPIO6));
//  Serial.println(digitalRead(GPIO7));
  RD_IDLE;
  for(int i=0;i<50000;i++){}
  uint32_t tempreg4 = *GPIO_DATA_REG & 0x000000FF;

  Serial.println(*tempreg1);
  Serial.println(*tempreg2);
  Serial.println(*tempreg3);
  Serial.println(*tempreg4);
 
}

void loop() {


}void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
