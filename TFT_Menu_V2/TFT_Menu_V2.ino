#include <LCD_SHAKTI.h>
#include <math.h> //Math Library
#include "stdlib.h"
#include "string.h"

#define NO_OPTIONS 10
#define MAX_VISIBLE 6

LCD_SHAKTI my_lcd(0x9486,480,320);

uint8_t* space_1 = "  ";
uint8_t* arrow = "->";

struct Options_List{
  uint8_t* main_option;
  uint8_t* sub_option[6];
  uint8_t no_sub_option;
  uint8_t count_r;
};

uint8_t* subOption1[] = {"Data1","Data2","Data3"};
uint8_t count_right = 0;
uint8_t* space = "                  ";
uint8_t set_point = 0;


uint8_t flag_down = 0;
uint8_t flag_up = 0;
char button;

struct Options_List Opt[NO_OPTIONS-1]; 

void setup() {
  
 Serial.begin(19200); 
 
 Opt[0].main_option = "TRIGGER MODE";
 Opt[1].main_option = "PULSE SETTINGS";
 Opt[2].main_option = "RAMP SETTINGS";
 Opt[3].main_option = "SCALE V/keV";
 Opt[4].main_option = "SAVE/RECALL";
 Opt[5].main_option = "OPERATING MODE";
 Opt[6].main_option = "TEST MODE"; 
 Opt[7].main_option = "TEST MODE 2"; 
 Opt[8].main_option = "TEST MODE 3"; 
 Opt[9].main_option = "TEST MODE 4";  
 
 my_lcd.Init_LCD();
 my_lcd.Set_Rotation(1);  
 Main_Screen();

 my_lcd.Set_Text_colour(255, 255, 255);
 my_lcd.Set_Text_Size(2);
 my_lcd.Print_String(arrow, 140, my_lcd.Get_Display_Height()*(0.3));

if(NO_OPTIONS <= MAX_VISIBLE)
{
 for(int i=0; i< NO_OPTIONS; i++)
 {
  my_lcd.Set_Text_Size(2);
  my_lcd.Print_String(space, 170, my_lcd.Get_Display_Height()*(0.3+i*0.1));
  my_lcd.Print_String(Opt[i].main_option, 170, my_lcd.Get_Display_Height()*(0.3+i*0.1));
 }
} 
else
{
 for(int i=0; i< MAX_VISIBLE; i++)
 {
  my_lcd.Set_Text_Size(2);
  my_lcd.Print_String(space, 170, my_lcd.Get_Display_Height()*(0.3+i*0.1));
  my_lcd.Print_String(Opt[i].main_option, 170, my_lcd.Get_Display_Height()*(0.3+i*0.1));
 } 
}
}

void loop() {
  
  if(Serial.available()>0)
  {
    button = Serial.read();
    Serial.println(button);
    ScrollDown();
    button=""; 
  }

}

void Main_Screen()
{
  my_lcd.Fill_Screen(42, 150, 138);
  my_lcd.Set_Draw_color(255,255,255);
  my_lcd.Draw_Round_Rectangle(5,5,475, 315, 2);
  my_lcd.Draw_Round_Rectangle(6,6,474, 314, 2);
  my_lcd.Set_Text_colour(0, 0, 0);
  my_lcd.Set_Text_Size(3);
  my_lcd.Set_Text_Back_colour(42, 150, 138);
  my_lcd.Set_Text_Mode(0);
  uint8_t* shakti_port = "FUNCTION GENERATOR";
  my_lcd.Print_String(shakti_port, 480/2 - 16*mystrlen(shakti_port)/2, my_lcd.Get_Display_Height()*0.1);
}

void ScrollDown()
{

  if(button == 50)
  {
    Serial.println("True 2");
    if(set_point < NO_OPTIONS-1)
    {
      set_point = set_point+1;   
      if(set_point%(MAX_VISIBLE) == 0 )
      {
        Serial.println("flag down");
        flag_down=1;
      } 
      if(flag_down==0)
      {
      for(int i=0; i< MAX_VISIBLE; i++)
      {
        my_lcd.Print_String(space_1, 140, my_lcd.Get_Display_Height()*(0.3 + i*0.1));
      }
      
      my_lcd.Print_String(arrow, 140, my_lcd.Get_Display_Height()*(0.3 + (set_point%MAX_VISIBLE)*0.1));
      }
      
      else
      {
          for(int i=0; i< MAX_VISIBLE; i++)
        {
          my_lcd.Print_String(space_1, 140, my_lcd.Get_Display_Height()*(0.3 + i*0.1));
        }    
        my_lcd.Print_String(arrow, 140, my_lcd.Get_Display_Height()*(0.3 + 0*0.1));
        
        if(NO_OPTIONS-set_point+1 > MAX_VISIBLE)
        {
        for(int i=0; i<MAX_VISIBLE; i++)
         {
          my_lcd.Set_Text_Size(2);
          my_lcd.Print_String(space, 170, my_lcd.Get_Display_Height()*(0.3+i*0.1));
          my_lcd.Print_String(Opt[set_point+i].main_option, 170, my_lcd.Get_Display_Height()*(0.3+i*0.1));
         } 
        }
        
        else if(NO_OPTIONS-set_point+1<= MAX_VISIBLE)
        {
        for(int i=0; i< NO_OPTIONS-set_point; i++)
         {
          my_lcd.Set_Text_Size(2);
          my_lcd.Print_String(space, 170, my_lcd.Get_Display_Height()*(0.3+i*0.1));
          my_lcd.Print_String(Opt[set_point+i].main_option, 170, my_lcd.Get_Display_Height()*(0.3+i*0.1));
         } 
         for(int i=NO_OPTIONS-set_point; i<MAX_VISIBLE; i++)
         {
          my_lcd.Set_Text_Size(2);
          my_lcd.Print_String(space, 170, my_lcd.Get_Display_Height()*(0.3+i*0.1));
         } 
        }
        flag_down=0;
      }

    }
  }

  if(button == 56)
  {
    Serial.println("True 8");
    if(set_point>0)
    {
      set_point = set_point-1;
      if(set_point%(MAX_VISIBLE-1) == 0 and set_point!= 0)
      {
        Serial.println("flag up");
        flag_up=1;
      }
      if(flag_up == 0)
      {
      for(int i=0; i<MAX_VISIBLE; i++)
      {
        my_lcd.Print_String(space_1, 140, my_lcd.Get_Display_Height()*(0.3 + i*0.1));
      }
      
      my_lcd.Print_String(arrow, 140, my_lcd.Get_Display_Height()*(0.3 + (set_point%MAX_VISIBLE)*0.1));
     }

      else
      {
      for(int i=0; i< MAX_VISIBLE; i++)
        {
          my_lcd.Print_String(space_1, 140, my_lcd.Get_Display_Height()*(0.3 + i*0.1));
        }    
        my_lcd.Print_String(arrow, 140, my_lcd.Get_Display_Height()*(0.3 + (MAX_VISIBLE-1)*0.1));
        
      for(int i=0; i<MAX_VISIBLE; i++)
         {
          my_lcd.Set_Text_Size(2);
          my_lcd.Print_String(space, 170, my_lcd.Get_Display_Height()*(0.3+i*0.1));
          my_lcd.Print_String(Opt[abs(set_point-MAX_VISIBLE)-1+i].main_option, 170, my_lcd.Get_Display_Height()*(0.3+i*0.1));
         } 
         flag_up = 0;
    }
    
   }
}
}

int mystrlen(char *str)
{
    int len = 0;
    int i;
    
    for (i=0; str[i] != 0; i++) 
    {
        len++;
    }
    return(len-1);
}
