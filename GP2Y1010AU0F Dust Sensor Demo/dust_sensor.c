#include <16F690.h>


#device *= 16
#device ADC = 10


#fuses NOWDT, INTRC_IO, PROTECT, PUT, CPD
#fuses NOBROWNOUT, NOMCLR, NOIESO, FCMEN
    
                                                      
#use delay (internal = 8MHz)
                              

#include "lcd.c" 
#include "lcd_print.c"
#include "GP2Y1010AU0F.c"
                                                                                        

void setup(void);  


void main(void)   
{                  
   unsigned char txt1[17] = {"Dust Sensor Demo"};       
   unsigned char txt2[6] = {"g/m3:"};   
   
   unsigned long old_value = 0x0000;
   unsigned long new_value = 0x0000;
   
   unsigned long  pm = 0;
      
   setup();                                                                                                          
     
   LCD_goto(0, 0);                 
   LCD_putstr(txt1);
   LCD_goto(1, 1);                 
   LCD_putstr(txt2);
   print_symbol(0, 1, 0);
                                                                                            
   while(TRUE)
   {                                                 
      new_value = GP2Y1010AU0F_ADC_running_avg(old_value);   
      
      pm = GP2Y1010AU0F_read(new_value);
      
      old_value = new_value;
      
      print_I(12, 1, pm);
      delay_ms(100);
      
   };
}                                                                                                                                                     
                      
                                                                                                                
void setup(void)
{                                                
   disable_interrupts(GLOBAL);                           
   setup_WDT(WDT_off);
   setup_oscillator(OSC_8MHz);
   setup_comparator(NC_NC_NC_NC);                               
   setup_ADC(ADC_clock_internal);
   setup_ADC_ports(sAN1);     
   set_ADC_channel(1);    
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
   GP2Y1010AU0F_init();
} 

