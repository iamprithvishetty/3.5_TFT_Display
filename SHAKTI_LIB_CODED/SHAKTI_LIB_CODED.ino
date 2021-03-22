#include <LCD_SHAKTI.h>
#include <math.h> //Math Library

#define MAX_VALUE 60
#define PI 3.141592653589793238

uint8_t cps=10;
uint8_t previous_cps_bar=0;
uint8_t previous_cps_curve=0;
bool flag_init=false;
bool flag_curve=false;
int time_now = millis();

LCD_SHAKTI my_lcd(0x9486,480,320);

void setup() {
  my_lcd.Init_LCD();
  my_lcd.Set_Rotation(1);
  my_lcd.Fill_Screen(0, 0, 0);
}

void loop() {
  
if(millis()-time_now>1000)
{
  time_now = millis();
  if(cps>=60)
  {
    cps=0;
  }
  else
  {
    cps=cps+1;
  }
}
Bar_Create_Update(15, 50, 65, 305, MAX_VALUE);
//Curve_Create_Update(280,300,MAX_VALUE,150);

}

void Bar_Create_Update(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t max_val)
{
  if(flag_init == false)
  {
  my_lcd.Set_Draw_color(198, 144, 245);
  my_lcd.Draw_Rectangle(x1, y1, x2,y2);
  my_lcd.Set_Draw_color(59, 33, 82);
  my_lcd.Fill_Rectangle(x1+1, y1+1, x2-1,y2-1);

  uint8_t count_x;
  uint8_t count_y;
  
  for(count_y=0;count_y<11;count_y++)
  {   
    for(count_x=0;count_x<8;count_x++)
    {
    my_lcd.Set_Draw_color(255, 255, 255);
    my_lcd.Draw_Pixel( x2+count_x, y2-((y2-y1)*0.1*count_y));
    }

    my_lcd.Set_Text_colour(255, 255, 255);
    my_lcd.Set_Text_Size(1);
    my_lcd.Set_Text_Mode(1);
    my_lcd.Print_Number_Int(long(max_val*count_y*0.1), x2+count_x+2, y2-((y2-y1)*0.1*count_y)-4, 3,' ', 10);
    
  }
  flag_init = true;
  }

  uint8_t threshold1 = max_val/3;
  uint8_t threshold2 = (max_val*2)/3;

 if(cps != previous_cps_bar)
 {
  previous_cps_bar =cps; 
  if(cps<threshold1)
  {
    my_lcd.Set_Draw_color(59, 33, 82);
    my_lcd.Fill_Rectangle(x1+1, y1+1, x2-1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val))));
    my_lcd.Set_Draw_color(84, 209, 91);
    my_lcd.Fill_Rectangle(x1+1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val))), x2-1,y2-1);
    
  }

  else if(cps<threshold2)
  {
    my_lcd.Set_Draw_color(59, 33, 82);
    my_lcd.Fill_Rectangle(x1+1, y1+1, x2-1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val))));
    my_lcd.Set_Draw_color(222, 115, 33);
    my_lcd.Fill_Rectangle(x1+1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val))), x2-1,y2-1);
  }

  else if(cps<=max_val)
  {
    my_lcd.Set_Draw_color(59, 33, 82);
    my_lcd.Fill_Rectangle(x1+1, y1+1, x2-1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val))));
    my_lcd.Set_Draw_color(204, 30, 24);
    my_lcd.Fill_Rectangle(x1+1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val))), x2-1,y2-1);
  }

    my_lcd.Set_Text_colour(255, 255, 255);
    my_lcd.Set_Text_Size(3);
    my_lcd.Set_Text_Mode(0);
    my_lcd.Set_Text_Back_colour(0,0,0);
    my_lcd.Print_Number_Int(long(cps), x1+9, y1-30, 3,' ', 10);
    //my_lcd.Print_String(var, x1+9, y1-30);

 }
  
}

void Curve_Create_Update(uint16_t x1,uint16_t y1,uint8_t max_val, uint8_t magnitude)
{
  uint16_t angle;
  uint16_t x_val,y_val;
  uint8_t magnitude_change;
  uint8_t width = 7;
  if(flag_curve == false)
  {
  for(angle =180; angle>=180-60; angle--)
  {
  for(magnitude_change=magnitude;magnitude_change>magnitude-width;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Set_Draw_color(84, 209, 91);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val);
  }
  
  }

  for(angle =120; angle>=180-120; angle--)
  {
  for(magnitude_change=magnitude;magnitude_change>magnitude-width;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Set_Draw_color(222, 115, 33);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val);
  }
  
  }

  for(angle =60; angle>0; angle--)
  {
  for(magnitude_change=magnitude;magnitude_change>magnitude-width;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Set_Draw_color(204, 30, 24);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val);
  }
  
  }
  flag_curve=true;
  }

   uint8_t threshold1 = max_val/3;
   uint8_t threshold2 = (max_val*2)/3;

 if(cps != previous_cps_curve)
 {
   previous_cps_curve = cps;
   if(cps<threshold1)
  {
  for(angle =180; angle>int(180-(float(cps)/float(max_val))*180); angle--)
  {
  for(magnitude_change=magnitude-width-10;magnitude_change>0;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Set_Draw_color(84, 209, 91);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val);
  } 
  }  

  for(angle =int(180-(float(cps)/float(max_val))*180); angle>0; angle--)
  {
  for(magnitude_change=magnitude-width-10;magnitude_change>0;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Set_Draw_color(0, 0, 0);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val);
  } 
  } 
  
  }

  else if(cps<threshold2)
  {
   for(angle =120; angle>int(180-(float(cps)/float(max_val))*180); angle--)
  {
  for(magnitude_change=magnitude-width-10;magnitude_change>0;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Set_Draw_color(222, 115, 33);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val);
  } 
  }  

  for(angle =int(180-(float(cps)/float(max_val))*180); angle>0; angle--)
  {
  for(magnitude_change=magnitude-width-10;magnitude_change>0;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Set_Draw_color(0, 0, 0);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val);
  } 
  } 
  
  }

  else if(cps<=max_val)
  {
   for(angle =60; angle>int(180-(float(cps)/float(max_val))*180); angle--)
  {
  for(magnitude_change=magnitude-width-10;magnitude_change>0;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Set_Draw_color(204, 30, 24);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val);
  } 
  }  

  for(angle =int(180-(float(cps)/float(max_val))*180); angle>0; angle--)
  {
  for(magnitude_change=magnitude-width-10;magnitude_change>0;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Set_Draw_color(0, 0, 0);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val);
  } 
  }
  }
 }
   
 } 
