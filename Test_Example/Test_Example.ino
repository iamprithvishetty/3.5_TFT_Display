#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library
#include <math.h>

#define BUFFER 432
#define THRESHOLD 10
#define ACQ_DATA A5

static int acq_data[BUFFER];
int  acq_data_check;
float data_entry;
int value;

LCDWIKI_KBV my_lcd(320,480,A3,A2,A1,A0,A4);//height,width,cs,cd,wr,rd,reset

void setup() 
{
  Serial.begin(115200);
  Serial.print(acq_data[0]);
  Serial.println("Reset");
  my_lcd.Init_LCD();
  my_lcd.Fill_Screen(0x0000);  
  my_lcd.Set_Rotation(1); 
  draw_line_delay(0,(my_lcd.Get_Height()/2)*1.9,480,(my_lcd.Get_Height()/2)*1.9,0xFFFF,2); // x-axis
  draw_line_delay((my_lcd.Get_Width()/2)*0.1 ,0,(my_lcd.Get_Width()/2)*0.1,320,0xFFFF,2); //y-axis
}

void loop() 
{
  analog_wave();
  delay(10);
}


void draw_line(int x1, int y1,int x2,int y2, uint16_t color)
{
  my_lcd.Set_Draw_color(color);
  if((x1 < 0) || (y1 < 0) || (x1 > my_lcd.Get_Width()) || (y1 > my_lcd.Get_Height() || (x2 < 0) || (y2 < 0) || (x2 > my_lcd.Get_Width()) || (y2 > my_lcd.Get_Height())))
  {
    return;
  }
  else{
    if((x1 < x2))
    {
    
    float slope = float(y2-y1)/float(x2-x1);
    Serial.println(slope);
    for(int row = x1; row<=x2; row++)
    {
      int col_value = y1+round(slope*(row-x1));
      int row_value = row;
     my_lcd.Draw_Pixel(row_value, col_value);
    }
    }

    else if((x1>x2))
    {
    float slope = float(y1-y2)/float(x1-x2);
    Serial.println(slope);
    for(int row = x2; row<=x1; row++)
    {
      int col_value = y2+round(slope*(row-x2));
      int row_value = row;
     my_lcd.Draw_Pixel(row_value, col_value);
    }
    }
    else
    {
      if(y2>y1)
    {
     for(int col = y1; col<=y2; col++)
    {
     int col_value = col;
     my_lcd.Draw_Pixel(x1, col_value);
    }
    }
    else if(y2<y1)
    {
       for(int col = y2; col<=y1; col++)
    {
     int col_value = col;
     my_lcd.Draw_Pixel(x1, col_value);
    }
    }
    }
  }
}

void draw_line_delay(int x1, int y1,int x2,int y2, uint16_t color, int wait)
{
  my_lcd.Set_Draw_color(color);
  if((x1 < 0) || (y1 < 0) || (x1 > my_lcd.Get_Width()) || (y1 > my_lcd.Get_Height() || (x2 < 0) || (y2 < 0) || (x2 > my_lcd.Get_Width()) || (y2 > my_lcd.Get_Height())))
  {
    return;
  }
  else{
    if(x1 < x2)
    {
    
    float slope = float(y2-y1)/float(x2-x1);
    for(int row = x1; row<=x2; row++)
    {
      delay(wait);
      int col_value = y1+round(slope*(row-x1));
      int row_value = row;
     my_lcd.Draw_Pixel(row_value, col_value);
    }
    }

       else if((x1>x2))
    {
    float slope = float(y1-y2)/float(x1-x2);
    for(int row = x2; row<=x1; row++)
    {
      delay(wait);
      int col_value = y2+round(slope*(row-x2));
      int row_value = row;
     my_lcd.Draw_Pixel(row_value, col_value);
    }
    }
    else
    {
      if(y2>y1)
    {
     for(int col = y1; col<=y2; col++)
    {
      delay(wait);
     int col_value = col;
     my_lcd.Draw_Pixel(x1, col_value);
    }
    }
    else if(y2<y1)
    {
       for(int col = y2; col<=y1; col++)
    {
      delay(wait);
     int col_value = col;
     my_lcd.Draw_Pixel(x1, col_value);
    }
    }
    }
  }
}

void sin_wave()
{
 //do nothing 
}

void analog_wave()
{
 acq_data_check =  analogRead(ACQ_DATA);
  if(acq_data_check >= THRESHOLD)
  {
    Serial.println(acq_data[int(data_entry)]);
    data_entry = acq_data_check*(480-48)/(1023-11);
    value = int(data_entry);
    acq_data[value]=acq_data[value]+1;
    update_screen(48+value,304-acq_data[value]);
  }
}

void update_screen(int row, int column)
{
   my_lcd.Draw_Pixel(row,column);
}
