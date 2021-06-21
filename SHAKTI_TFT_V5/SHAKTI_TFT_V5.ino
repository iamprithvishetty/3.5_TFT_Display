#include <LCD_SHAKTI.h>
#include <math.h> //Math Library

extern "C"{
#include <stdint.h>//Includes the definitions of standard input/output functions//
#include "pwm_driver.h"
#include "plic_driver.h"
#include "platform.h"
#include "log.h"
#include "stddef.h"
#include "gpio.h"
#include "utils.h"
}
#define INTERRUPT PLIC_INTERRUPT_21
unsigned int retval;
boolean interrupt_enabled = false;

#define MAX_VALUE 50 // Counts Per Second Maximum Value
#define BUFFER 1024 // Array Size for 1024 Channels

#define PI 3.141592653589793238 // Value of PI

#define top_y 50

volatile uint8_t cps=10; //counts per second static volatile variable
uint8_t previous_cps_bar=0; // Track Bar previous cps value saved for checking if there are changes in new value and previous value
uint8_t previous_cps_curve=0; // Curve Pointer previous cps value saved for checking if there are changes in new value and previous value
uint8_t previous_cps_warn=0; // Warning Text previous cps value saved for checking if there are changes in new value and previous value
bool flag_init=false; // To initialize the Track Bar to make it run only once
bool flag_curve=false; // To initialize the Curve and Pointer to make it run only once
bool flag_warn=false; // To initialize the Warning Text to make it run only once
int time_now = millis();

boolean Screen1 = false; // Screen1 Flag to make Screen1 Run
boolean Screen2 = false; // Screen2 Flag to make Screen2 Run
boolean Screen3 = true;

uint16_t x_origin; // Origin x-coordinate
uint16_t y_origin; // Origin y-coordinate
uint16_t max_limit_x; // Max x-limit to assign nos to x-axis
uint16_t max_limit_y; // Max y-limit to assign nos to y-axis

volatile uint16_t Scale = 1; // Scale to keep a track of how much to adjust the Screen Spectrum Display
volatile uint16_t Scale_exp = 1;
volatile uint32_t record_data[BUFFER]={0}; // Record of all the data stored
volatile uint32_t record_data_compressed[480]={0}; // Compressed data of the Entire Spectrum fit in the entire Display
boolean flag_start=0; // Start flag to Draw Spectrum and Make the axis only once

boolean flag_start_max=0; // Start flag to Draw Spectrum Small and Make the axis only once
volatile uint16_t lower_max_x=0;
volatile uint16_t higher_max_x=0;
volatile uint16_t Scale_max = 1; // Scale to keep a track of how much to adjust the Screen Spectrum Display
volatile uint16_t Scale_exp_max = 1;
volatile uint16_t max_channel = 0;

int time_now_max = 0;
 
uint16_t legend_x;
uint16_t legend_y;
uint16_t legend_x_max;
uint16_t legend_y_max;

LCD_SHAKTI my_lcd(0x9486,480,320); // Constructor to give display ID, width and height

void handle_button_press(__attribute__((unused)) uint32_t num)
{
  while(digitalRead(1<<(INTERRUPT-7)) == 1)
  {
    continue;
  }
  interrupt_enabled= true;
}

void setup() {
  plic_init();
//configure interrupt id 7
configure_interrupt(INTERRUPT);
//set the corresponding isr for interrupt id 7
isr_table[INTERRUPT] = handle_button_press;
// Enable Global (PLIC) interrupts.
asm volatile("li      t0, 8\t\n"
       "csrrs   zero, mstatus, t0\t\n"
      );
// Enable Local (PLIC) interrupts.
asm volatile("li      t0, 0x800\t\n"
       "csrrs   zero, mie, t0\t\n"
      );
asm volatile(
       "csrr %[retval], mstatus\n"
       :
       [retval]
       "=r"
       (retval)
      );
log_debug("mstatus = %x\n", retval);
asm volatile(
       "csrr %[retval], mie\n"
       :
       [retval]
       "=r"
       (retval)
      );
log_debug("mie = %x\n", retval);
asm volatile(
       "csrr %[retval], mip\n"
       :
       [retval]
       "=r"
       (retval)
      );
log_debug("mip = %u\n", retval);
  
  my_lcd.Init_LCD(); // To Initialize the TFT Display and pass the initial commands
  my_lcd.Set_Rotation(1); 
  my_lcd.Fill_Screen(0, 0, 0);
  Set_Origin(45,30); // To Set the Orgin as Static Variables
  Set_Max_Limit(BUFFER,100); // To Set Max x-axis limit and y-axis limit
  for(int count=0;count<20000;count++) // Testing purpose record data
  {
    Record_Data(random(0,1024));
  }
//  for(int count=0; count<50; count++)
//  {
//    Record_Data(610);
//  }
 
  
}


void loop() {
 if(interrupt_enabled== true)
 {
  if(Screen1 == true)
  {
    Screen1 = false;
    Screen2 = true;
    Screen3 =false;
  }
  else if(Screen2 == true)
  {
    Screen1 = false;
    Screen2 = false;
    Screen3 =true;
  }
  else if(Screen3 == true)
  {
    Screen1 = true;
    Screen2 = false;
    Screen3 =false;
  }
  flag_start=0;
  flag_start_max=0;
  flag_init=0;
  flag_curve=0;
  flag_warn=0;
  my_lcd.Fill_Screen(0, 0, 0);
  interrupt_enabled = false;
 }
 
 if(Screen1) // If Screen1 is TRUE
 {
  if(millis()-time_now>50)
  {
    time_now = millis();
    if(cps>=MAX_VALUE)
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
  if(flag_start == 0)
  {
     Draw_Spectrum_Full();
  }
  Record_Data(random(600,610));
 }
 
 if(Screen3)
 {
  if(flag_start_max == 0)
  {
    Draw_Spectrum_Max();
  }
  Record_Data(random(520,680));
 }
 
}

void Bar_Create_Update(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint8_t max_val) //To create a TRACK BAR and Update it Coordinates of Rectangle (x1,y1),(x2,y2) with MAXIMUM VALUE
{
  if(flag_init == false) // Initializing so that this runs only once
  {
  my_lcd.Set_Draw_color(198, 144, 245); // Border Colour
  my_lcd.Draw_Rectangle(x1, y1, x2,y2); // Coordinates for border
  my_lcd.Set_Draw_color(59, 33, 82); // Fill in Colour 
  my_lcd.Fill_Rectangle(x1+1, y1+1, x2-1,y2-1); // Coordinates for the Fill in Part

  uint8_t count_x;
  uint8_t count_y;
  
  for(count_y=0;count_y<11;count_y++) // To divide the entire bar into 10 parts for labelling
  {   
    for(count_x=0;count_x<8;count_x++) // To make small line in order to show the partition
    {
    my_lcd.Set_Draw_color(255, 255, 255); // Color of that line
    my_lcd.Draw_Pixel( x2+count_x, y2-((y2-y1)*0.1*count_y));
    }

    my_lcd.Set_Text_colour(255, 255, 255); // Color of text in this case the integers
    my_lcd.Set_Text_Size(1);
    my_lcd.Set_Text_Mode(1);
    my_lcd.Print_Number_Int(long(max_val*count_y*0.1), x2+count_x+2, y2-((y2-y1)*0.1*count_y)-4, 3,' ', 10); // Printing Numbers at position( x2+count_x+ 2<-- right shift , y2-((y2-y1)*0.1*count_y)- 4<-- up shift)
    
  }
  flag_init = true; // To make this run only once
  }

  uint8_t threshold1 = max_val/3; //Setting Lower Threshold 
  uint8_t threshold2 = (max_val*2)/3; // Setting Upper Threshold

  uint8_t cps_now = cps; //Current Counts Per Second Value
 if(cps_now != previous_cps_bar) // Comparing with previously recorded value
 {
  previous_cps_bar = cps_now; // Equating previously recorded to current cps
  
  if(cps_now<threshold1) // Checking if current cps is less than threshold1
  {
    my_lcd.Set_Draw_color(59, 33, 82); // Dark Purple Color
    my_lcd.Fill_Rectangle(x1+1, y1+1, x2-1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val)))); // Colouring the rest apart from Green to Dark Purple
    my_lcd.Set_Draw_color(84, 209, 91); // Green Color for safe indiacation of cps
    my_lcd.Fill_Rectangle(x1+1,(y2-1)-(float(y2-y1-2)*(float(cps_now)/float(max_val))), x2-1,y2-1); // Colouring the cps level Green
    
  }

  else if(cps_now<threshold2) // Checking if current cps is less than threshold2
  {
    my_lcd.Set_Draw_color(59, 33, 82); // Dark Purple Color
    my_lcd.Fill_Rectangle(x1+1, y1+1, x2-1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val)))); // Colouring the rest apart from Orange to Dark Purple
    my_lcd.Set_Draw_color(222, 115, 33); // Orange Color for moderate indiacation of cps
    my_lcd.Fill_Rectangle(x1+1,(y2-1)-(float(y2-y1-2)*(float(cps_now)/float(max_val))), x2-1,y2-1); // Colouring the cps level Orange
  }

  else if(cps_now<=max_val)
  {
    my_lcd.Set_Draw_color(59, 33, 82); // Dark Purple Color
    my_lcd.Fill_Rectangle(x1+1, y1+1, x2-1,(y2-1)-(float(y2-y1-2)*(float(cps)/float(max_val)))); // Colouring the rest apart from Red to Dark Purple
    my_lcd.Set_Draw_color(204, 30, 24); // Red Color for danger indiacation of cps
    my_lcd.Fill_Rectangle(x1+1,(y2-1)-(float(y2-y1-2)*(float(cps_now)/float(max_val))), x2-1,y2-1); // Colouring the cps level Red
  }

    my_lcd.Set_Text_colour(255, 255, 255); // Setting White Text Colour
    my_lcd.Set_Text_Size(3); 
    my_lcd.Set_Text_Mode(0); // BAckground Colour Visible
    my_lcd.Set_Text_Back_colour(0,0,0); // Background Colour
    my_lcd.Print_Number_Int(long(cps_now), x1+9, y1-30, 3,' ', 10); // Printing the Current cps level at the top of the Bar

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
  for(angle =180; angle>0; angle--) // To draw a semi circle with 0-180degree
  {
  for(magnitude_change=4;magnitude_change>0;magnitude_change--) //lower radius-->0 higher radius-->4 Here magnitude change starting from 4 indicates the width i.e 4 pixels
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180); //r(cosA+i*sinA) Complex No
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val); // w.r.t the Screen the coordinates are shifted i.e why x1+x_val, y1-y_val
  }
  
  }

  my_lcd.Set_Draw_color(84, 209, 91); // Green Colour
  for(angle =180; angle>=180-60; angle--) // To draw a Arc from 180-120degree
  {
  for(magnitude_change=magnitude;magnitude_change>magnitude-width;magnitude_change--) //lower radius-->(magnitude-width) higher radius-->(magnitude)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val); // w.r.t the Screen the coordinates are shifted i.e why x1+x_val, y1-y_val
  }
  
  }

  my_lcd.Set_Draw_color(222, 115, 33); // Orange Colour
  for(angle =120; angle>=180-120; angle--) // To draw a Arc from 120-60degree
  {
  for(magnitude_change=magnitude;magnitude_change>magnitude-width;magnitude_change--) //lower radius-->(magnitude-width) higher radius-->(magnitude)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val); // w.r.t the Screen the coordinates are shifted i.e why x1+x_val, y1-y_val
  }
  
  }

  my_lcd.Set_Draw_color(204, 30, 24); // Red Colour
  for(angle =60; angle>0; angle--) // To draw a Arc from 60-0degree
  {
  for(magnitude_change=magnitude;magnitude_change>magnitude-width;magnitude_change--) //lower radius-->(magnitude-width) higher radius-->(magnitude)
  {
    x_val = float(magnitude_change)*cos((angle*PI)/180);
    y_val = float(magnitude_change)*sin((angle*PI)/180);
    my_lcd.Draw_Pixel(x1+x_val,y1-y_val); // w.r.t the Screen the coordinates are shifted i.e why x1+x_val, y1-y_val
  }
  
  }
  flag_curve=true; // To make initialization part only run once
  }

  else // To draw the Pointer
  {

    uint8_t cps_now = cps; // Current cps value stored in cps_now because cps is dynamically changing
    uint8_t len_arr = 8; // Length of Arrow Pointer we want to make
    uint8_t angle_arr = 30; // Angle of the Arrow Pointer For eg  "--->" Something Like this
    
    uint8_t threshold1 = max_val/3; // Lower Threshold
    uint8_t threshold2 = (max_val*2)/3; // Upper Threshold
    
    angle_arr = 180/angle_arr; // Factor for dividing with Constat value PI because cos and sin function takes angle in radians
    
    if(cps_now != previous_cps_curve) // To check if the new value has changed from the previously recorded value
  {
    my_lcd.Set_Draw_color(0, 0, 0); // Black Colour to remove the previously created Arrow
    x_val = float(magnitude-13)*cos((float(max_val-previous_cps_curve)/float(max_val))*PI); // Calculating the end x-point of the pointer; 13 here is the distance it should be apart from the outer curvvature
    y_val = float(magnitude-13)*sin((float(max_val-previous_cps_curve)/float(max_val))*PI); // Calculating the end y-point of the pointer; 13 here is the distance it should be apart from the outer curvvature
    my_lcd.Draw_Line(x1, y1-5, x1+x_val,y1-y_val); // This is to draw the straight line from the Center to the End Point
    my_lcd.Draw_Line(x1+x_val,y1-y_val,x1+x_val-len_arr*cos((float(max_val-previous_cps_curve)/float(max_val))*PI+(PI/angle_arr)),y1-y_val+len_arr*sin((float(max_val-previous_cps_curve)/float(max_val))*PI+(PI/angle_arr))); // This is to make the Arrow line using the concept of rotating the line by cos(theta1+theta2) and sin(theta1+theta2) 
    my_lcd.Draw_Line(x1+x_val,y1-y_val,x1+x_val-len_arr*cos((float(max_val-previous_cps_curve)/float(max_val))*PI-(PI/angle_arr)),y1-y_val+len_arr*sin((float(max_val-previous_cps_curve)/float(max_val))*PI-(PI/angle_arr))); // This is to make the Arrow line using the concept of rotating the line by cos(theta1+theta2) and sin(theta1+theta2) 

    previous_cps_curve = cps_now; // Setting previous value to the new value of cps
    if(cps_now < threshold1) // Checking if cps value is less than threshold1
    {
    my_lcd.Set_Draw_color(84, 209, 91); // Setting Arrow colour to Green
    x_val = float(magnitude-13)*cos((float(max_val-cps_now)/float(max_val))*PI); // Same Logic as previously mentioned
    y_val = float(magnitude-13)*sin((float(max_val-cps_now)/float(max_val))*PI);
    my_lcd.Draw_Line(x1, y1-5, x1+x_val,y1-y_val);
    my_lcd.Draw_Line(x1+x_val,y1-y_val,x1+x_val-len_arr*cos((float(max_val-cps_now)/float(max_val))*PI+(PI/angle_arr)),y1-y_val+len_arr*sin((float(max_val-cps_now)/float(max_val))*PI+(PI/angle_arr)));
    my_lcd.Draw_Line(x1+x_val,y1-y_val,x1+x_val-len_arr*cos((float(max_val-cps_now)/float(max_val))*PI-(PI/angle_arr)),y1-y_val+len_arr*sin((float(max_val-cps_now)/float(max_val))*PI-(PI/angle_arr)));
    }
    else if(cps_now < threshold2) // Checking if cps value is less than threshold2 and greater than threshold1
    {
    my_lcd.Set_Draw_color(222, 115, 33); // Setting Arrow colour to Orange
    x_val = float(magnitude-13)*cos((float(max_val-cps_now)/float(max_val))*PI); // Same Logic as previously mentioned
    y_val = float(magnitude-13)*sin((float(max_val-cps_now)/float(max_val))*PI);
    my_lcd.Draw_Line(x1, y1-5, x1+x_val,y1-y_val);
    my_lcd.Draw_Line(x1+x_val,y1-y_val,x1+x_val-len_arr*cos((float(max_val-cps_now)/float(max_val))*PI+(PI/angle_arr)),y1-y_val+len_arr*sin((float(max_val-cps_now)/float(max_val))*PI+(PI/angle_arr)));
    my_lcd.Draw_Line(x1+x_val,y1-y_val,x1+x_val-len_arr*cos((float(max_val-cps_now)/float(max_val))*PI-(PI/angle_arr)),y1-y_val+len_arr*sin((float(max_val-cps_now)/float(max_val))*PI-(PI/angle_arr))); 
    }
    else // Checking if cps value is greater than threshold2
    {
    my_lcd.Set_Draw_color(204, 30, 24); // Setting Arrow colour to Red
    x_val = float(magnitude-13)*cos((float(max_val-cps_now)/float(max_val))*PI); // Same Logic as previously mentioned
    y_val = float(magnitude-13)*sin((float(max_val-cps_now)/float(max_val))*PI);
    my_lcd.Draw_Line(x1, y1-5, x1+x_val,y1-y_val);
    my_lcd.Draw_Line(x1+x_val,y1-y_val,x1+x_val-len_arr*cos((float(max_val-cps_now)/float(max_val))*PI+(PI/angle_arr)),y1-y_val+len_arr*sin((float(max_val-cps_now)/float(max_val))*PI+(PI/angle_arr)));
    my_lcd.Draw_Line(x1+x_val,y1-y_val,x1+x_val-len_arr*cos((float(max_val-cps_now)/float(max_val))*PI-(PI/angle_arr)),y1-y_val+len_arr*sin((float(max_val-cps_now)/float(max_val))*PI-(PI/angle_arr))); 
    }
  }
  }
 
 } 

 void Warning_Text(uint16_t x, uint16_t y,uint8_t max_val) // To Display the Warning text at Position (x,y) with Size 4
 {

  uint8_t *text_safe = "SAFE !    "; // Safe Text
  uint8_t *text_mod = "MODERATE !"; // Moderate Text
  uint8_t *text_dan = "DANGER !  "; // Danger Text
  uint8_t cps_now = cps; // Setting current cps_value
  uint8_t threshold1 = max_val/3; // Setting threshold1
  uint8_t threshold2 = (max_val*2)/3; // Setting threshold2
  static uint8_t flag_check=0; // To keep a check on what to show depending on the cps value flag_check = 0 indicates Safe ,flag check = 1 indicates Moderate and flag_
  if(flag_warn == false) // To make this part run only once to initialize during the start
  {
    if(cps_now<threshold1) // Checking if cps value is less than threshold1
  {
  my_lcd.Set_Text_colour(0x0FFF); // Color set to Light Blue
  my_lcd.Set_Text_Back_colour(0x0000); // Black Color set to background
  my_lcd.Set_Text_Size(4); // Text size set to 4
  my_lcd.Print_String(text_safe,x,y); // Print String "SAFE !"
  flag_check = 0; // Flag Check Set to 0; this is to not repeat same process again
  }
  else if(cps_now<threshold2) // Checking if cps value is less than threshold2
  {
  my_lcd.Set_Text_colour(0x0FFF);
  my_lcd.Set_Text_Back_colour(0x0000);
  my_lcd.Set_Text_Size(4); 
  my_lcd.Print_String(text_mod,x,y); // Print String "MODERATE !"
  flag_check = 1; // Flag Check Set to 1; this is to not repeat same process again
  }

  else if(cps_now>threshold2) // Checking if cps value is greater than threshold2
  {
  my_lcd.Set_Text_colour(0x0FFF);
  my_lcd.Set_Text_Back_colour(0x0000);
  my_lcd.Set_Text_Size(4);
  my_lcd.Print_String(text_dan,x,y); // Print String "DANGER !"
  flag_check = 2; // Flag Check Set to 2; this is to not repeat same process again
  }
  flag_warn=true; // Making flag_warn true because it has done initializing
  }
  
  else // Next time when it has to change the Text it will come here
  {
  if(cps_now!= previous_cps_warn) // To check if the new value has changed from the previously recorded value
  {
  if(cps_now<threshold1 && (flag_check==1 || flag_check==2)) // Checking if current cps is less than threshold1 and its previous text was different
  {
  my_lcd.Set_Text_colour(0x0FFF);  // Same as previous logic as mentioned above
  my_lcd.Set_Text_Back_colour(0x0000);
  my_lcd.Set_Text_Size(4);
  my_lcd.Print_String(text_safe,x,y);
  flag_check = 0;
  }
  else if(cps_now<threshold2 && cps_now>threshold1 && (flag_check==0 || flag_check==2)) // Checking if current cps is less than threshold2 and greater than thrshold1 and its previous text was different
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

void Set_Origin(uint16_t x,  uint16_t y) // Setting Origin so that we don't have to pass it every time in Function we make; This needs to be initialized first
{
  x_origin = x; // x-origin is the static variable set before void setup()
  y_origin = y; // y-origin is the static variable set before void setup()
  
}

void Set_Max_Limit(uint16_t x_lim, uint16_t y_lim) // Setting Maximum value for x-axis and y-axis value respectively
{
  max_limit_x = x_lim;
  max_limit_y = y_lim;
}

void Make_Axis() // To Make X-axis and Y-sxis with ma
{
  
  my_lcd.Set_Draw_color(0xFFFF);
  
  //X-axis (Start x coordinate, Start y coordinate, Height)
  my_lcd.Draw_Fast_HLine(0, my_lcd.Get_Height()-y_origin, my_lcd.Get_Width());
  
  //Y-axis (Start x coordinate, Start y coordinate, Width)
  my_lcd.Draw_Fast_VLine(x_origin, 0, my_lcd.Get_Height());

  my_lcd.Set_Text_colour(0x0FFF); // Text Colour Set to Light Blue
  my_lcd.Set_Text_Back_colour(0,0,0); // Text Background colour Black
  my_lcd.Set_Text_Size(1); // Text Size to 1
  my_lcd.Set_Text_Mode(0);
  
  //X-Axis Point
  for(uint8_t count_x=1; count_x<= 10; count_x++) // Displaying 10 values ranging from 1 to max_limit_x both inclusive on X-axis
    {
      
    for(uint8_t count_y=0;count_y<8;count_y++) // This is to make the white vertical partitions for Numbering whose length is 8px
      {
      my_lcd.Draw_Pixel( x_origin+(my_lcd.Get_Width()-x_origin)*count_x*0.1, my_lcd.Get_Height()-y_origin+count_y);
      }  
      
      my_lcd.Print_Number_Int(long(max_limit_x*count_x*0.1), x_origin+(my_lcd.Get_Width()-x_origin)*count_x*0.1-25, (my_lcd.Get_Height()-y_origin)+15, 3,' ', 10);
    
    }
  
  // Y-Axis Points
  my_lcd.Print_Number_Int(long(0), x_origin-15, (my_lcd.Get_Height()-y_origin)+5, 3,' ', 10); // To Draw 0 of the origin
  
  for(uint8_t count_y=1; count_y<= 10; count_y++) // Displaying 10 values ranging from 1 to max_limit_y both inclusive on Y-axis
  {
    
  for(uint8_t count_x=0;count_x<8;count_x++) // This is to make the white horizontal partitions for Numbering whose length is 8px
    {
    my_lcd.Draw_Pixel( x_origin-count_x, (my_lcd.Get_Height()-y_origin)-(my_lcd.Get_Height()-y_origin-top_y)*0.1*count_y);
    } 
     
    my_lcd.Print_Number_Int(long(Scale_exp*max_limit_y*count_y*0.1), x_origin-35, (my_lcd.Get_Height()-y_origin)-(my_lcd.Get_Height()-top_y-y_origin)*0.1*count_y, 3,' ', 10);
  
  }
  
}

void Update_Y_Axis() // Updating the Y-Axis Numbering
{
   // Y-Axis Points
  my_lcd.Set_Text_colour(0x0FFF);// Light Blue Colour
  my_lcd.Set_Text_Back_colour(0,0,0); // Setting text background colour to Black
  my_lcd.Set_Text_Size(1); // Text Size
  my_lcd.Set_Text_Mode(0);
  
  for(uint8_t count_y=1; count_y<= 10; count_y++)  // Updating only the Y-axis numbering to new scaled value depending on Scale=?
  {
    my_lcd.Print_Number_Int(long(Scale_exp*max_limit_y*count_y*0.1), x_origin-35, (my_lcd.Get_Height()-y_origin)-(my_lcd.Get_Height()-top_y-y_origin)*0.1*count_y, 3,' ', 10);
  }
}

void Update_Y_Axis_Max() // Updating the Y-Axis Numbering
{
   // Y-Axis Points
  my_lcd.Set_Text_colour(0x0FFF);// Light Blue Colour
  my_lcd.Set_Text_Back_colour(0,0,0); // Setting text background colour to Black
  my_lcd.Set_Text_Size(1); // Text Size
  my_lcd.Set_Text_Mode(0);
  
  for(uint8_t count_y=1; count_y<= 10; count_y++)  // Updating only the Y-axis numbering to new scaled value depending on Scale=?
  {
    my_lcd.Print_Number_Int(long(Scale_exp_max*max_limit_y*count_y*0.1), x_origin-35, (my_lcd.Get_Height()-y_origin)-(my_lcd.Get_Height()-top_y-y_origin)*0.1*count_y, 3,' ', 10);
  }
}

void Draw_Spectrum_Full()  // Draw Full Spectrum starting from Origin
{

if(flag_start != 1) // flag_start set to 0 to run this only during initialization
{
  
for(int count=0; count<my_lcd.Get_Width() - x_origin; count++) // Setting all the values record data compressed array to 0
{
  record_data_compressed[count] = 0;
}

for(int count=0; count<BUFFER; count++) // Changing the record data compressed array depending on values of record data array which has the entire datatset
{
  record_data_compressed[int((count*(my_lcd.Get_Width()-x_origin))/BUFFER)] = record_data_compressed[int((count*(my_lcd.Get_Width()-x_origin))/BUFFER)] + record_data[count];
}

Make_Axis();

my_lcd.Set_Draw_color(0x0FFF);

for(int count=0; count< my_lcd.Get_Width()-x_origin; count++) // To draw the spectrum lines starting from origin to the end of the screen width
{
  my_lcd.Set_Draw_color(0x0FFF); // Light Blue Colour
  my_lcd.Draw_Fast_VLine(x_origin+count+1,my_lcd.Get_Height()-y_origin-((record_data_compressed[count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp)),((record_data_compressed[count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp))); // Drawing the Light Blue Line
  my_lcd.Set_Draw_color(0x0000); // Black Colour
  my_lcd.Draw_Fast_VLine(x_origin+count+1,top_y,my_lcd.Get_Height()-y_origin-((record_data_compressed[count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp))-top_y); // Drawing the reamining Black Line i.e not light blue
}

Legends(210 ,5);
Make_Max_Value();
flag_start = 1; // flag_start set to 1 to not make this run only once

}
else
{
  for(int count=0; count< my_lcd.Get_Width()-x_origin; count++) // To update the spectrum lines starting from origin to the end of the screen width whenever the scale changes
{
  my_lcd.Set_Draw_color(0x0FFF); // Light Blue Colour
  my_lcd.Draw_Fast_VLine(x_origin+count+1,my_lcd.Get_Height()-y_origin-((record_data_compressed[count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp)),((record_data_compressed[count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp)));  // Drawing the Light Blue Line
  my_lcd.Set_Draw_color(0x0000); // Black Colour
  my_lcd.Draw_Fast_VLine(x_origin+count+1,top_y,my_lcd.Get_Height()-y_origin-((record_data_compressed[count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp))-top_y); // Drawing the reamining Black Line i.e not light blue
}
}
}

void Make_Axis_Max() // To Make X-axis and Y-sxis with ma
{
  
  my_lcd.Set_Draw_color(0xFFFF);
  
  //X-axis (Start x coordinate, Start y coordinate, Height)
  my_lcd.Draw_Fast_HLine(0, my_lcd.Get_Height()-y_origin, my_lcd.Get_Width());
  
  //Y-axis (Start x coordinate, Start y coordinate, Width)
  my_lcd.Draw_Fast_VLine(x_origin, 0, my_lcd.Get_Height());

  my_lcd.Set_Text_colour(0x0FFF); // Text Colour Set to Light Blue
  my_lcd.Set_Text_Back_colour(0,0,0); // Text Background colour Black
  my_lcd.Set_Text_Size(1); // Text Size to 1
  my_lcd.Set_Text_Mode(0);
  
  //X-Axis Point
  for(uint8_t count_x=0; count_x<= 10; count_x++) // Displaying 10 values ranging from 1 to max_limit_x both inclusive on X-axis
    {
      
    for(uint8_t count_y=0;count_y<8;count_y++) // This is to make the white vertical partitions for Numbering whose length is 8px
      {
      my_lcd.Draw_Pixel( x_origin+(my_lcd.Get_Width()-x_origin)*count_x*0.1, my_lcd.Get_Height()-y_origin+count_y);
      }  
      
      my_lcd.Print_Number_Int(long(lower_max_x+(higher_max_x-lower_max_x)*count_x*0.1), x_origin+(my_lcd.Get_Width()-x_origin)*count_x*0.1-25, (my_lcd.Get_Height()-y_origin)+15, 5,' ', 10);
    
    }
  
  // Y-Axis Points
  
  for(uint8_t count_y=1; count_y<= 10; count_y++) // Displaying 10 values ranging from 1 to max_limit_y both inclusive on Y-axis
  {
    
  for(uint8_t count_x=0;count_x<8;count_x++) // This is to make the white horizontal partitions for Numbering whose length is 8px
    {
    my_lcd.Draw_Pixel( x_origin-count_x, (my_lcd.Get_Height()-y_origin)-(my_lcd.Get_Height()-y_origin-top_y)*0.1*count_y);
    } 
     
    my_lcd.Print_Number_Int(long(Scale_exp_max*max_limit_y*count_y*0.1), x_origin-35, (my_lcd.Get_Height()-y_origin)-(my_lcd.Get_Height()-top_y-y_origin)*0.1*count_y, 3,' ', 10);
  
  }
  
}

void Draw_Spectrum_Max()
{

  if(flag_start_max == 0)
  {
     max_channel = Max_Value(record_data,BUFFER);
     Legends_Max(210 ,5);
     if ( max_channel >= BUFFER - (my_lcd.Get_Width() - x_origin) + (my_lcd.Get_Width()-x_origin)/3)
     {
       
        lower_max_x = BUFFER - (my_lcd.Get_Width() - x_origin);
        higher_max_x = BUFFER;
        Make_Axis_Max(); 
        for(int count = 0;count< my_lcd.Get_Width()-x_origin;count++)
        {
          my_lcd.Set_Draw_color(0x0FFF); // Light Blue Colour
          my_lcd.Draw_Fast_VLine(x_origin+count+1,my_lcd.Get_Height()-y_origin-((record_data[lower_max_x+count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp_max)),((record_data[lower_max_x+count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp_max))); // Drawing the Light Blue Line
          my_lcd.Set_Draw_color(0x0000); // Black Colour
          my_lcd.Draw_Fast_VLine(x_origin+count+1,top_y,my_lcd.Get_Height()-y_origin-((record_data[lower_max_x+count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp_max))-top_y); // Drawing the reamining Black Line i.e not light blue
        }
     }

     else if ( max_channel <= my_lcd.Get_Width() - x_origin - (my_lcd.Get_Width()-x_origin)/3)
     {

        lower_max_x = BUFFER - (my_lcd.Get_Width() - x_origin);
        higher_max_x = BUFFER;
        Make_Axis_Max(); 
        for(int count = 0;count< my_lcd.Get_Width()-x_origin;count++)
        {
          my_lcd.Set_Draw_color(0x0FFF); // Light Blue Colour
          my_lcd.Draw_Fast_VLine(x_origin+count+1,my_lcd.Get_Height()-y_origin-((record_data[lower_max_x+count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp_max)),((record_data[lower_max_x+count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp_max))); // Drawing the Light Blue Line
          my_lcd.Set_Draw_color(0x0000); // Black Colour
          my_lcd.Draw_Fast_VLine(x_origin+count+1,top_y,my_lcd.Get_Height()-y_origin-((record_data[lower_max_x+count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp_max))-top_y); // Drawing the reamining Black Line i.e not light blue
        }
     }
     else
     {
        lower_max_x = max_channel - (my_lcd.Get_Width() - x_origin)/2;
        higher_max_x = max_channel + (my_lcd.Get_Width() - x_origin)/2 + 1;
        Make_Axis_Max(); 
        for(int count = 0;count< my_lcd.Get_Width()-x_origin;count++)
        {
          my_lcd.Set_Draw_color(0x0FFF); // Light Blue Colour
          my_lcd.Draw_Fast_VLine(x_origin+count+1,my_lcd.Get_Height()-y_origin-((record_data[lower_max_x+count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp_max)),((record_data[lower_max_x+count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp_max))); // Drawing the Light Blue Line
          my_lcd.Set_Draw_color(0x0000); // Black Colour
          my_lcd.Draw_Fast_VLine(x_origin+count+1,top_y,my_lcd.Get_Height()-y_origin-((record_data[lower_max_x+count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp_max))-top_y); // Drawing the reamining Black Line i.e not light blue
        }
     }
     Make_Max_Value();
     flag_start_max = 1;
  }

  else
  {
        for(int count = 0;count< my_lcd.Get_Width()-x_origin;count++)
        {
          my_lcd.Set_Draw_color(0x0FFF); // Light Blue Colour
          my_lcd.Draw_Fast_VLine(x_origin+count+1,my_lcd.Get_Height()-y_origin-((record_data[lower_max_x+count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp_max)),((record_data[lower_max_x+count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp_max))); // Drawing the Light Blue Line
          my_lcd.Set_Draw_color(0x0000); // Black Colour
          my_lcd.Draw_Fast_VLine(x_origin+count+1,top_y,my_lcd.Get_Height()-y_origin-((record_data[lower_max_x+count]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp_max))-top_y); // Drawing the reamining Black Line i.e not light blue
        }

  }
}



void Update_Spectrum_Full(uint16_t adc_value) // Updating the spectrum 
{
  uint16_t record_scale_change = ((record_data_compressed[adc_value]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp)); // Making changes in the record value to fit it in the specific given height
  my_lcd.Set_Draw_color(0x0FFF); 
  my_lcd.Draw_Fast_VLine(x_origin+adc_value+1,my_lcd.Get_Height()-y_origin-record_scale_change,record_scale_change); // Blue Line
  my_lcd.Set_Draw_color(0x0000);
  my_lcd.Draw_Fast_VLine(x_origin+adc_value+1,top_y,my_lcd.Get_Height()-y_origin-record_scale_change-top_y); // Black Line

}

void Update_Spectrum_Max(uint16_t adc_value) // Updating the spectrum 
{
  if(adc_value >= lower_max_x && adc_value <= higher_max_x)
  {    
  uint16_t record_scale_change = ((record_data[adc_value]*(my_lcd.Get_Height()-top_y-y_origin))/(max_limit_y*Scale_exp_max)); // Making changes in the record value to fit it in the specific given height
  my_lcd.Set_Draw_color(0x0FFF); 
  my_lcd.Draw_Fast_VLine(x_origin+adc_value-lower_max_x+1,my_lcd.Get_Height()-y_origin-record_scale_change,record_scale_change); // Blue Line
  my_lcd.Set_Draw_color(0x0000);
  my_lcd.Draw_Fast_VLine(x_origin+adc_value-lower_max_x+1,top_y,my_lcd.Get_Height()-y_origin-record_scale_change-top_y); // Black Line
  
  }
}

void Legends(uint16_t x, uint16_t y)
{

  uint8_t *text_x_axis = "X-Axis :1 ut=    ch";
  uint8_t *text_y_axis = "Y-Axis :1 ut=    cps";

  legend_x = x;
  legend_y = y;
 
  my_lcd.Set_Text_colour(255, 255, 255);
  my_lcd.Set_Text_Back_colour(0x0000);
  my_lcd.Set_Text_Size(2);
  
  my_lcd.Print_String(text_x_axis,x,y);
  my_lcd.Print_String(text_y_axis,x,y+18);

  Legends_Update_X();
  Legends_Update_Y();

}

void Legends_Update_X()
{
  my_lcd.Set_Text_colour(255, 255, 255);
  my_lcd.Set_Text_Back_colour(0x0000);
  my_lcd.Set_Text_Size(2);
  my_lcd.Print_Number_Int(long(max_limit_x/10),legend_x+160,legend_y, 3,' ', 10);
}

void Legends_Update_Y()
{
  my_lcd.Set_Text_colour(255, 255, 255);
  my_lcd.Set_Text_Back_colour(0x0000);
  my_lcd.Set_Text_Size(2);
  my_lcd.Print_Number_Int(long((Scale_exp*max_limit_y)/10),legend_x+160,legend_y+18, 3,' ', 10);
}

void Legends_Max(uint16_t x, uint16_t y)
{

  uint8_t *text_x_axis = "X-Axis :1 ut=    ch";
  uint8_t *text_y_axis = "Y-Axis :1 ut=    cps";

  legend_x = x;
  legend_y = y;
 
  my_lcd.Set_Text_colour(255, 255, 255);
  my_lcd.Set_Text_Back_colour(0x0000);
  my_lcd.Set_Text_Size(2);
  
  my_lcd.Print_String(text_x_axis,x,y);
  my_lcd.Print_String(text_y_axis,x,y+18);

  Legends_Update_X_Max();
  Legends_Update_Y_Max();

}

void Legends_Update_X_Max()
{
  my_lcd.Set_Text_colour(255, 255, 255);
  my_lcd.Set_Text_Back_colour(0x0000);
  my_lcd.Set_Text_Size(2);
  my_lcd.Print_Number_Int(long((my_lcd.Get_Width()-x_origin)/10),legend_x+160,legend_y, 3,' ', 10);
}

void Legends_Update_Y_Max()
{
  my_lcd.Set_Text_colour(255, 255, 255);
  my_lcd.Set_Text_Back_colour(0x0000);
  my_lcd.Set_Text_Size(2);
  my_lcd.Print_Number_Int(long((Scale_exp_max*max_limit_y)/10),legend_x+160,legend_y+18, 3,' ', 10);
}

void Record_Data(uint16_t adc_value) // Recording the data everytime a interrupt is generated
{
  uint16_t max_channel_now = max_channel;
  record_data[adc_value] = record_data[adc_value]+1; // The record data array with 1024 elements are increased accordingly depending on what the ADC value is
  if(millis() - time_now_max >= 20000)
  {
  time_now_max = millis();
  max_channel_now = Max_Value(record_data,BUFFER);
  if(Screen3 or Screen2)
  {
    Update_Max_Value();
  }
  }
  if(record_data[adc_value] >= max_limit_y*Scale_exp_max)
  {
    Scale_max = Scale_max +1; // Scale is incremented by one
    Scale_exp_max = Scale_exp_max*2;

     if(flag_start_max == 1 && Screen3) // If Screen is 3 and Draw Spectrum has been run initialized then this will Refresh the spectrum and also change the y-axis values
  {
    Draw_Spectrum_Max(); // Refresh the Spectrum
    Update_Y_Axis_Max(); // Update the Y-axis Scalings
    Legends_Update_Y_Max();
  }
  }

  if(flag_start_max == 1 && Screen3) // If Screen is 2 and Draw Spectrum has been run initialized then this will update the individual values that have changed
  {
    if( abs(max_channel_now - max_channel) > 25 )
    {
      max_channel = max_channel_now;
      flag_start_max = 0;
      Draw_Spectrum_Max(); 
    }
    Update_Spectrum_Max(adc_value); // This is to update the spectrum
  }


  
  adc_value = int((adc_value*(my_lcd.Get_Width()-x_origin))/BUFFER); // Compressing the adc value for record data compressed array
  record_data_compressed[adc_value] = record_data_compressed[adc_value] + 1; // Increasing the Record data compressed array values 
  if(record_data_compressed[adc_value] >= max_limit_y*Scale_exp) // This is to update the Spectrum when the height reaches the specified value. In this case 100*Scale
  {
    Scale = Scale+1; // Scale is incremented by one
    Scale_exp = Scale_exp*2;
    
    if(flag_start == 1 && Screen2) // If Screen is 2 and Draw Spectrum has been run initialized then this will Refresh the spectrum and also change the y-axis values
  {
    Draw_Spectrum_Full(); // Refresh the Spectrum
    Update_Y_Axis(); // Update the Y-axis Scalings
    Legends_Update_Y();
  }
  
  }

  
  if(flag_start == 1 && Screen2) // If Screen is 2 and Draw Spectrum has been run initialized then this will update the individual values that have changed
  {
    Update_Spectrum_Full(adc_value); // This is to update the spectrum
  }

}

uint16_t Max_Value(uint32_t *arr, uint16_t len)
{
  uint32_t compare = 0;
  uint16_t channel=0;
  for(uint16_t i=0; i < len; i++)
  {
    if(compare < arr[i])
    {
      compare = arr[i];
      channel = i;
    }
  }
  return channel;
}

void Make_Max_Value()
{
  uint8_t *text_max_value = "MAX CPS";
  my_lcd.Set_Text_colour(255, 255, 255);
  my_lcd.Set_Text_Back_colour(0x0000);
  my_lcd.Set_Text_Size(2);
  my_lcd.Print_String(text_max_value,legend_x-100,legend_y);
  Update_Max_Value();
}

void Update_Max_Value()
{
  uint16_t max_channel = Max_Value(record_data,BUFFER);
  Serial.println(max_channel);
  my_lcd.Print_Number_Int(long(max_channel),legend_x-100,legend_y+18, 4,' ', 10);
}
