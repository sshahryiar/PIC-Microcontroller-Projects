#include <16F876A.h>


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
                                           
                                      
#use delay (clock = 10MHz)                   
                

#include "HDM64GS12.c"            
#include "graphics.c"        
#include "background.c" 


#define CAL    input(pin_A2)


void setup();
void draw_background();    
float constrain(float value, float max, float min);  
float map(float v, float x_min, float x_max, float y_min, float y_max);
void adc_avg(unsigned long *x_dir, unsigned long *y_dir); 
void clear_screen_1();                                
void clear_screen_2(); 
                                                                   
                          
void main()
{
    unsigned long x_old = 0;
    unsigned long y_old = 0;
    unsigned long x_axis = 0;
    unsigned long y_axis = 0; 
                                                    
    unsigned long raw_x_max = 590;
    unsigned long raw_x_min = 370;
    unsigned long raw_y_max = 590;
    unsigned long raw_y_min = 370;
    
    unsigned char xn = 0;
    unsigned char xp = 0;         
    unsigned char yn = 0;
    unsigned char yp = 0;   
    
    unsigned char xnc = 0;
    unsigned char xpc = 0;         
    unsigned char ync = 0;
    unsigned char ypc = 0;    
                          
    unsigned char txt0[9]; 
    unsigned char txt1[9];
    unsigned char txt2[9];               
                               
    unsigned char text1[] = {"Calibration"};  
    unsigned char text2[] = {"Center to Right:"};     
    unsigned char text3[] = {"Center to Left :"};        
    unsigned char text4[] = {"Center to Up   :"};     
    unsigned char text5[] = {"Center to Down :"};    
    unsigned char text6[] = {"Calibration Done."};   
                                                      
    memset(txt1, 0, sizeof(txt1));
    memset(txt2, 0, sizeof(txt2));    
    
    setup();
    draw_background();                               
    
    while(TRUE)                    
    {                                              
       if(CAL)
       {    
           clear_screen_1();                 
           glcd_text57(32, 1 , text1, 1, 1);      
           delay_ms(2000);                        
           
           glcd_text57(4, 14 , text2, 1, 1);  
           delay_ms(4000);    
           adc_avg(&x_axis, &y_axis); 
           raw_x_max = x_axis;
           sprintf(txt0, "%4lu", raw_x_max);                 
           glcd_text57(100, 14 , txt0, 1, 1);  
  
           glcd_text57(4, 24 , text3, 1, 1); 
           delay_ms(4000);    
           adc_avg(&x_axis, &y_axis); 
           raw_x_min = x_axis;
           sprintf(txt0, "%4lu", raw_x_min);                 
           glcd_text57(100, 24 , txt0, 1, 1);    
           
           glcd_text57(4, 34 , text4, 1, 1); 
           delay_ms(4000);    
           adc_avg(&x_axis, &y_axis); 
           raw_y_max = y_axis;
           sprintf(txt0, "%4lu", raw_y_max);                 
           glcd_text57(100, 34 , txt0, 1, 1); 
           
           glcd_text57(4, 44 , text5, 1, 1);    
           delay_ms(4000);    
           adc_avg(&x_axis, &y_axis); 
           raw_y_min = y_axis;
           sprintf(txt0, "%4lu", raw_y_min);                 
           glcd_text57(100, 44 , txt0, 1, 1);  
           
           glcd_text57(16, 56 , text6, 1, 1);      
           delay_ms(4000);   
                               
           clear_screen_2();    
           draw_background();
       }
          
       else                         
       {
          adc_avg(&x_axis, &y_axis);    
             
          if((x_old != x_axis) || (y_old != y_axis))     
          {                             
              xp = map(x_old, raw_x_min, raw_x_max, 22, 120);   
              xn = map(x_axis, raw_x_min, raw_x_max, 22, 120);   
              
              yp = map(y_old, raw_y_min, raw_y_max, 6, 57);   
              yn = map(y_axis, raw_y_min, raw_y_max, 6, 57);     
              
              xpc = map(x_old, raw_x_min, raw_x_max, 22, 58);   
              xnc = map(x_axis, raw_x_min, raw_x_max, 22, 58);   
              
              ypc = map(y_old, raw_y_min, raw_y_max, 21, 57);   
              ync = map(y_axis, raw_y_min, raw_y_max, 21, 57);   
                                         
              glcd_circle(xpc, ypc, 3, 1, 1);          
              glcd_circle(xnc, ync, 3, 1, 0);      
                            
              glcd_circle(7, yp, 3, 1, 1);
              glcd_circle(7, yn, 3, 1, 0);    
                          
              glcd_circle(xp, 7, 3, 1, 1);
              glcd_circle(xn, 7, 3, 1, 0); 
              
              sprintf(txt1, "X:%4lu", x_old);                 
              glcd_text57(74, 30 , txt1, 1, 1);      
              sprintf(txt1, "X:%4lu", x_axis);             
              glcd_text57(74, 30 , txt1, 1, 0);           
               
              sprintf(txt2, "Y:%4lu", y_old);             
              glcd_text57(74, 40 , txt2, 1, 1);      
              sprintf(txt2, "Y:%4lu", y_axis);             
              glcd_text57(74, 40 , txt2, 1, 0);   
                                                   
              glcd_circle(40, 39, 8, 0, 0);  
              glcd_circle(40, 39, 21, 0, 0); 
              glcd_circle(40, 39, 22, 0, 0); 
              glcd_circle(40, 39, 23, 0, 0); 
              
              x_old = x_axis;
              y_old = y_axis;                                         
          }   
       }
    };                
}                                       


void setup()                                   
{                              
   disable_interrupts(global); 
   setup_spi(spi_disabled);          
   setup_timer_0(0 | 0 | 0);               
   setup_timer_1(t1_disabled);                     
   setup_timer_2(t2_disabled, 255, 1); 
   setup_ccp1(ccp_off);            
   setup_ccp2(ccp_off);
   set_timer0(0);                        
   set_timer1(0);                
   set_timer2(0);       
   setup_comparator(NC_NC_NC_NC);
   setup_adc(adc_clock_div_16);     
   setup_adc_ports(AN0_AN1_VSS_VREF);   
   glcd_init(ON);                                  
   delay_ms(100);       
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


float constrain(float value, float max, double min)
{
     if(value > max)                       
     {
         value = max;
     } 
     else if(value < min)
     {
         value = min;
     }  
     return value;                       
}  


float map(float v, float x_min, float x_max, float y_min, float y_max)  
{                                                                     
    register float m = 0.0;
    m = ((y_max - y_min) / (x_max - x_min));
    m = (y_min + (m * (v - x_min)));  
    m = constrain(m, y_max, y_min);
    return m;
}      
                                    

void adc_avg(unsigned long *x_dir, unsigned long *y_dir) 
{
   unsigned char samples = 16;
   unsigned long avg1 = 0;     
   unsigned long avg2 = 0;
      
   while(samples > 0)
   {
       set_adc_channel(0);
       delay_ms(1);
       read_ADC(ADC_start_only);
       while(!ADC_done());
       avg1 += read_ADC(ADC_read_only);     
       
       set_adc_channel(1);
       delay_ms(1);
       read_ADC(ADC_start_only);
       while(!ADC_done());
       avg2 += read_ADC(ADC_read_only);  
       
       samples -= 1;
   }                
    
   *x_dir = (avg1 >> 4);   
   *y_dir = (avg2 >> 4);        
}   


void clear_screen_1()
{                                               
     unsigned char i = 0;             
     for(i = 0; i < 64; i++)
     {                                         
          glcd_line((64 + i), 0, (64 + i), 63, 0);      
          glcd_line((63 - i), 0, (63 - i), 63, 0); 
          delay_ms(1);  
     }       
}                              
                                                
                                         
void clear_screen_2()          
{                                                
     unsigned char i = 0;    
     for(i = 0; i < 32; i++)
     {
          glcd_line(0, (32 + i), 127, (32 + i), 0); 
          glcd_line(0, (31 - i), 127, (31 - i), 0); 
          delay_ms(1);  
     }                 
}  



