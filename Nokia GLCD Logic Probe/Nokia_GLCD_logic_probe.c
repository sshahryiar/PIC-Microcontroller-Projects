#include <18F242.h> 


#device *= 16 
#device ADC = 10
           
                
#fuses HS, NOWDT, PUT, PROTECT, NOSTVREN
#fuses NOCPB, NOWRT, NOWRTB NOEBTRB, NOEBTR
#fuses BROWNOUT, CPD, NOOSCSEN, BORV20
#fuses NOLVP, NODEBUG, NOWRTC, NOWRTD           
 
                             
#use delay(clock = 10MHz)                   
                              
                                                  
#byte lcdport         = 0x07   
#byte lcdport_tris    = 0x87         
    
                                     
#include "3310_custom.c" 
#include "background_art.c" 


#define v_limit         21.77 
                                    

register unsigned long previous_v = 0;
register unsigned long present_v = 0;
register float volts = 0;  
float high = 0;
float low = 0;
float supply = 0;          
                               
                

void setup();
void draw_background();
void clear_zone();
unsigned long adc_avg();
float map(float v, float x_min, float x_max, float y_min, float y_max);
void check_range();    
void draw_bars(unsigned char value); 


void main()                                       
{ 
   unsigned char bars = 0;
   setup();
   draw_background(); 
   check_range();        
   while(TRUE)
   {          
        present_v = adc_avg();
        if(present_v != previous_v)                      
        {                                                  
            clear_zone();
            volts = map(present_v, 0, 1023, 0, v_limit); 
            if(((volts >= high) && (volts <= supply)))
            {     
                  LcdPixel(78, 24, 1); 
                  LcdPixel(79, 23, 1); 
                  LcdPixel(79, 25, 1); 
                  LcdPixel(80, 22, 1); 
                  LcdPixel(80, 26, 1); 
            }                                       
            else if(((volts >= 0) && (volts <= low)))
            {        
                  LcdPixel(78, 42, 1);
                  LcdPixel(79, 41, 1); 
                  LcdPixel(79, 43, 1); 
                  LcdPixel(80, 40, 1); 
                  LcdPixel(80, 44, 1);       
            }                             
            else    
            {       
                  LcdPixel(78, 33, 1); 
                  LcdPixel(79, 32, 1); 
                  LcdPixel(79, 34, 1); 
                  LcdPixel(80, 31, 1); 
                  LcdPixel(80, 35, 1); 
            }
            bars = map(volts, 0, supply, 0, 79);   
            nokia_gotoxy(2, 4);
            printf(nokia_printchar,"%2.2gV ", supply);    
            nokia_gotoxy(2, 5);            
            printf(nokia_printchar,"%2.2gV ",volts);  
            draw_bars(bars);        
            delay_ms(60);
            previous_v = present_v;  
        }                   
   }                           
}   

                               
void setup()                                      
{
   disable_interrupts(global);
   setup_WDT(WDT_off);              
   setup_adc_ports(AN0_AN1_AN2_AN4_VSS_VREF);//AN0_AN1_AN3
   setup_adc(ADC_clock_div_32);   
   set_ADC_channel(0);
   setup_timer_0(T0_OFF);   
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,255,1);                 
   setup_timer_3(T3_DISABLED);
   setup_CCP1(CCP_off);
   setup_CCP2(CCP_off);
   setup_SPI(SPI_DISABLED);
   lcdport_tris=0x00;      
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
   register unsigned long i = 0;
   nokia_clean_ddram();
   nokia_clear_screen();
   nokia_gotoxy(0,0); 
   for(i = 0; i < 504; i++)
   {
        nokia_write_data(background[i]);
   }
   delay_ms(100);          
}  


void clear_zone()
{                             
         unsigned char x_pos = 0;   
         unsigned char y_pos = 0; 
         for(x_pos = 77; x_pos < 84; x_pos++)
         {
                  for(y_pos = 22; y_pos < 48; y_pos++) 
                  {
                           lcdPixel(x_pos, y_pos, 0);
                  }
         }
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
    return tmp;                 
}  


float map(float v, float x_min, float x_max, float y_min, float y_max)    
{
    float m = 0.0;
    m = ((y_max - y_min)/(x_max - x_min));
    return (y_min + (m * (v - x_min)));            
}

                
void check_range() 
{                   
         supply = adc_avg();
         supply = map(supply, 0, 1023, 0, v_limit);  
         if((supply < 3.9) || (supply > 5.3))
         {
                  LcdPixel(39, 22, 1); 
                  LcdPixel(40, 23, 1); 
                  LcdPixel(41, 24, 1); 
                  LcdPixel(40, 25, 1); 
                  LcdPixel(39, 26, 1); 
                  high = (0.67 * supply);
                  low = (0.33 * supply);  
         }                            
         else                               
         {                                   
                  LcdPixel(16, 22, 1); 
                  LcdPixel(17, 23, 1); 
                  LcdPixel(18, 24, 1); 
                  LcdPixel(17, 25, 1); 
                  LcdPixel(16, 26, 1);
                  high = 2.8;
                  low = 0.8;  
         }                               
}                                      


void draw_bars(unsigned char value)
{                                     
         unsigned char x = 0;
         for(x = value; x < 79; x += 2)
         {                          
                  drawLine(x, 14, x, 15, 0);         
         }  
         for(x = 4; x < value; x += 2)
         {                              
                  drawLine(x, 14, x, 15, 1);         
         } 
}                                               
