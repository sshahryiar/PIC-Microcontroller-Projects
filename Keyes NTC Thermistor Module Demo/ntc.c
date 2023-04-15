#include <16F877A.h>   


#device *= 16      
#device ADC = 10
                                                                                     
                             
#fuses HS, PUT, PROTECT, CPD, NODEBUG, NOLVP, NOWDT, NOWRT, BROWNOUT 


#use delay (clock = 10MHz)  

                 
#include "lcd.c"  
#include <math.h>

                                                                                           
#define fixed_R                      10000.0
#define Beta                          3950.0 
#define ADC_Max                       1023.0
#define Ro                          0.007569     
                                                                                                    

                                                 
const unsigned char symbols[16]=                                                             
{                                          
    0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0E, 0x11, 0x11, 0x0A, 0x11, 0x00                            
};                                      
             
                            
void setup(void);  
void lcd_symbol(void);
unsigned long ADC_avg(void);


void main(void)                                      
{                   
    float t = 0; 
    float R = 0;                                                                                                   
    float RT = 0;   
    
    setup();
    
    while(TRUE)
    {                                  
      t = adc_avg();   
      
      RT = ((ADC_Max / t) - 1);
      r = (fixed_R / RT);     
      
      RT = (Beta / (log (r / Ro)));
      t = (RT - 273.15);
      
                                             
      lcd_gotoxy(1, 1);
      lcd_putc("T/ C:");                  
      lcd_gotoxy(3, 1);                 
      lcd_putc(0);
      lcd_gotoxy(13, 1);
      printf(lcd_putc, "%3.1g ", t);                                                
      lcd_gotoxy(1, 2);
      lcd_putc("R/k :");
      lcd_gotoxy(4, 2);               
      lcd_putc(1);             
      lcd_gotoxy(11, 2);
      printf(lcd_putc, "%5.1g ", r);
      delay_ms(100); 
   };
}


void setup(void)                            
{       
   disable_interrupts(GLOBAL);
   setup_comparator(NC_NC_NC_NC); 
   setup_SPI(SPI_disabled | SPI_SS_disabled);
   setup_ADC(ADC_clock_div_4);
   setup_ADC_ports(AN0);
   set_ADC_channel(0);
   setup_timer_0(T0_internal);
   setup_timer_1(T1_disabled);
   setup_timer_2(T2_disabled, 255, 1);
   set_timer0(0);     
   set_timer1(0);
   set_timer2(0);
   setup_CCP1(CCP_off);
   setup_CCP2(CCP_off);  
   lcd_init();
   lcd_symbol();                     
   delay_ms(100);     
}  


void lcd_symbol(void)  
{
   unsigned char n = 0;  
 
   lcd_send_byte(0, 0x40); 
   for(n = 0; n < 16; n++)                                       
   {
        lcd_send_byte(1, symbols[n]);
   }
   lcd_send_byte(0, 0x80);                      
} 
     
                                          
unsigned long ADC_avg(void) 
{
   unsigned char samples = 16;
   
   unsigned long avg = 0;
   
   while(samples > 0)
   {
      read_ADC(ADC_start_only);
      while(ADC_done() == FALSE);
      avg += read_ADC(ADC_read_only);
      
      samples--;
   };
                                            
   avg >>= 4;
               
   return avg;
}   


                                                           
