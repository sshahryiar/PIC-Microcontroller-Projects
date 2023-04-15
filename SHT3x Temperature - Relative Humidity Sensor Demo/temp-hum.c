#include <16F690.h>


#device *= 16
#device ADC = 10


#fuses NOWDT, INTRC_IO, PROTECT, PUT, CPD
#fuses NOBROWNOUT, NOMCLR, NOIESO, FCMEN
    
                                                      
#use delay (internal = 8MHz)
#use I2C(MASTER, SDA = PIN_B4, SCL = PIN_B5, fast = 100000)
                              

#include "lcd.c" 
#include "lcd_print.c"
#include "SHT3x.c"    
                                                                                         

void setup(void);                          


void main(void)   
{                  
   unsigned char txt1[9] = {"R.H./ %:"};       
   unsigned char txt2[9] = {"Temp/ C:"};    
       
   float T = 0.0;    
   float RH = 0.0;                   
                      
   setup();                                                                                                          
     
   LCD_goto(0, 0);                 
   LCD_putstr(txt1);
   LCD_goto(0, 1);
   LCD_putstr(txt2);    
   print_symbol(5, 1, 0);    
                                                                                            
   while(TRUE)
   {                                                 
      SHT3x_read_temp_humidity(&RH, &T);  
      
      print_F(10, 0, RH, 2);
      print_F(10, 1, T, 2);                 
      
      delay_ms(1000);         
   };
}                                                                                                                                                     
                      
                                                                                                                
void setup(void)
{                                                
   disable_interrupts(GLOBAL);                           
   setup_WDT(WDT_off);
   setup_oscillator(OSC_8MHz);
   setup_comparator(NC_NC_NC_NC);                               
   setup_ADC(ADC_off);
   setup_ADC_ports(no_analogs);                                    
   setup_CCP1(CCP_off);                                         
   setup_timer_0(T0_internal);        
   setup_timer_1(T1_disabled);      
   setup_timer_2(T2_disabled, 255, 1);
   set_timer0(0);                                                  
   set_timer1(0);                                              
   set_timer2(0);    
   lcd_init();
   lcd_clear_home();
   load_custom_symbol();   
   SHT3x_init(); 
} 



