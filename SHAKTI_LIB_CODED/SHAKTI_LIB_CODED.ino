#include <LCD_SHAKTI.h>
#include <math.h> //Math Library

#define MAX_VALUE 60
#define BUFFER 1024

#define PI 3.141592653589793238

uint8_t cps=10;
uint8_t previous_cps_bar=0;
uint8_t previous_cps_curve=0;
uint8_t previous_cps_warn=0;
bool flag_init=false;
bool flag_curve=false;
bool flag_warn=false;
int time_now = millis();

boolean Screen1 = true;
boolean Screen2 = false;

static volatile uint16_t record_data[BUFFER]={0};
static volatile uint16_t record_data_compressed[435]={0};
static boolean flag_start=0;

LCD_SHAKTI my_lcd(0x9486,480,320);

void setup() {
  my_lcd.Init_LCD();
  my_lcd.Set_Rotation(1);
  my_lcd.Fill_Screen(0, 0, 0);
  for(int count=0;count<10000;count++)
  {
    Record_Data(random(0,1024));
  }
  Make_Axis(45,35,1024,500);
  Draw_Spectrum_Full(45,35);
  
}

void loop() {
 if(Screen1)
 {
  if(millis()-time_now>50)
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
  Curve_Create_Update(280,300,MAX_VALUE,150);
  Warning_Text(180,60,MAX_VALUE);
 }
 if(Screen2)
 {
  Record_Data(random(0,1024));
 }
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

  uint8_t cps_now = cps;
 if(cps_now != previous_cps_bar)
 {
  previous_cps_bar =cps_now; 
  if(cps_now<threshold1)
  {
    my_lcd.Set_Draw_color(59, 33, 82);
    my_lcd.Fill_Rectangle(x1+1, y1+1, x2-1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val))));
    my_lcd.Set_Draw_color(84, 209, 91);
    my_lcd.Fill_Rectangle(x1+1,(y2-1)-(float(y2-y1-2)*(float(cps_now)/float(max_val))), x2-1,y2-1);
    
  }

  else if(cps_now<threshold2)
  {
    my_lcd.Set_Draw_color(59, 33, 82);
    my_lcd.Fill_Rectangle(x1+1, y1+1, x2-1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val))));
    my_lcd.Set_Draw_color(222, 115, 33);
    my_lcd.Fill_Rectangle(x1+1,(y2-1)-(float(y2-y1-2)*(float(cps_now)/float(max_val))), x2-1,y2-1);
  }

  else if(cps_now<=max_val)
  {
    my_lcd.Set_Draw_color(59, 33, 82);
    my_lcd.Fill_Rectangle(x1+1, y1+1, x2-1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val))));
    my_lcd.Set_Draw_color(204, 30, 24);
    my_lcd.Fill_Rectangle(x1+1,(y2-1)-(float(y2-y1-2)*(float(cps_now)/float(max_val))), x2-1,y2-1);
  }

    my_lcd.Set_Text_colour(255, 255, 255);
    my_lcd.Set_Text_Size(3);
    my_lcd.Set_Text_Mode(0);
    my_lcd.Set_Text_Back_colour(0,0,0);
    my_lcd.Print_Number_Int(long(cps_now), x1+9, y1-30, 3,' ', 10);

 }
  
}


// CURVE FUNCTION
void Curve_Create_Update(uint16_t x1,uint16_t y1,uint8_t max_val, uint8_t magnitude) //x1,y1--> Center, magnitude --> radius of curvature
{
  uint16_t angle;
  int32_t x_val=0,y_val=0;
  uint8_t magnitude_change;
  uint8_t width = 7; // Width of Curvature
  
  if(flag_curve == false) // Only runs once to initialize
  {
  //For making the Centre WHITE Point of radius 4  
  my_lcd.Set_Draw_color(255, 255, 255);
  for(angle =180; angle>0; angle--)
  {
  for(magnitude_change=4;magnitude_change>0;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val); // w.r.t the Screen the coordinates are shifted i.e why x1+x_val, y1-y_val
  }
  
  }

  my_lcd.Set_Draw_color(84, 209, 91);
  for(angle =180; angle>=180-60; angle--)
  {
  for(magnitude_change=magnitude;magnitude_change>magnitude-width;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val);
  }
  
  }

  my_lcd.Set_Draw_color(222, 115, 33);
  for(angle =120; angle>=180-120; angle--)
  {
  for(magnitude_change=magnitude;magnitude_change>magnitude-width;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val);
  }
  
  }

  my_lcd.Set_Draw_color(204, 30, 24);
  for(angle =60; angle>0; angle--)
  {
  for(magnitude_change=magnitude;magnitude_change>magnitude-width;magnitude_change--)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val);
  }
  
  }
  flag_curve=true;
  }

  else
  {

    uint8_t cps_now = cps;
    uint8_t len_arr = 8;
    uint8_t angle_arr = 30;
    uint8_t threshold1 = max_val/3;
    uint8_t threshold2 = (max_val*2)/3;
    angle_arr = 180/angle_arr;
    if(cps_now != previous_cps_curve)
  {
    my_lcd.Set_Draw_color(0, 0, 0);
    x_val = float(magnitude-13)*cos((float(max_val-previous_cps_curve)/float(max_val))*PI);
    y_val = float(magnitude-13)*sin((float(max_val-previous_cps_curve)/float(max_val))*PI);
    my_lcd.Draw_Line(x1, y1-5, x1+x_val,y1-y_val);
    my_lcd.Draw_Line(x1+x_val,y1-y_val,x1+x_val-len_arr*cos((float(max_val-previous_cps_curve)/float(max_val))*PI+(PI/angle_arr)),y1-y_val+len_arr*sin((float(max_val-previous_cps_curve)/float(max_val))*PI+(PI/angle_arr)));
    my_lcd.Draw_Line(x1+x_val,y1-y_val,x1+x_val-len_arr*cos((float(max_val-previous_cps_curve)/float(max_val))*PI-(PI/angle_arr)),y1-y_val+len_arr*sin((float(max_val-previous_cps_curve)/float(max_val))*PI-(PI/angle_arr)));

    previous_cps_curve = cps_now;
    if(cps_now < threshold1)
    {
    my_lcd.Set_Draw_color(84, 209, 91);
    x_val = float(magnitude-13)*cos((float(max_val-cps_now)/float(max_val))*PI);
    y_val = float(magnitude-13)*sin((float(max_val-cps_now)/float(max_val))*PI);
    my_lcd.Draw_Line(x1, y1-5, x1+x_val,y1-y_val);
    my_lcd.Draw_Line(x1+x_val,y1-y_val,x1+x_val-len_arr*cos((float(max_val-cps_now)/float(max_val))*PI+(PI/angle_arr)),y1-y_val+len_arr*sin((float(max_val-cps_now)/float(max_val))*PI+(PI/angle_arr)));
    my_lcd.Draw_Line(x1+x_val,y1-y_val,x1+x_val-len_arr*cos((float(max_val-cps_now)/float(max_val))*PI-(PI/angle_arr)),y1-y_val+len_arr*sin((float(max_val-cps_now)/float(max_val))*PI-(PI/angle_arr)));
    }
    else if(cps_now < threshold2)
    {
    my_lcd.Set_Draw_color(222, 115, 33);
    x_val = float(magnitude-13)*cos((float(max_val-cps_now)/float(max_val))*PI);
    y_val = float(magnitude-13)*sin((float(max_val-cps_now)/float(max_val))*PI);
    my_lcd.Draw_Line(x1, y1-5, x1+x_val,y1-y_val);
    my_lcd.Draw_Line(x1+x_val,y1-y_val,x1+x_val-len_arr*cos((float(max_val-cps_now)/float(max_val))*PI+(PI/angle_arr)),y1-y_val+len_arr*sin((float(max_val-cps_now)/float(max_val))*PI+(PI/angle_arr)));
    my_lcd.Draw_Line(x1+x_val,y1-y_val,x1+x_val-len_arr*cos((float(max_val-cps_now)/float(max_val))*PI-(PI/angle_arr)),y1-y_val+len_arr*sin((float(max_val-cps_now)/float(max_val))*PI-(PI/angle_arr))); 
    }
    else
    {
    my_lcd.Set_Draw_color(204, 30, 24);
    x_val = float(magnitude-13)*cos((float(max_val-cps_now)/float(max_val))*PI);
    y_val = float(magnitude-13)*sin((float(max_val-cps_now)/float(max_val))*PI);
    my_lcd.Draw_Line(x1, y1-5, x1+x_val,y1-y_val);
    my_lcd.Draw_Line(x1+x_val,y1-y_val,x1+x_val-len_arr*cos((float(max_val-cps_now)/float(max_val))*PI+(PI/angle_arr)),y1-y_val+len_arr*sin((float(max_val-cps_now)/float(max_val))*PI+(PI/angle_arr)));
    my_lcd.Draw_Line(x1+x_val,y1-y_val,x1+x_val-len_arr*cos((float(max_val-cps_now)/float(max_val))*PI-(PI/angle_arr)),y1-y_val+len_arr*sin((float(max_val-cps_now)/float(max_val))*PI-(PI/angle_arr))); 
    }
  }
  }
 
 } 

 void Warning_Text(uint16_t x, uint16_t y,uint8_t max_val)
 {

  uint8_t *text_safe = "SAFE !    ";
  uint8_t *text_mod = "MODERATE !";
  uint8_t *text_dan = "DANGER !  ";
  uint8_t cps_now = cps;
  uint8_t threshold1 = max_val/3;
  uint8_t threshold2 = (max_val*2)/3;
  static uint8_t flag_check=0;
  if(flag_warn == false)
  {
    if(cps_now<threshold1)
  {
  my_lcd.Set_Text_colour(0x0FFF);
  my_lcd.Set_Text_Back_colour(0x0000);
  my_lcd.Set_Text_Size(4);
  my_lcd.Print_String(text_safe,x,y);
  flag_check = 0;
  }
  else if(cps_now<threshold2)
  {
  my_lcd.Set_Text_colour(0x0FFF);
  my_lcd.Set_Text_Back_colour(0x0000);
  my_lcd.Set_Text_Size(4);
  my_lcd.Print_String(text_mod,x,y);
  flag_check = 1;
  }

  else if(cps_now>threshold2)
  {
  my_lcd.Set_Text_colour(0x0FFF);
  my_lcd.Set_Text_Back_colour(0x0000);
  my_lcd.Set_Text_Size(4);
  my_lcd.Print_String(text_dan,x,y);
  flag_check = 2;
  }
  flag_warn=true;
  }
  
  else
  {
  if(cps_now!= previous_cps_warn)
  {
  if(cps_now<threshold1 && (flag_check==1 || flag_check==2))
  {
  my_lcd.Set_Text_colour(0x0FFF);
  my_lcd.Set_Text_Back_colour(0x0000);
  my_lcd.Set_Text_Size(4);
  my_lcd.Print_String(text_safe,x,y);
  flag_check = 0;
  }
  else if(cps_now<threshold2 && cps_now>threshold1 && (flag_check==0 || flag_check==2))
  {
  my_lcd.Set_Text_colour(0x0FFF);
  my_lcd.Set_Text_Back_colour(0x0000);
  my_lcd.Set_Text_Size(4);
  my_lcd.Print_String(text_mod,x,y);
  flag_check = 1;
  }

  else if(cps_now>threshold2 && (flag_check==0 || flag_check==1))
  {
  my_lcd.Set_Text_colour(0x0FFF);
  my_lcd.Set_Text_Back_colour(0x0000);
  my_lcd.Set_Text_Size(4);
  my_lcd.Print_String(text_dan,x,y);
  flag_check = 2;
  }
  }
  }
 }

void Make_Axis(uint16_t x, uint16_t y, uint16_t max_limit_x, uint16_t max_limit_y)
{
  
  my_lcd.Set_Draw_color(0xFFFF);
  
  //X-axis (Start x coordinate, Start y coordinate, Height)
  my_lcd.Draw_Fast_HLine(0, my_lcd.Get_Height()-y, my_lcd.Get_Width());
  
  //Y-axis (Start x coordinate, Start y coordinate, Width)
  my_lcd.Draw_Fast_VLine(x, 0, my_lcd.Get_Height());

  my_lcd.Set_Text_colour(0x0FFF);
  my_lcd.Set_Text_Size(1);
  my_lcd.Set_Text_Mode(1);

  //X-Axis Point
  for(uint8_t count_x=1; count_x<= 10; count_x++)
    {
    for(uint8_t count_y=0;count_y<8;count_y++)
      {
      my_lcd.Draw_Pixel( x+(my_lcd.Get_Width()-x)*count_x*0.1, my_lcd.Get_Height()-y+count_y);
      }  
      my_lcd.Print_Number_Int(long(max_limit_x*count_x*0.1), x+(my_lcd.Get_Width()-x)*count_x*0.1-25, (my_lcd.Get_Height()-y)+15, 3,' ', 10);
    
    }
  
  // Y-Axis Points
  my_lcd.Print_Number_Int(long(0), x-15, (my_lcd.Get_Height()-y)+5, 3,' ', 10); // To Draw 0
  
  for(uint8_t count_y=1; count_y<= 10; count_y++)
  {
  for(uint8_t count_x=0;count_x<8;count_x++)
    {
    my_lcd.Draw_Pixel( x-count_x, (my_lcd.Get_Height()-y)-(my_lcd.Get_Height()-y)*0.1*count_y);
    }  
    my_lcd.Print_Number_Int(long(max_limit_y*count_y*0.1), x-25, (my_lcd.Get_Height()-y)-(my_lcd.Get_Height()-y)*0.1*count_y, 3,' ', 10);
  
  }
  
}

void Draw_Spectrum_Full(uint16_t x, uint16_t y)
{

if(flag_start != 1)
{

for(int count=0; count<BUFFER; count++)
{
  record_data_compressed[int((count*(my_lcd.Get_Width()-x))/BUFFER)] = record_data_compressed[int((count*(my_lcd.Get_Width()-x))/BUFFER)] + record_data[count];
}
my_lcd.Set_Draw_color(0x0FFF);
for(int count=0; count< my_lcd.Get_Width()-x; count++)
{
  my_lcd.Draw_Fast_VLine(x+count+1,my_lcd.Get_Height()-y-record_data_compressed[count],record_data_compressed[count]);
}

flag_start = 1;

}

}

void Update_Spectrum_Full(uint16_t x, uint16_t y, uint16_t adc_value)
{
  adc_value = int((adc_value*(my_lcd.Get_Width()-x))/BUFFER);
  record_data_compressed[adc_value] = record_data_compressed[adc_value] + 1;
  my_lcd.Set_Draw_color(0x0FFF);
  my_lcd.Draw_Fast_VLine(x+adc_value+1,my_lcd.Get_Height()-y-record_data_compressed[adc_value],record_data_compressed[adc_value]); // Blue Line
  my_lcd.Set_Draw_color(0x0000);
  my_lcd.Draw_Fast_VLine(x+adc_value+1,0,my_lcd.Get_Height()-y-record_data_compressed[adc_value]); // Black Line

}

void Record_Data(uint16_t adc_value)
{
  record_data[adc_value] = record_data[adc_value]+1;
  if(flag_start == 1 && Screen2)
  {
    Update_Spectrum_Full(45,35,adc_value);
  }
}

void update_screen(int row, int column, uint16_t color)
{
  my_lcd.Set_Draw_color(color);
   my_lcd.Draw_Pixel(row,column);
}
