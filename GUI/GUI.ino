#include <LCDWIKI_GUI.h> //Core graphics library
#include <LCDWIKI_KBV.h> //Hardware-specific library

LCDWIKI_KBV my_lcd(320,480,A3,A2,A1,A0,A4);//width,height,cs,cd,wr,rd,reset

struct Options_List{
  String main_option;
  String sub_option[6];
  uint8_t no_sub_option;
  uint8_t count_r;
};

String subOption1[] = {"Data1","Data2","Data3"};
uint8_t count_right = 0;
String space = "            ";
uint8_t set_point = 0;

String button;
uint8_t flag_down;
uint8_t flag_up;

const uint8_t no_options = 4;
struct Options_List Opt[no_options-1]; 

void setup() {
  
 Serial.begin(9600); 
 
 Opt[0].main_option = "OPTION 1 :";
 Opt[0].sub_option[0] = "Data1";
 Opt[0].sub_option[1] = "Data2";
 Opt[0].sub_option[2] = "Data3";
 Opt[0].no_sub_option = 3;
 Opt[0].count_r = 0;

 Opt[1].main_option = "OPTION 2 :";
 Opt[1].sub_option[0] = "Data1";
 Opt[1].sub_option[1] = "Data2";
 Opt[1].sub_option[2] = "Data3";
 Opt[1].no_sub_option = 3;
 Opt[1].count_r = 0;

 Opt[2].main_option = "OPTION 3 :";
 Opt[2].sub_option[0] = "Data1";
 Opt[2].sub_option[1] = "Data2";
 Opt[2].sub_option[2] = "Data3";
 Opt[2].no_sub_option = 3;
 Opt[2].count_r = 0;

 Opt[3].main_option = "OPTION 4 :";
 Opt[3].sub_option[0] = "Data1";
 Opt[3].sub_option[1] = "Data2";
 Opt[3].sub_option[2] = "Data3";
 Opt[3].no_sub_option = 3;
 Opt[3].count_r = 0;
    
 my_lcd.Init_LCD();
 my_lcd.Set_Rotation(1);  
 Main_Screen();

 my_lcd.Set_Text_colour(0, 0, 0);
 my_lcd.Set_Text_Size(2);
 my_lcd.Print_String("->", 45, my_lcd.Get_Display_Height()*(0.3));

if(no_options<4)
{
 for(int i=0; i<no_options; i++)
 {
  my_lcd.Set_Text_Size(2);
  my_lcd.Print_String(space, 75, my_lcd.Get_Display_Height()*(0.3+i*0.2));
  my_lcd.Print_String(Opt[i].main_option, 75, my_lcd.Get_Display_Height()*(0.3+i*0.2));
  my_lcd.Print_String(space, 300, my_lcd.Get_Display_Height()*(0.3+i*0.2));
  my_lcd.Print_String(Opt[i].sub_option[0], 300, my_lcd.Get_Display_Height()*(0.3+i*0.2));
 }
} 
else
{
 for(int i=0; i<3; i++)
 {
  my_lcd.Set_Text_Size(2);
  my_lcd.Print_String(space, 75, my_lcd.Get_Display_Height()*(0.3+i*0.2));
  my_lcd.Print_String(Opt[i].main_option, 75, my_lcd.Get_Display_Height()*(0.3+i*0.2));
  my_lcd.Print_String(space, 300, my_lcd.Get_Display_Height()*(0.3+i*0.2));
  my_lcd.Print_String(Opt[i].sub_option[0], 300, my_lcd.Get_Display_Height()*(0.3+i*0.2));
 } 
}
}

void loop() {
  
  if(Serial.available()>0)
  {
    button = Serial.readString();
    button.trim();
    ScrollDown();
    SubOption(); 
    button=""; 
  }

}

void Main_Screen()
{
  my_lcd.Fill_Screen(245, 194, 66);
  my_lcd.Set_Draw_color(0,0,0);
  my_lcd.Draw_Round_Rectangle(5,5,475, 315, 2);
  my_lcd.Draw_Round_Rectangle(6,6,474, 314, 2);
  my_lcd.Set_Text_colour(28, 81, 128);
  my_lcd.Set_Text_Size(3);
  my_lcd.Set_Text_Back_colour(245, 194, 66);
  my_lcd.Set_Text_Mode(0);
  my_lcd.Print_String("SHAKTI PORTABLE MCA", CENTER, my_lcd.Get_Display_Height()*0.1);
}

void ScrollDown()
{
  if(button == "2")
  {
    if(set_point < no_options-1)
    {
      set_point = set_point+1;
      if(flag_down==0)
      {
      for(int i=0; i<3; i++)
      {
        my_lcd.Print_String("  ", 45, my_lcd.Get_Display_Height()*(0.3 + i*0.2));
      }
      
      my_lcd.Print_String("->", 45, my_lcd.Get_Display_Height()*(0.3 + set_point*0.2));
      }
      else
      {
        if(no_options-1-set_point>3)
        {
        for(int i=0; i<3; i++)
         {
          my_lcd.Set_Text_Size(2);
          my_lcd.Print_String(space, 75, my_lcd.Get_Display_Height()*(0.3+i*0.2));
          my_lcd.Print_String(Opt[set_point+i].main_option, 75, my_lcd.Get_Display_Height()*(0.3+i*0.2));
          my_lcd.Print_String(space, 300, my_lcd.Get_Display_Height()*(0.3+i*0.2));
          my_lcd.Print_String(Opt[set_point+i].sub_option[Opt[set_point+i].count_r], 300, my_lcd.Get_Display_Height()*(0.3+i*0.2));
         } 
        }
        else if(no_options-1-set_point<=3)
        {
        for(int i=0; i<no_options-1-set_point; i++)
         {
          my_lcd.Set_Text_Size(2);
          my_lcd.Print_String(space, 75, my_lcd.Get_Display_Height()*(0.3+i*0.2));
          my_lcd.Print_String(Opt[set_point+i].main_option, 75, my_lcd.Get_Display_Height()*(0.3+i*0.2));
          my_lcd.Print_String(space, 300, my_lcd.Get_Display_Height()*(0.3+i*0.2));
          my_lcd.Print_String(Opt[set_point+i].sub_option[Opt[set_point+i].count_r], 300, my_lcd.Get_Display_Height()*(0.3+i*0.2));
         } 
        }
        flag_down=0;
      }
      if( (set_point+1)%3 == 0 )
      {
        flag_down=1;
      }
    }
  }

  if(button == "8")
  {
    if(set_point>0)
    {
      set_point = set_point-1;
      
      for(int i=0; i<3; i++)
      {
        my_lcd.Print_String("  ", 45, my_lcd.Get_Display_Height()*(0.3 + i*0.2));
      }
      
      my_lcd.Print_String("->", 45, my_lcd.Get_Display_Height()*(0.3 + set_point*0.2));
    }
    
   }
}


void SubOption()
{
    if(button =="6")
    {
       if(Opt[set_point].count_r < Opt[set_point].no_sub_option-1)
       { 
       Opt[set_point].count_r = Opt[set_point].count_r + 1;    
       my_lcd.Set_Text_Size(2);
       my_lcd.Print_String(space, 300, my_lcd.Get_Display_Height()*(0.3+set_point*0.2));
       my_lcd.Print_String(Opt[set_point].sub_option[Opt[set_point].count_r], 300, my_lcd.Get_Display_Height()*(0.3+set_point*0.2));
       }
    }
    if(button =="4")
    {
      if(Opt[set_point].count_r>0)
       { 
       Opt[set_point].count_r = Opt[set_point].count_r - 1;    
       my_lcd.Set_Text_Size(2);
       my_lcd.Print_String(space, 300, my_lcd.Get_Display_Height()*(0.3+set_point*0.2));
       my_lcd.Print_String(Opt[set_point].sub_option[Opt[set_point].count_r], 300, my_lcd.Get_Display_Height()*(0.3+set_point*0.2));
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
