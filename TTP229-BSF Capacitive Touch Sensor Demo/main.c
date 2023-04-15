#include <16F877A.h>


#fuses NOWDT, PROTECT, CPD, NOLVP, HS, NOBROWNOUT, NODEBUG, NOWRT, PUT  
                                                         
                                                                                                     
#use delay (clock = 10MHz) 

                                                           
#include "lcd.c"   


#define SDO_pin               pin_C7
#define SCL_pin               pin_C6

#define get_SDO_state()       !input(SDO_pin)

#define SCL_pin_High()        output_high(SCL_pin)
#define SCL_pin_Low()         output_low(SCL_pin)


unsigned char pressed_key = 0;

                          
void setup(void);
unsigned char read_TTP229_BSF_keypad(void); 


#int_RTCC
void RTCC_ISR(void)
{
   pressed_key = read_TTP229_BSF_keypad();
   set_timer0(0);
}
                                                                                                                    
                                                                                                                        
void main()                                                                    
{                                                            
   setup();
   
   lcd_gotoxy(1, 1);
   lcd_putc("TTP229-BSF  Demo");
   lcd_gotoxy(1, 2);
   lcd_putc("Pressed Key:");
                       
   while(TRUE)                               
   {                         
      lcd_gotoxy(15, 2);
      printf(lcd_putc, "%02u ", pressed_key);
      
      delay_ms(100);
   };                                             
}                                                 
                                   
                                         
void setup(void)
{                                                                                               
   disable_interrupts(global);              
   setup_timer_0(T0_internal); 
   setup_timer_1(T1_disabled);
   setup_timer_2(T2_disabled, 255, 1);
   set_timer0(0);
   set_timer1(0);           
   set_timer2(0);                                                           
   setup_CCP1(CCP_off);
   setup_CCP2(CCP_off);
   setup_psp(PSP_disabled);                                
   setup_comparator(NC_NC_NC_NC);       
   setup_SPI(SPI_disabled);                   
   port_B_pullups(FALSE);  
   setup_adc_ports(no_analogs);
   setup_ADC(ADC_OFF);                    
   enable_interrupts(int_RTCC);
   enable_interrupts(global);
   lcd_init();   
   lcd_putc("\r");
}                                                            


unsigned char read_TTP229_BSF_keypad(void)
{
   unsigned char s = 16;
   unsigned char key_value = 0;
   
   while(s > 0)
   {
      SCL_pin_High();
      SCL_pin_Low();
      
      if (get_SDO_state())
      {
         key_value = (17 - s); 
      }
     
      s--;
   } 
   
   return key_value; 
}
