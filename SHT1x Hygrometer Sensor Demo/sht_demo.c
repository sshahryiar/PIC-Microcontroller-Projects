#include <16F690.h>


#device *= 16
#device ADC = 10


#fuses NOWDT, INTRC_IO, PROTECT, PUT, CPD
#fuses NOBROWNOUT, NOMCLR, NOIESO, FCMEN
    
                                                      
#use delay (internal = 8MHz)
                              

#include "lcd.c" 
#include "lcd_print.c"
#include "SHT1x.c"   



unsigned char data_value;  
                                                                                         

void setup();                          


void main()   
{                
   float T = 0.0;
   float RH = 0.0; 
   
   unsigned char txt1[9] = {"R.H./ %:"};       
   unsigned char txt2[9] = {"Temp/ C:"};    
                       
   setup();                                                                                                          
     
   LCD_goto(0, 0);                 
   LCD_putstr(txt1);
   LCD_goto(0, 1);
   LCD_putstr(txt2);    
   print_symbol(5, 1, 0);    
                                                                                            
   while(TRUE)
   {                          
                                                           
      SHT1x_get_data(T, RH);                          
             
      print_F(10, 0, RH, 2);
      print_F(10, 1, T, 2);                 
      
      delay_ms(100);         
   };
}                                                                                                                                                     
                      
                                                                                                                
void setup()
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
   SHT1x_init(); 
} 


