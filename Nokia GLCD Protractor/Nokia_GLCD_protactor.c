#include <18F242.h> 


#device *= 16 
#device ADC = 10
                                         
                
#fuses HS, NOWDT, PUT, PROTECT, NOSTVREN
#fuses NOCPB, NOWRT, NOWRTB NOEBTRB, NOEBTR
#fuses BROWNOUT, CPD, NOOSCSEN, BORV20
#fuses NOLVP, NODEBUG, NOWRTC, NOWRTD
 
                             
#use delay(clock = 10MHz)   


#define button  input(pin_C6) 

#define L_limit          258
#define H_limit          775     
                               
                                                  
#byte lcdport         = 0x07         
#byte lcdport_tris    = 0x87         
                                          
                                  
#include "3310_custom.c"         
#include "background_art.c" 
#include <math.h>         
                                       

void setup();
void draw_background();
unsigned long adc_avg();
float map(float v, float x_min, float x_max, float y_min, float y_max);
float constrain(float value, float value_min, float value_max);


void main()                                                   
{                                                 
   register float present_pos = 0.0;
   register float previous_pos = 0.0;  
   register float angle = 0;
   setup();                                               
   draw_background();
   while(TRUE)                                              
   {                               
         angle = adc_avg();
         present_pos = map(angle, L_limit, H_limit, 0, 3.142);   
         angle = map(angle, L_limit, H_limit, 0, 180); 
         if(present_pos != previous_pos)   
         {                                      
                  drawLine(40, 31, (40 - (15 * cos(previous_pos))), (31 - (15 * sin(previous_pos))), 0); 
                  drawLine(40, 31, (40 - (15 * cos(present_pos))), (31 - (15 * sin(present_pos))), 1);   
                  drawCircle(40, 31, 1, 1);  
                  nokia_gotoxy(1, 5);
                  printf(nokia_printchar,"Angle: %3.2g ", angle);  
                  previous_pos = present_pos;
         }                   
         delay_ms(100);                       
   }                                               
}   

                                                 
void setup() 
{
   disable_interrupts(global);
   setup_WDT(WDT_off);
   setup_adc_ports(AN0);
   setup_adc(ADC_clock_div_16);
   set_ADC_channel(0);
   setup_timer_0(T0_OFF);      
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED, 255, 1);
   setup_timer_3(T3_DISABLED);
   setup_CCP1(CCP_off);
   setup_CCP2(CCP_off);
   setup_SPI(SPI_DISABLED);
   lcdport_tris = 0x00;      
   nokia_init();
   set_timer0(0);                                                                                       
   set_timer1(0);                   
   set_timer2(0);
   set_timer3(0); 
   nokia_clean_ddram();
   nokia_clear_screen(); 
   lcd_update();           
   delay_ms(100);
}                                   


void draw_background()
{
   register unsigned int16 s = 0;
   nokia_clean_ddram();
   nokia_clear_screen();
   nokia_gotoxy(0,0); 
   for(s = 0; s < 504; s++)
   {
        nokia_write_data(background[s]);
   }                          
   delay_ms(20);
}


unsigned long adc_avg()
{
    register unsigned long tmp = 0;
    unsigned char s = 0;
    while(s < 64)
    {
        read_adc(adc_start_only);
        while(!adc_done());
        tmp += read_adc(adc_read_only);
        s++;
    }
    tmp >>= 6;  
    tmp = constrain(tmp, L_limit, H_limit); 
    return tmp;        
}  


float map(float v, float x_min, float x_max, float y_min, float y_max)   
{
    register float m = 0.0;
    m = ((y_max - y_min)/(x_max - x_min));
    return (y_min + (m * (v - x_min)));
}       


float constrain(float value, float value_min, float value_max)   
{
      if(value >= value_max)
      {
           return value_max;
      }
      else if(value <= value_min)
      {
           return value_min;
      }        
      else
      {
           return value;
      }
}                                  
