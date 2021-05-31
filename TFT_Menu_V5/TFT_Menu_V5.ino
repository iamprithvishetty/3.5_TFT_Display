#include <LCD_SHAKTI.h>
#include <math.h> //Math Library
#include "stdlib.h"
#include "string.h"

#define NO_OPTIONS 12
#define MAX_VISIBLE 6

#define UP 65
#define DOWN 66
#define LEFT 68
#define RIGHT 67
#define ENTER 10

LCD_SHAKTI my_lcd(0x9486,480,320);

uint8_t* space_1 = "  ";
uint8_t* arrow = "->";
uint8_t *space_big = "                       ";
uint8_t *space_small = "                ";
uint8_t *colon = ": ";

uint8_t* shakti_port = "FUNCTION GENERATOR";

struct Sub_Option_List{
  uint8_t *sub_option;
  uint8_t *sub_option_list[10];
  uint8_t no_sub_option_list;
  uint8_t count_right; 
};

struct Options_List{
  uint8_t* main_option;
  struct Sub_Option_List Sub_Option[15];
  uint8_t no_sub_option;
};

uint8_t* space = "                  ";
uint8_t set_point = 0;
uint8_t set_point_enter = 0;


uint8_t flag_down = 0;
uint8_t flag_up = 0;
bool flag_enter = false;
uint8_t flag_down_enter = 0;
uint8_t flag_up_enter = 0;
char button;


struct Options_List Opt[NO_OPTIONS-1]; 

void setup() {
  
 Serial.begin(19200); 
 
 Opt[0].main_option = "TRIGGER MODE";
 
 Opt[1].main_option = "PULSE SETTINGS";
 Opt[1].no_sub_option = 12;
 Opt[1].Sub_Option[0].sub_option = "PB-5 Pulse";
 Opt[1].Sub_Option[0].no_sub_option_list = 2;
 Opt[1].Sub_Option[0].count_right = 0;
 Opt[1].Sub_Option[0].sub_option_list[0] = "ON";
 Opt[1].Sub_Option[0].sub_option_list[1] = "OFF";
 Opt[1].Sub_Option[1].sub_option = "Amp1";
 Opt[1].Sub_Option[1].no_sub_option_list = 1;
 Opt[1].Sub_Option[1].count_right = 0;
 Opt[1].Sub_Option[1].sub_option_list[0] = "1.0000V/keV";
 Opt[1].Sub_Option[2].sub_option = "Rate";
 Opt[1].Sub_Option[2].no_sub_option_list = 1;
 Opt[1].Sub_Option[2].count_right = 0;
 Opt[1].Sub_Option[2].sub_option_list[0] = "100kHz";
 Opt[1].Sub_Option[3].sub_option = "Width";
 Opt[1].Sub_Option[3].no_sub_option_list = 1;
 Opt[1].Sub_Option[3].count_right = 0;
 Opt[1].Sub_Option[3].sub_option_list[0] = "500ns";
 Opt[1].Sub_Option[4].sub_option = "Delay";
 Opt[1].Sub_Option[4].no_sub_option_list = 1;
 Opt[1].Sub_Option[4].count_right = 0;
 Opt[1].Sub_Option[4].sub_option_list[0] = "250ns";
 Opt[1].Sub_Option[5].sub_option = "Rise Time";
 Opt[1].Sub_Option[5].no_sub_option_list = 8;
 Opt[1].Sub_Option[5].count_right = 0;
 Opt[1].Sub_Option[5].sub_option_list[0] = ".05us";
 Opt[1].Sub_Option[5].sub_option_list[1] = ".1us";
 Opt[1].Sub_Option[5].sub_option_list[2] = ".2us";
 Opt[1].Sub_Option[5].sub_option_list[3] = ".5us";
 Opt[1].Sub_Option[5].sub_option_list[4] = "1us";
 Opt[1].Sub_Option[5].sub_option_list[5] = "2us";
 Opt[1].Sub_Option[5].sub_option_list[6] = "5us";
 Opt[1].Sub_Option[5].sub_option_list[7] = "10us";
 Opt[1].Sub_Option[6].sub_option = "Fall Time";
 Opt[1].Sub_Option[6].no_sub_option_list = 10;
 Opt[1].Sub_Option[6].count_right = 0;
 Opt[1].Sub_Option[6].sub_option_list[0] = ".05us";
 Opt[1].Sub_Option[6].sub_option_list[1] = "1us";
 Opt[1].Sub_Option[6].sub_option_list[2] = "2us";
 Opt[1].Sub_Option[6].sub_option_list[3] = "10us";
 Opt[1].Sub_Option[6].sub_option_list[4] = "20us";
 Opt[1].Sub_Option[6].sub_option_list[5] = "50us";
 Opt[1].Sub_Option[6].sub_option_list[6] = "100us";
 Opt[1].Sub_Option[6].sub_option_list[7] = "200us";
 Opt[1].Sub_Option[6].sub_option_list[8] = "500us";
 Opt[1].Sub_Option[6].sub_option_list[9] = "1.0ms";
 Opt[1].Sub_Option[7].sub_option = "Polarity";
 Opt[1].Sub_Option[7].no_sub_option_list = 2;
 Opt[1].Sub_Option[7].count_right = 0;
 Opt[1].Sub_Option[7].sub_option_list[0] = "Pos";
 Opt[1].Sub_Option[7].sub_option_list[1] = "Neg";
 Opt[1].Sub_Option[8].sub_option = "Pulse Top";
 Opt[1].Sub_Option[8].no_sub_option_list = 1;
 Opt[1].Sub_Option[8].count_right = 0;
 Opt[1].Sub_Option[8].sub_option_list[0] = "Flat";
 Opt[1].Sub_Option[9].sub_option = "Atten";
 Opt[1].Sub_Option[9].no_sub_option_list = 10;
 Opt[1].Sub_Option[9].count_right = 0;
 Opt[1].Sub_Option[9].sub_option_list[0] = "1X";
 Opt[1].Sub_Option[9].sub_option_list[1] = "2X";
 Opt[1].Sub_Option[9].sub_option_list[2] = "5X";
 Opt[1].Sub_Option[9].sub_option_list[3] = "10X";
 Opt[1].Sub_Option[9].sub_option_list[4] = "20X";
 Opt[1].Sub_Option[9].sub_option_list[5] = "50X";
 Opt[1].Sub_Option[9].sub_option_list[6] = "100X";
 Opt[1].Sub_Option[9].sub_option_list[7] = "200X";
 Opt[1].Sub_Option[9].sub_option_list[8] = "500X";
 Opt[1].Sub_Option[9].sub_option_list[9] = "1000X";
 Opt[1].Sub_Option[10].sub_option = "Clamp";
 Opt[1].Sub_Option[10].no_sub_option_list = 2;
 Opt[1].Sub_Option[10].sub_option_list[0] = "ON";
 Opt[1].Sub_Option[10].sub_option_list[1] = "OFF";
 Opt[1].Sub_Option[11].sub_option = "[Return]";
 Opt[1].Sub_Option[11].no_sub_option_list = 0;
 
 Opt[2].main_option = "RAMP SETTINGS";
 Opt[3].main_option = "SCALE V/keV";
 Opt[4].main_option = "SAVE/RECALL";
 Opt[5].main_option = "OPERATING MODE";
 Opt[6].main_option = "TEST MODE 1"; 
 Opt[7].main_option = "TEST MODE 2"; 
 Opt[8].main_option = "TEST MODE 3"; 
 Opt[9].main_option = "TEST MODE 4";  
 Opt[10].main_option = "TEST MODE 5"; 
 Opt[11].main_option = "TEST MODE 6"; 
 
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
    Serial.println((int)button);
    if(flag_enter == false)
    {
    ScrollDown();
    }
    else if(flag_enter == true)
    {
     ScrollDownEnter(); 
     ScrollRight();
    }
    Enter_Button();
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
  my_lcd.Print_String(shakti_port, 480/2 - 16*mystrlen(shakti_port)/2, my_lcd.Get_Display_Height()*0.1);
}

void ScrollDown()
{

  if(button == DOWN)
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

  if(button == UP)
  {
    Serial.println("True 8");
    if(set_point>0)
    {
      set_point = set_point-1;
      if((set_point+1)%MAX_VISIBLE == 0 and set_point!= 0)
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

void Enter_Button()
{
    
  if(button == ENTER)
  {
    if(flag_enter == false)
    {
    flag_enter = true;
    set_point_enter = 0;
    flag_down_enter = 0;
    flag_up_enter = 0;
    my_lcd.Set_Text_Size(3);
    my_lcd.Set_Text_Back_colour(42, 150, 138);
    my_lcd.Set_Text_colour(0, 0, 0);
    my_lcd.Set_Text_Mode(0);
    my_lcd.Print_String(space_big, 20, my_lcd.Get_Display_Height()*0.1);
    my_lcd.Print_String(Opt[set_point].main_option, 480/2 - 16*mystrlen(Opt[set_point].main_option)/2, my_lcd.Get_Display_Height()*0.1);
    my_lcd.Set_Text_colour(255, 255, 255);
  
    for(int i=0; i< MAX_VISIBLE;i++)
    {
       my_lcd.Set_Text_Size(2);
       my_lcd.Print_String(space, 170, my_lcd.Get_Display_Height()*(0.3+i*0.1));
       my_lcd.Print_String(space_1, 140, my_lcd.Get_Display_Height()*(0.3 + i*0.1));  
    }
    my_lcd.Print_String(arrow, 90, my_lcd.Get_Display_Height()*(0.3 + (0)*0.1));
    if(Opt[set_point].no_sub_option+1 < MAX_VISIBLE)
    {
    for(int i=0; i< Opt[set_point].no_sub_option;i++)
    {
       my_lcd.Set_Text_Size(2);
       my_lcd.Print_String(space, 120, my_lcd.Get_Display_Height()*(0.3+i*0.1));
       my_lcd.Print_String(Opt[set_point].Sub_Option[i].sub_option, 120, my_lcd.Get_Display_Height()*(0.3+i*0.1));
       if( Opt[set_point].Sub_Option[i].no_sub_option_list != 0)
       {
       my_lcd.Print_String(colon,270,my_lcd.Get_Display_Height()*(0.3+i*0.1));
       my_lcd.Print_String(Opt[set_point].Sub_Option[i].sub_option_list[Opt[set_point].Sub_Option[i].count_right],285,my_lcd.Get_Display_Height()*(0.3+i*0.1));
       }
    }
    }

    else if(Opt[set_point].no_sub_option+1 >= MAX_VISIBLE)
    {
      for(int i=0; i< MAX_VISIBLE;i++)
    {
       my_lcd.Set_Text_Size(2);
       my_lcd.Print_String(space, 120, my_lcd.Get_Display_Height()*(0.3+i*0.1));
       my_lcd.Print_String(Opt[set_point].Sub_Option[i].sub_option, 120, my_lcd.Get_Display_Height()*(0.3+i*0.1));
       if( Opt[set_point].Sub_Option[i].no_sub_option_list != 0)
       {
       my_lcd.Print_String(colon,270,my_lcd.Get_Display_Height()*(0.3+i*0.1));
       my_lcd.Print_String(Opt[set_point].Sub_Option[i].sub_option_list[Opt[set_point].Sub_Option[i].count_right],285,my_lcd.Get_Display_Height()*(0.3+i*0.1));
       }
    }
    }
    }

    else if(flag_enter == true and set_point_enter == Opt[set_point].no_sub_option -1)
    {
      Back_To_MainScreen();
    }
  }
}

void ScrollDownEnter()
{
  if(button == DOWN)
  {
    Serial.println("True 2 Enter");
    if(set_point_enter < Opt[set_point].no_sub_option -1 )
    {
      set_point_enter = set_point_enter+1;   
      if(set_point_enter%(MAX_VISIBLE) == 0 )
      {
        Serial.println("flag enter down");
        flag_down_enter=1;
      } 
      if(flag_down_enter==0)
      {
      for(int i=0; i< MAX_VISIBLE; i++)
      {
        my_lcd.Print_String(space_1, 90, my_lcd.Get_Display_Height()*(0.3 + i*0.1)); 
      }
      
      my_lcd.Print_String(arrow, 90, my_lcd.Get_Display_Height()*(0.3 + (set_point_enter%MAX_VISIBLE)*0.1));
      }
      
      else
      {
          for(int i=0; i< MAX_VISIBLE; i++)
        {
          my_lcd.Print_String(space_1, 90, my_lcd.Get_Display_Height()*(0.3 + i*0.1));  
        }    
        my_lcd.Print_String(arrow, 90, my_lcd.Get_Display_Height()*(0.3 + 0*0.1));
        
        if(Opt[set_point].no_sub_option-set_point_enter+1 > MAX_VISIBLE)
        {
        for(int i=0; i<MAX_VISIBLE; i++)
         {
          my_lcd.Set_Text_Size(2);
          my_lcd.Print_String(space, 120, my_lcd.Get_Display_Height()*(0.3+i*0.1));
          my_lcd.Print_String(space_small,270,my_lcd.Get_Display_Height()*(0.3+i*0.1));
          my_lcd.Print_String(Opt[set_point].Sub_Option[set_point_enter+i].sub_option, 120, my_lcd.Get_Display_Height()*(0.3+i*0.1));
          if(Opt[set_point].Sub_Option[set_point_enter+i].no_sub_option_list != 0)
          {
          my_lcd.Print_String(colon,270,my_lcd.Get_Display_Height()*(0.3+i*0.1));
          my_lcd.Print_String(Opt[set_point].Sub_Option[set_point_enter+i].sub_option_list[Opt[set_point].Sub_Option[set_point_enter+i].count_right],285,my_lcd.Get_Display_Height()*(0.3+i*0.1));
          }
         } 
        }
        
        else if(Opt[set_point].no_sub_option-set_point_enter+1<= MAX_VISIBLE)
        {
        for(int i=0; i< Opt[set_point].no_sub_option-set_point_enter; i++)
         {
          my_lcd.Set_Text_Size(2);
          my_lcd.Print_String(space, 120, my_lcd.Get_Display_Height()*(0.3+i*0.1));
          my_lcd.Print_String(space_small,270,my_lcd.Get_Display_Height()*(0.3+i*0.1));
          my_lcd.Print_String(Opt[set_point].Sub_Option[set_point_enter+i].sub_option, 120, my_lcd.Get_Display_Height()*(0.3+i*0.1));
          if(Opt[set_point].Sub_Option[set_point_enter+i].no_sub_option_list != 0)
          {
          my_lcd.Print_String(colon,270,my_lcd.Get_Display_Height()*(0.3+i*0.1));
          my_lcd.Print_String(Opt[set_point].Sub_Option[set_point_enter+i].sub_option_list[Opt[set_point].Sub_Option[set_point_enter+i].count_right],285,my_lcd.Get_Display_Height()*(0.3+i*0.1));
          }
         } 
         for(int i=Opt[set_point].no_sub_option-set_point_enter; i<MAX_VISIBLE; i++)
         {
          my_lcd.Set_Text_Size(2);
          my_lcd.Print_String(space, 120, my_lcd.Get_Display_Height()*(0.3+i*0.1));
          my_lcd.Print_String(space_small,270,my_lcd.Get_Display_Height()*(0.3+i*0.1));  
         } 
        }
        flag_down_enter=0;
      }

    }
  }

  if(button == UP)
  {
    Serial.println("True 8");
    if(set_point_enter>0)
    {
      set_point_enter = set_point_enter-1;
      if((set_point_enter+1)%MAX_VISIBLE == 0 and set_point_enter!= 0)
      {
        Serial.println("flag up");
        flag_up_enter=1;
      }
      if(flag_up_enter == 0)
      {
      for(int i=0; i<MAX_VISIBLE; i++)
      {
        my_lcd.Print_String(space_1, 90, my_lcd.Get_Display_Height()*(0.3 + i*0.1));  
      }
      
      my_lcd.Print_String(arrow, 90, my_lcd.Get_Display_Height()*(0.3 + (set_point_enter%MAX_VISIBLE)*0.1));
     }

      else
      {
      for(int i=0; i< MAX_VISIBLE; i++)
        {
          my_lcd.Print_String(space_1, 90, my_lcd.Get_Display_Height()*(0.3 + i*0.1));
        }    
        my_lcd.Print_String(arrow, 90, my_lcd.Get_Display_Height()*(0.3 + (MAX_VISIBLE-1)*0.1));
        
      for(int i=0; i<MAX_VISIBLE; i++)
         {
          my_lcd.Set_Text_Size(2);
          my_lcd.Print_String(space, 120, my_lcd.Get_Display_Height()*(0.3+i*0.1));
          my_lcd.Print_String(space_small,270,my_lcd.Get_Display_Height()*(0.3+i*0.1));
          my_lcd.Print_String(Opt[set_point].Sub_Option[abs(set_point_enter-MAX_VISIBLE)-1+i].sub_option, 120, my_lcd.Get_Display_Height()*(0.3+i*0.1));
          if( Opt[set_point].Sub_Option[abs(set_point_enter-MAX_VISIBLE)-1+i].no_sub_option_list != 0)
          {
          my_lcd.Print_String(colon,270,my_lcd.Get_Display_Height()*(0.3+i*0.1));
          my_lcd.Print_String(Opt[set_point].Sub_Option[abs(set_point_enter-MAX_VISIBLE)-1+i].sub_option_list[Opt[set_point].Sub_Option[abs(set_point_enter-MAX_VISIBLE)-1+i].count_right],285,my_lcd.Get_Display_Height()*(0.3+i*0.1));
          }
         } 
         flag_up_enter = 0;
    }
    
   }
}
}

void Back_To_MainScreen()
{
  uint8_t page = set_point/(MAX_VISIBLE-1);
  flag_enter = false;
  my_lcd.Set_Text_Size(3);
  my_lcd.Set_Text_Back_colour(42, 150, 138);
  my_lcd.Set_Text_colour(0, 0, 0);
  my_lcd.Set_Text_Mode(0);
  my_lcd.Print_String(space_big, 20, my_lcd.Get_Display_Height()*0.1);
  my_lcd.Print_String(shakti_port, 480/2 - 16*mystrlen(shakti_port)/2, my_lcd.Get_Display_Height()*0.1);
  my_lcd.Set_Text_colour(255, 255, 255);
  
    if(NO_OPTIONS <= MAX_VISIBLE)
  {
   for(int i=0; i< NO_OPTIONS; i++)
   {
    my_lcd.Set_Text_Size(2);
    my_lcd.Print_String(space_1, 90, my_lcd.Get_Display_Height()*(0.3 + i*0.1));
    my_lcd.Print_String(space, 120, my_lcd.Get_Display_Height()*(0.3+i*0.1));
    my_lcd.Print_String(Opt[i].main_option, 170, my_lcd.Get_Display_Height()*(0.3+i*0.1));
   }
    my_lcd.Print_String(arrow, 140, my_lcd.Get_Display_Height()*(0.3 + (set_point%(MAX_VISIBLE-1))*0.1));
  } 
  else
  {
   for(int i=0; i< MAX_VISIBLE; i++)
   {
    my_lcd.Set_Text_Size(2);
    my_lcd.Print_String(space_1, 90, my_lcd.Get_Display_Height()*(0.3 + i*0.1));
    my_lcd.Print_String(space, 120, my_lcd.Get_Display_Height()*(0.3+i*0.1));
    my_lcd.Print_String(Opt[page*MAX_VISIBLE+i].main_option, 170, my_lcd.Get_Display_Height()*(0.3+i*0.1));
   } 
   my_lcd.Print_String(arrow, 140, my_lcd.Get_Display_Height()*(0.3 + (set_point%(MAX_VISIBLE-1))*0.1));
  }
  
}

void ScrollRight()
{
  if(button == RIGHT)
  {
    if(Opt[set_point].Sub_Option[set_point_enter].no_sub_option_list > 1)
    {
     if(Opt[set_point].Sub_Option[set_point_enter].count_right == Opt[set_point].Sub_Option[set_point_enter].no_sub_option_list-1)
     {
      Opt[set_point].Sub_Option[set_point_enter].count_right = 0;
      my_lcd.Print_String(space_small,270,my_lcd.Get_Display_Height()*(0.3+((set_point_enter%MAX_VISIBLE)*0.1)));
      my_lcd.Print_String(colon,270,my_lcd.Get_Display_Height()*(0.3+(set_point_enter%MAX_VISIBLE)*0.1));
      my_lcd.Print_String(Opt[set_point].Sub_Option[set_point_enter].sub_option_list[Opt[set_point].Sub_Option[set_point_enter].count_right],285,my_lcd.Get_Display_Height()*(0.3+(set_point_enter%MAX_VISIBLE)*0.1));
     }
     else if(Opt[set_point].Sub_Option[set_point_enter].count_right < Opt[set_point].Sub_Option[set_point_enter].no_sub_option_list-1)
     {
      Opt[set_point].Sub_Option[set_point_enter].count_right += 1;
      my_lcd.Print_String(space_small,270,my_lcd.Get_Display_Height()*(0.3+((set_point_enter%MAX_VISIBLE)*0.1)));
      my_lcd.Print_String(colon,270,my_lcd.Get_Display_Height()*(0.3+(set_point_enter%MAX_VISIBLE)*0.1));
      my_lcd.Print_String(Opt[set_point].Sub_Option[set_point_enter].sub_option_list[Opt[set_point].Sub_Option[set_point_enter].count_right],285,my_lcd.Get_Display_Height()*(0.3+(set_point_enter%MAX_VISIBLE)*0.1));
     }
    }
  }

  if(button == LEFT)
  {
    if(Opt[set_point].Sub_Option[set_point_enter].no_sub_option_list > 1)
    {
     if(Opt[set_point].Sub_Option[set_point_enter].count_right == 0)
     {
      Opt[set_point].Sub_Option[set_point_enter].count_right = Opt[set_point].Sub_Option[set_point_enter].no_sub_option_list-1;
      my_lcd.Print_String(space_small,270,my_lcd.Get_Display_Height()*(0.3+((set_point_enter%MAX_VISIBLE)*0.1)));
      my_lcd.Print_String(colon,270,my_lcd.Get_Display_Height()*(0.3+(set_point_enter%MAX_VISIBLE)*0.1));
      my_lcd.Print_String(Opt[set_point].Sub_Option[set_point_enter].sub_option_list[Opt[set_point].Sub_Option[set_point_enter].count_right],285,my_lcd.Get_Display_Height()*(0.3+(set_point_enter%MAX_VISIBLE)*0.1));
     }
     else if(Opt[set_point].Sub_Option[set_point_enter].count_right <= Opt[set_point].Sub_Option[set_point_enter].no_sub_option_list-1)
     {
      Opt[set_point].Sub_Option[set_point_enter].count_right -= 1;
      my_lcd.Print_String(space_small,270,my_lcd.Get_Display_Height()*(0.3+((set_point_enter%MAX_VISIBLE)*0.1)));
      my_lcd.Print_String(colon,270,my_lcd.Get_Display_Height()*(0.3+(set_point_enter%MAX_VISIBLE)*0.1));
      my_lcd.Print_String(Opt[set_point].Sub_Option[set_point_enter].sub_option_list[Opt[set_point].Sub_Option[set_point_enter].count_right],285,my_lcd.Get_Display_Height()*(0.3+(set_point_enter%MAX_VISIBLE)*0.1));
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
