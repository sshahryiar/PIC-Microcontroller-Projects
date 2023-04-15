#include <18F452.h>


#device *= 16
#device ADC = 10


#fuses HS            
#fuses PUT                                
#fuses CPD                                                    
#fuses NOLVP                                             
#fuses NOWDT                                
#fuses NOWRT                                                      
#fuses NODEBUG                                                                                                
#fuses PROTECT                       
#fuses BROWNOUT                                  
                                                              
#fuses NOCPB
#fuses BORV42                       
#fuses CCP2C1                                         
#fuses NOWRTC
#fuses NOWRTB              
#fuses NOWRTD                                                     
#fuses NOEBTR                        
#fuses NOEBTRB
#fuses NOOSCSEN                                                    
#fuses NOSTVREN                
                          
#use delay (clock = 10MHz)                   


#include "HDM64GS12.c"            
#include "graphics.c"        
#include "background.c" 
#include "sound.c"
#include <math.h>
                                     
#define  reload  63036     
                                          
#define set      input(pin_D0)   
#define inc      input(pin_D1)
#define dec      input(pin_D2)   
#define start    input(pin_D3)

#define LED      pin_D4
#define load     pin_D7
  
#define T_lim    60
#define V_cnt    442
#define I_cnt    205
                           
             
unsigned char txt1[15];    
unsigned char txt2[15];  
unsigned char txt3[15];                 
unsigned char txt4[15];      
unsigned char txt5[15]; 
unsigned char txt6[15];  
unsigned char txt7[15];   
unsigned char txt8[15]; 
unsigned char txt9[15]; 
unsigned char txt10[15];  
unsigned char txt11[15]; 
unsigned char txt12[15]; 
unsigned char text[15];        

unsigned long V[100]; 
unsigned long I[100];
unsigned long T[100]; 
                
short id = 1;
short log_done = 0;
short logging = OFF; 
short done_setting = 1; 

unsigned char l = 0;     
unsigned char x = 0;       
unsigned char pos = 0;
                                   
signed char s = 1; 
signed char min = 0; 
signed char hr = 0;                             
signed char s_t = 0; 
signed char min_t = 0; 
signed char hr_t = 0; 
                                                                                                                                                    
register unsigned long ms = 0;
                                
float P_max = 0; 
float P_min = 0;                                      
float I_max = 0;
float I_min = 0;
float I_lim = 0.1;                        
float V_lim = 5.0;                  
float V_max = 0;
float V_min = 0;                 
float T_max = 0;
float T_min = 0;  
float P = 0;                                    
float PV_P = 0;
float PV_I = 0;
float PV_V = 0;   
float PV_T = 0;  

                                                                        
void setup();            
unsigned char scan_keys();               
void intro();                               
void draw_background();                  
float map(float v, float x_min, float x_max, float y_min, float y_max)  ; 
float constrain(float value, float max, float min);
void clear_screen_1();                                
void clear_screen_2(); 
unsigned long adc_rms(unsigned char channel); 
void read_parameters();
void compare_values(); 
signed char change_value(unsigned char x, unsigned char y, signed  char value, signed char low, signed char high); 
float change_value_f(unsigned char x, unsigned char y, float, float low, float high);    
void settings();                           
void set_parameters();                             
void general_display();
void plot_data(short type, unsigned char parameter);  


#int_RTCC                       

void time_keeping()                        
{       
   set_RTCC(reload);  
   ms++;
   if(ms > 999)
   {
         ms = 0;                                 
         s_t -= 1;   
         if(s_t < 0)
         {
              s_t = 59;
              min_t -= 1;
              if(min_t < 0)
              {                                 
                    min_T = 59;
                    hr_t -= 1;
                    if(hr_t < 0)
                    {
                        hr_t = 23;
                    }   
              }  
         }
   }                                          
   if((s_t == 0) && (min_t == 0) && (hr_t == 0) && (x < 100))
   {                   
         s_t = s;            
         min_t = min;
         hr_t = hr;   
         log_done = 1;
   }                       
   if(x > 99)      
   {                                      
       logging = OFF;
       id = 1;
       pos = 0;
       output_low(LED);
       setup_timer_0(t0_off);
       disable_interrupts(global);
       disable_interrupts(int_RTCC);  
   }   
}                                  
                                                       
                                                                     
void main()                          
{                                  
    unsigned char k = 1;
    unsigned char clr = 0;
    setup();
    intro();                               
    draw_background();                       
    while(TRUE)                                                                           
    {                                         
         set_parameters(); 
         read_parameters();
         compare_values();                         
         general_display(); 
         
         if(scan_keys() == 4)    
         {                    
             while(scan_keys() == 4);
             
             logging = ~logging;
                                                                      
             if(logging)
             {          
                  log_done = 0; 
                  for(clr = 0; clr < 100; clr += 1)
                  {
                      glcd_line((12 + clr), 53, (12 + clr), 29, OFF);  
                  }                                
                  id = 0;
                  x = 0;
                  P_max = 0;                  
                  P_min = 20;
                  V_max = 0;    
                  V_min = 30;           
                  I_max = 0;    
                  I_min = 2;
                  T_max = 0;    
                  T_min = 70; 
                  output_high(LED);
                  enable_interrupts(global);
                  enable_interrupts(int_RTCC);   
                  setup_timer_0(t0_internal);   
                  set_RTCC(reload);        
            }                             
            else                                   
            {                
                id = 1;
                pos = 0;
                output_low(LED);
                setup_timer_0(t0_off);
                disable_interrupts(global);
                disable_interrupts(int_RTCC);   
            }
         }   
         
         if(scan_keys() == 2)           
         {                       
             k += 1;
             pos = 0;
             if(k > 3)
             {                                     
                 k = 0;
             }                            
             draw_background();   
         }     
         
         plot_data(id, k);      
    }                                 
}  
                                                    
                                      
void setup()                                   
{                              
   disable_interrupts(global); 
   setup_spi(spi_disabled);          
   setup_timer_0(t0_off);    
   setup_timer_1(t1_disabled);  
   setup_timer_2(t2_disabled, 255, 1); 
   setup_ccp1(ccp_off);            
   setup_ccp2(ccp_off);
   set_RTCC(reload);                        
   set_timer1(0);                
   set_timer2(0);                                                    
   setup_adc(adc_clock_div_16);     
   setup_adc_ports(AN0_AN1_AN3);   
   disable_interrupts(global);
   disable_interrupts(int_RTCC);   
   glcd_init(ON);                                  
   delay_ms(100);       
}                            


unsigned char scan_keys()
{
     if(set)
     {
          generate_tone(2000, 25);
          return 1;              
     }  
     else if(inc)
     {
          generate_tone(1800, 25);     
          return 2;  
     }  
     else if(dec)
     {                    
          generate_tone(1200, 25);
          return 3;   
     }   
     else if(start)
     {                    
          generate_tone(1600, 25);
          return 4;   
     }
     else 
     {
          return 0;         
     }
}              
                                                       

void intro()                           
{                                                                      
     unsigned char text1[] = {"MicroArena"};
     unsigned char text2[] = {"Graphical Solar"};           
     unsigned char text3[] = {"Logging System"};      
     clear_screen_1();
     glcd_text57(22, 4 , text2, 1, OFF);                
     glcd_text57(25, 14 , text3, 1, OFF);        
     delay_ms(1500);  
     glcd_text57(36, 44 , text1, 1, OFF);                                           
     delay_ms(1500);               
     clear_screen_2();
     memset(V, 0x0000, sizeof(V));
     memset(I, 0x0000, sizeof(I));
     memset(T, 0x0000, sizeof(T));
} 


void draw_background()           
{                            
   register unsigned long n = 0;                             
   unsigned char i = 0;                    
   unsigned char j = 0;
   short cs = 0;    
                                                 
   for(i = 0; i < 8; ++i)       
   {
      output_low(GLCD_DI);                      
      glcd_writeByte(GLCD_LEFT, 0x40);   
      glcd_writeByte(GLCD_RIGHT, 0x40);
      glcd_writeByte(GLCD_LEFT, i | 0xB8);                                                              
      glcd_writeByte(GLCD_RIGHT, i | 0xB8);
      output_high(GLCD_DI);         
      for(j = 0; j < 128; ++j)
      {
         if(j < 64) 
         {
            cs = GLCD_LEFT;
         }
         else 
         {
            cs = GLCD_RIGHT;                    
         }                                           
         glcd_writeByte(cs, image[n]);  
         n += 1;
      }                     
   }
}                   


float map(float v, float x_min, float x_max, float y_min, float y_max)  
{                                                                     
    register float m = 0.0;
    m = ((y_max - y_min) / (x_max - x_min));
    return (y_min + (m * (v - x_min)));
}


float constrain(float value, float max, double min)
{
     if(value>max)                       
     {
         value=max;
     } 
     if(value<min)
     {
         value=min;
     }  
     return value;    
}
                     
                             
unsigned long adc_rms(unsigned char channel)   
{
    register float rms = 0.0;
    register unsigned long long tmp = 0;
    unsigned char samples = 0; 
    set_adc_channel(channel);
    while(samples < 8)        
    {
        read_ADC(ADC_start_only);
        while(!ADC_done());    
        rms = read_ADC(ADC_read_only); 
        tmp += (rms * rms);
        samples += 1;                                         
    }                
    tmp >>= 3;  
    rms = sqrt(tmp);
    return rms;                        
}                                                      

                                       
void read_parameters()
{
     if(logging)
     {
        PV_V = adc_rms(0);
        delay_ms(10);
        output_high(load);
        PV_I = adc_rms(1);
        delay_ms(10);
        output_low(load);
        PV_T = adc_rms(3); 
        delay_ms(10);
     }
     if(log_done)
     {
         V[x] = PV_V;
         I[x] = PV_I;
         T[x] = PV_T; 
         x += 1;   
         log_done = 0;
     } 
     if(logging)
     {
        PV_V = map(PV_V, 0, V_cnt, 0, V_lim); 
        PV_I = map(PV_I, 0, I_cnt, 0, I_lim);  
        PV_T = map(PV_T, 0, 1023, 0, 511.5);  
        P = (PV_V * PV_I);
     }
}   

                       
void compare_values()
{
   if(done_setting)                            
   {                 
     P_max = 0;                  
     P_min = 20;
     V_max = 0;    
     V_min = 30;           
     I_max = 0;    
     I_min = 2;
     T_max = 0;    
     T_min = 70;  
     done_setting = 0;  
   }                                
   else                          
   {                   
     if(P > P_max)
     {        
          P_max = P;                   
     }                                                  
     if(P < P_min)
     {              
          P_min = P;
     }           
     if(PV_V > V_max)
     {           
          V_max = PV_V;        
     }        
     if(PV_V < V_min)
     {           
          V_min = PV_V; 
     } 
     if(PV_I > I_max)
     {               
          I_max = PV_I;        
     }                      
     if(PV_I < I_min)    
     {
          I_min = PV_I; 
     }                            
     if(PV_T > T_max)
     {                      
          T_max = PV_T;        
     }          
     if(PV_T < T_min)    
     {
          T_min = PV_T;            
     }    
   }   
}         


void clear_screen_1()
{                                               
     unsigned char i = 0;             
     for(i = 0; i < 64; i++)
     {                                         
          glcd_line((64 + i), 0, (64 + i), 63, YES);      
          glcd_line((63 - i), 0, (63 - i), 63, YES); 
          delay_ms(2);  
     }       
}                              
                                                
                                         
void clear_screen_2()          
{                                                
     unsigned char i = 0;    
     for(i = 0; i < 32; i++)
     {
          glcd_line(0, (32 + i), 127, (32 + i), NO); 
          glcd_line(0, (31 - i), 127, (31 - i), NO); 
          delay_ms(2);  
     }                     
     glcd_fillScreen(0);  
} 


signed char change_value(unsigned char x, unsigned char y, signed char value, signed char low, signed char high)  
{                                                                                                 
    while(TRUE)
    {
        sprintf(text, "%02u", value);             
        glcd_text57(x, y , text, 1, OFF);             
        delay_ms(100);                                                 
        
        if(scan_keys() == 2)
        {                                                                   
            value += 1;
        }     
        
        if(value>high)
        {
            value = low;
        } 
        
        if(scan_keys() == 3)            
        {
            value -= 1;
        } 
        
        if(value < low)
        {
            value = high;
        }                     
                 
        sprintf(text, "%02u", value);                                                                                                                                             
        glcd_text57(x, y , text, 1, ON);     
        delay_ms(100);   
                                      
        if(scan_keys()==1)                                
        {
            while(set);
            return value;            
        }
    };
}  

                                  
float change_value_f(unsigned char x, unsigned char y, float value, float low, float high)    
{                                                                                                 
    while(TRUE)
    {
        sprintf(text, "%02.1f", value);             
        glcd_text57(x, y , text, 1, OFF);             
        delay_ms(100);                                                 
                    
        if(scan_keys() == 2)
        {                                                                   
            value += 0.1;
            delay_ms(20);
        }   
        
        if(scan_keys() == 3)            
        {
            value -= 0.1;
            delay_ms(20);
        } 
        
        if(value > high)
        {
            value = low;     
        }
        
        if(value < low)
        {
            value = high;
        }                     
                 
        sprintf(text, "%02.1f", value);             
        glcd_text57(x, y , text, 1, ON);     
        delay_ms(100);   
                                      
        if(scan_keys()==1)                                
        {
            delay_ms(20);             
            while(set);
            return value;            
        }
    };
}                                                     
   

void settings()               
{                                    
    unsigned char text1[] = {"Interval"};  
    unsigned char text2[] = {"  :  :  "};  
    unsigned char text3[] = {"P Max/W"}; 
    unsigned char text4[] = {"I Max/A"};   
    unsigned char text5[] = {"V Max/V"};                
    unsigned char text6[] = {"Settings Menu"}; 
    
    PV_P = 0.5;
    PV_I = 0.1;
    PV_V = 5;   
    PV_T = 25;  
    
    output_low(LED);
    disable_interrupts(global);
    disable_interrupts(int_RTCC);   
    setup_timer_0(t0_off);   
    
    clear_screen_2();  
    glcd_text57(20, 1 , text6, 1, ON);  
    glcd_text57(5, 20 , text1, 1, ON);                
    glcd_text57(60, 20 , text2, 1, ON); 
    glcd_text57(5, 30 , text3, 1, ON);
    glcd_text57(5, 40 , text4, 1, ON);
    glcd_text57(5, 50 , text5, 1, ON);
    setup_timer_0(t0_off);                  
    disable_interrupts(global);         
    disable_interrupts(int_RTCC); 
    s = change_value(95, 20, s, 0, 59);  
    s_t = s;          
    min = change_value(77, 20, min, 0, 59);
    min_t = min;
    hr = change_value(60, 20, hr, 0, 23);      
    hr_t = hr; 
    PV_P = change_value_f(82, 30, PV_P, 0, 10);                      
    I_lim = change_value_f(88, 40, I_lim, 0, 1); 
    V_lim = change_value_f(82, 50, V_lim, 0, 22);    
    clear_screen_1();
    done_setting = 1;
    draw_background(); 
}
                             
                                                           
void set_parameters()  
{
   if(scan_keys()==1)
   {
        delay_ms(100);       
        if(set)             
        {
            settings();
        }
   }                                  
}
                       
                           
void general_display()  
{                                             
    glcd_text35(1, 1 , txt1, OFF);    
    sprintf(txt1, "V:%2.1f ", PV_V);              
    glcd_text35(1, 1 , txt1, ON);   
    
    glcd_text35(1, 9 , txt4, OFF);                          
    sprintf(txt4, "x:%2.1f ", V_max);              
    glcd_text35(1, 9 , txt4, ON); 
    
    glcd_text35(1, 17 , txt7, OFF);  
    sprintf(txt7, "m:%2.1f ", V_min);              
    glcd_text35(1, 17 , txt7, ON);   
    
    glcd_text35(35, 1 , txt2, OFF);  
    sprintf(txt2, "I:%1.2f ", PV_I);                     
    glcd_text35(35, 1 , txt2, ON);  
    
    glcd_text35(35, 9 , txt5, OFF);                     
    sprintf(txt5, "x:%1.2f ", I_max);              
    glcd_text35(35, 9 , txt5, ON); 
      
    glcd_text35(35, 17 , txt8, OFF);                     
    sprintf(txt8, "m:%1.2f ", I_min);              
    glcd_text35(35, 17 , txt8, ON);            

    glcd_text35(68, 1 , txt3, OFF);                               
    sprintf(txt3, "P:%2.1f ", P);              
    glcd_text35(68, 1 , txt3, ON);                                 
                                                                                       
    glcd_text35(68, 9 , txt6, OFF);                               
    sprintf(txt6, "x:%2.1f ", P_max);              
    glcd_text35(68, 9 , txt6, ON);       

    glcd_text35(68, 17 , txt9, OFF);                               
    sprintf(txt9, "m:%2.1f ", P_min);              
    glcd_text35(68, 17 , txt9, ON);      
    
    
    glcd_text35(99, 1 , txt10, OFF);                               
    sprintf(txt10, "T:%2.1f ", PV_T);              
    glcd_text35(99, 1 , txt10, ON);                                 
                                                                                       
    glcd_text35(99, 9 , txt11, OFF);                               
    sprintf(txt11, "x:%2.1f ", T_max);              
    glcd_text35(99, 9 , txt11, ON);       
                                        
    glcd_text35(99, 17 , txt12, OFF);                               
    sprintf(txt12, "m:%2.1f ", T_min);              
    glcd_text35(99, 17 , txt12, ON);                                    
}                

                                          
void plot_data(short type, unsigned char parameter) 
{                                         
    unsigned char text1[] = {"V"}; 
    unsigned char text2[] = {"I"}; 
    unsigned char text3[] = {"P"};           
    unsigned char text4[] = {"T"};
    float temp_p = 0.0;
    float temp_v = 0.0;
    float temp_i = 0.0;
    
    switch(parameter)
    {
          case 1:                            
          {
               switch(type)
               {
                  case 0:
                  {
                      l = map(PV_V, 0, V_lim, 0, 25);
                      break;
                  }
                  default:
                  {
                      temp_v = V[pos];
                      temp_v = map(temp_v, 0, V_cnt, 0, V_lim);
                      l = map(temp_v, 0, V_lim, 0, 25);
                      break;
                  }
               }
               
               glcd_text57(2, 43 , text2, 1, OFF); 
               glcd_text57(2, 43 , text3, 1, OFF); 
               glcd_text57(2, 43 , text4, 1, OFF);  
               glcd_text57(2, 43 , text1, 1, ON);         
               break;       
          }                                                     
          case 2:
          {                         
               switch(type)
               {
                  case 0:
                  {
                      l = map(PV_I, 0, I_lim, 0, 25);
                      break;
                  }
                  default:
                  {
                      temp_i = I[pos];
                      temp_i = map(temp_i, 0, I_cnt, 0, I_lim);
                      l = map(temp_i, 0, I_lim, 0, 25);
                      break;
                  }
               }
               
               glcd_text57(2, 43 , text1, 1, OFF); 
               glcd_text57(2, 43 , text3, 1, OFF); 
               glcd_text57(2, 43 , text4, 1, OFF);  
               glcd_text57(2, 43 , text2, 1, ON); 
               break;                               
          }
          case 3:        
          {                  
               switch(type)
               {
                  case 0:
                  {
                      l = map(PV_T, 0, T_lim, 0, 25);
                      break;
                  }
                  default:
                  {
                      temp_p = T[pos];
                      temp_p = map(temp_p, 0, 1023, 0, 511.5);
                      l = map(temp_p, 0, T_lim, 0, 25);
                      break;
                  }
               }
               
               glcd_text57(2, 43 , text2, 1, OFF); 
               glcd_text57(2, 43 , text3, 1, OFF); 
               glcd_text57(2, 43 , text1, 1, OFF);   
               glcd_text57(2, 43 , text4, 1, ON);  
               break;       
          }    
          default:
          {                                              
               l = map(P, 0, PV_P, 0, 25); 
               
               switch(type)
               {
                  case 0:
                  {
                      l = map(P, 0, PV_P, 0, 25);
                      break;
                  }
                  default:
                  {
                      temp_v = V[pos];
                      temp_i = I[pos];
                      temp_v = map(temp_v, 0, V_cnt, 0, V_lim);
                      temp_i = map(temp_i, 0, I_cnt, 0, I_lim);
                      temp_p = (temp_v * temp_i);
                      l = map(temp_p, 0, PV_P, 0, 25);
                      break;
                  }
               }
               
               glcd_text57(2, 43 , text2, 1, OFF); 
               glcd_text57(2, 43 , text1, 1, OFF); 
               glcd_text57(2, 43 , text4, 1, OFF);    
               glcd_text57(2, 43 , text3, 1, ON); 
               break;           
          }          
    }  
    l = constrain(l, 25, 0);    
                                                    
    switch(type)                    
    {                                        
          case 0:                   
          {      
               glcd_line((12 + x), 54, (12 + x), 29, OFF);              
               glcd_line((12 + x), 54, (12 + x), (54 - l), ON);              
               break;        
          }            
          default:
          {
               glcd_line((12 + pos), 54, (12 + pos), 29, OFF);              
               glcd_line((12 + pos), 54, (12 + pos), (54 - l), ON); 
               
               pos += 1;                     
               if(pos > 99)
               {
                   pos = 0;                 
               }
               
               break;                    
          }                          
    }                      
    
     
}   
                 
                                   
