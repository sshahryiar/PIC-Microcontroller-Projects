#include <16F877A.h>


#device *=16
 

#fuses NOWDT, PROTECT, CPD, NOLVP, XT, NOBROWNOUT, NODEBUG, NOWRT, PUT  
                                                                        
                                                                                                     
#use delay (clock = 4MHz) 


#include <math.h>
#include "lcd.c"   
#include "QMC5883.c"


const unsigned char symbol[8] = 

{0x04, 0x0A, 0x0A, 0x04, 0x00, 0x00, 0x00, 0x00};

          
void setup(void);     
void lcd_symbol(void) ;
                                                                                                                    
                                                                                                                        
void main(void)                                                                    
{                                                                 
   signed long x_value = 0;
   signed long y_value = 0;
   signed long z_value = 0;
   
   float heading = 0.0;
   float t = 0.0;
   
   setup();
   
   lcd_gotoxy(1, 1); 
   lcd_putc("Heading  N"); 
   lcd_gotoxy(9, 1);                          
   lcd_putc(0); 
   
   lcd_gotoxy(1, 2); 
   lcd_putc("Temp. /  C"); 
   lcd_gotoxy(9, 2);                          
   lcd_putc(0); 
                       
   while(TRUE)                               
   {                                
      QMC5883_read_axes(&x_value, &y_value, &z_value);
      
      heading = QMC5883_read_heading(y_value, x_value);
      
      t = QMC5883_read_temperature();
      
      
      lcd_gotoxy(12, 1);
      printf(lcd_putc, "%03.1f  ", heading);
      
       lcd_gotoxy(13, 2);
      printf(lcd_putc, "%03.1f  ", t);
      delay_ms(600);
   };                                             
}                                                 
                                   
                                         
void setup(void)
{                                                                                               
   disable_interrupts(global);              
   setup_timer_0(T0_internal | T0_8_BIT);
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
   setup_ADC(ADC_off);                    
   set_analog_pins(no_analogs);
   QMC5883_init();
   lcd_init();    
   lcd_symbol();
   lcd_putc("\f");
}   


void lcd_symbol(void)                    
{
   unsigned char i = 0x00;
   
   lcd_send_byte(0, 0x40); 
   
   for(i = 0; i < 8; i++)
   {
       lcd_send_byte(1, symbol[i]);
   }
   
   lcd_send_byte(0, 0x80);    
}

           

