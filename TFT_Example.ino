#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library

#define ACQ_DATA A5
#define BUFFER_1024 1024 
#define BUFFER_768 768
#define BUFFER_512 512
#define BUFFER_256 256

LCDWIKI_KBV my_lcd(320,480,A3,A2,A1,A0,A4);//height,width,cs,cd,wr,rd,reset

float Scaler = 1;
int THRESHOLD =  10;

int acq_data[BUFFER_1024]={0}; //A
float  acq_data_check;
float data_entry;
float temp_value;
int value;

float slope;
int col_value,row_value;

const float width = my_lcd.Get_Height();
const float height = my_lcd.Get_Width();
const float x_axis_scale = 0.1; //Scale factor for X-axis
const float y_axis_scale = 0.1; //Scale factor for Y-axis

void setup() 
{
  Serial.begin(115200);
  Serial.println(height);
  
  my_lcd.Init_LCD();
  my_lcd.Fill_Screen(0x0000);  
  my_lcd.Set_Rotation(1); 
  
  draw_line( 0 , height*y_axis_scale , width , height*y_axis_scale , 0xFFFF ,2); // x-axis
  draw_line( 480-256 , 0 , 480-256 , height ,0xFFFF ,2); //y-axis
}

void loop() 
{
  //analog_wave();
  //delay(10);
}


void draw_line(int x1, int y1,int x2,int y2, uint16_t color)
{
  y1 = height - y1;
  y2 =  height - y2;
  my_lcd.Set_Draw_color(color);
  if((x1 < 0) || (y1 < 0) || (x1 > width) || (y1 > height) || (x2 < 0) || (y2 < 0) || (x2 > width) || (y2 > height))
  {
    Serial.println("Null");
    return;
  }
  else{
    if((x1 < x2))
    {
    Serial.println("3");
    slope = float(y2-y1)/float(x2-x1);
    Serial.println(slope);
    for(int row = x1; row<=x2; row++)
    {
     col_value = y1+round(slope*(row-x1));
     row_value = row;
     Serial.println(String(row_value)+" "+String(col_value));
     my_lcd.Draw_Pixel(row_value, col_value);
    }
    }

    else if((x1>x2))
    {
      Serial.println("2");
    slope = float(y1-y2)/float(x1-x2);
    Serial.println(slope);
    for(int row = x2; row<=x1; row++)
    {
     col_value = y2+round(slope*(row-x2));
     row_value = row;
     my_lcd.Draw_Pixel(row_value, col_value);
    }
    }
    else
    { 
      Serial.println("1");
      if(y2>y1)
    {
     for(int col = y1; col<=y2; col++)
    {
     col_value = col;
     my_lcd.Draw_Pixel(x1, col_value);
    }
    }
    else if(y2<y1)
    {
       for(int col = y2; col<=y1; col++)
    {
     col_value = col;
     my_lcd.Draw_Pixel(x1, col_value);
    }
    }
    }
  }
}

void draw_line(int x1, int y1,int x2,int y2, uint16_t color, int wait_t)
{
  y1 = height - y1;
  y2 =  height - y2;
  my_lcd.Set_Draw_color(color);
  if((x1 < 0) || (y1 < 0) || (x1 > width) || (y1 > height) || (x2 < 0) || (y2 < 0) || (x2 > width) || (y2 > height))
  {
    Serial.println("Null");
    return;
  }
  else{
    if((x1 < x2))
    {
    Serial.println("3");
    slope = float(y2-y1)/float(x2-x1);
    Serial.println(slope);
    for(int row = x1; row<=x2; row++)
    {
     delay(wait_t);
     col_value = y1+round(slope*(row-x1));
     row_value = row;
     Serial.println(String(row_value)+" "+String(col_value));
     my_lcd.Draw_Pixel(row_value, col_value);
    }
    }

    else if((x1>x2))
    {
    Serial.println("2");
    slope = float(y1-y2)/float(x1-x2);
    Serial.println(slope);
    for(int row = x2; row<=x1; row++)
    {
     delay(wait_t);
     col_value = y2+round(slope*(row-x2));
     row_value = row;
     my_lcd.Draw_Pixel(row_value, col_value);
    }
    }
    else
    { 
      Serial.println("1");
      if(y2>y1)
    {
     for(int col = y1; col<=y2; col++)
    {
     delay(wait_t);
     col_value = col;
     my_lcd.Draw_Pixel(x1, col_value);
    }
    }
    else if(y2<y1)
    {
       for(int col = y2; col<=y1; col++)
    {
     delay(wait_t);
     col_value = col;
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


void histogram()
{
  acq_data_check =  analogRead(ACQ_DATA);
  if(acq_data_check > THRESHOLD)
  {
    data_entry = ((acq_data_check-THRESHOLD)*(width - width*x_axis_scale))/(1023-THRESHOLD);
    
    //Serial.println(value);
    if(data_entry < BUFFER_1024 && data_entry>=0)
    {
      value = int(data_entry);
      if(acq_data[value] < 304)
      {
        
        acq_data[value]=int(acq_data[value]+1);
        //Serial.println(acq_data[value]);
        update_screen(25+value,304-acq_data[value],0xFFF);
      }
      else
      {
        Scaler = Scaler+1;
        acq_data[value]=int(acq_data[value]+1);
        my_lcd.Set_Draw_color(0x0000);
        for(int num=0; num<BUFFER_1024; num++)
        {
          temp_value = acq_data[num]*0.5;
          
          acq_data[num]= int(temp_value);
          //Serial.println(temp_value);
          for(int i=0;i<304-acq_data[num];i++)
          {
          my_lcd.Set_Draw_color(0x0000);
          my_lcd.Draw_Pixel(25+num,i);
          }
        }
      }
    }
  }
}

void update_screen(int row, int column, uint16_t color)
{
  my_lcd.Set_Draw_color(color);
   my_lcd.Draw_Pixel(row,column);
}
