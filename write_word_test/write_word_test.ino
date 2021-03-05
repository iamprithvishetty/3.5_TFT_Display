void setup() {
  
  // put your setup code here, to run once:
  write_word(GPIO_DIRECTION_CNTRL_REG,*GPIO_DIRECTION_CNTRL_REG|0x00FF);
  write_word(GPIO_DATA_REG,*GPIO_DATA_REG|0x56);//   0101 0110
}

void loop() {
  // put your main code here, to run repeatedly:

}
