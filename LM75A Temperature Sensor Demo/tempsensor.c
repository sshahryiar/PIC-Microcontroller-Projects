#include <16F877A.h>


#device *= 16 
#device const=ROM

                                                                     
#fuses NOWDT, XT, PROTECT, CPD, NOWRT, BROWNOUT, NODEBUG, NOLVP, PUT
                    
                        
#use delay (clock = 4MHz)           
#use I2C(Master, SDA = pin_D2, SCL = pin_D3)      
                           
               
#include "lcd.c"       
#include "LM75.c"


const unsigned char symbol[8] = {0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00};
                      
                                     
void setup(void);  
void lcd_symbols(void); 

                                                                        
void main()            
{                                                                                                                                     
     float t = 0.0;
     
     setup();
     
     while(TRUE)                                  
     {                                   
        t = LM75_get_temp();  
        lcd_gotoxy(10, 2);
        printf(lcd_putc, " %2.3f", t);    
        delay_ms(600);
     };                                  
}                                              

                            
void setup(void)              
{    
    disable_interrupts(global);      
    setup_comparator(NC_NC_NC_NC);
    setup_ADC(ADC_off);                            
    setup_ADC_ports(no_analogs);
    setup_timer_0(0 | 0| 0);  
    setup_timer_1(T1_disabled);                                                               
    setup_timer_2(T2_disabled, 255, 1); 
    set_timer0(0);
    set_timer1(0); 
    set_timer2(0);
    setup_SPI(SPI_disabled | SPI_SS_disabled);
    setup_PSP(PSP_disabled);    
    setup_CCP1(CCP_off); 
    setup_CCP2(CCP_off);  
    port_B_pullups(FALSE);
    lcd_init();          
    lcd_putc("\f");   
    lcd_symbols();     
    LM75_init();       
    lcd_gotoxy(4, 1);
    lcd_putc("LM75A Demo");    
    lcd_gotoxy(1, 2);     
    lcd_putc("T/ C:"); 
    lcd_gotoxy(3, 2);     
    lcd_putc(0); 
}                        
  
  
void lcd_symbols(void) 
{
   unsigned char s = 0x00;                 
                                                        
   lcd_send_byte(0, 0x40);
   
   for(s = 0; s < 8; s++)        
   {                           
       lcd_send_byte(1, symbol[s]);          
   }
   
   lcd_send_byte(0, 0x80);   
} 

                                                   
