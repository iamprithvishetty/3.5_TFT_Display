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
  write_word(GPIO_DATA_REG,*GPIO_DATA_REG|0xD3);
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
  //Read first byte-garbage
  RD_ACTIVE; 
  write_word(GPIO_DIRECTION_CNTRL_REG,*GPIO_DIRECTION_CNTRL_REG & 0xFFFFFF00);
  delayMicroseconds(2);
  Serial.print(digitalRead(GPIO0));
  Serial.print(digitalRead(GPIO1));
  Serial.print(digitalRead(GPIO2));
  Serial.print(digitalRead(GPIO3));
  Serial.print(digitalRead(GPIO4));
  Serial.print(digitalRead(GPIO5));
  Serial.print(digitalRead(GPIO6));
  Serial.println(digitalRead(GPIO7));
  RD_IDLE;
  
  
  //Read 2nd byte-garbage
  RD_ACTIVE;
  delayMicroseconds(2);
  Serial.print(digitalRead(GPIO0));
  Serial.print(digitalRead(GPIO1));
  Serial.print(digitalRead(GPIO2));
  Serial.print(digitalRead(GPIO3));
  Serial.print(digitalRead(GPIO4));
  Serial.print(digitalRead(GPIO5));
  Serial.print(digitalRead(GPIO6));
  Serial.println(digitalRead(GPIO7));
  RD_IDLE;
  //Read 3rd byte-garbage
  RD_ACTIVE;
  delayMicroseconds(1);
  Serial.print(digitalRead(GPIO0));
  Serial.print(digitalRead(GPIO1));
  Serial.print(digitalRead(GPIO2));
  Serial.print(digitalRead(GPIO3));
  Serial.print(digitalRead(GPIO4));
  Serial.print(digitalRead(GPIO5));
  Serial.print(digitalRead(GPIO6));
  Serial.println(digitalRead(GPIO7));
  RD_IDLE;
  //Read 3rd byte-garbage
  
  RD_ACTIVE; 
  delayMicroseconds(1);
  Serial.print(digitalRead(GPIO0));
  Serial.print(digitalRead(GPIO1));
  Serial.print(digitalRead(GPIO2));
  Serial.print(digitalRead(GPIO3));
  Serial.print(digitalRead(GPIO4));
  Serial.print(digitalRead(GPIO5));
  Serial.print(digitalRead(GPIO6));
  Serial.println(digitalRead(GPIO7));
  RD_IDLE;
}

void loop() {


}
