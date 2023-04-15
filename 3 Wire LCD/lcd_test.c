#include <12F675.h>


#device *= 16

                                 
#fuses NOWDT, PROTECT, CPD, NOMCLR, INTRC_IO, BROWNOUT, PUT

                               
#use delay (internal = 4MHz)    
#include "lcd.c"                          


void setup();                       


void main()
{         
    char txt1[] = {"MicroArena"}; 
    char txt2[] = {"SShahryiar"}; 
                                    
    setup();
                                          
    LCD_goto(3, 0);                              
    LCD_putstr(txt1); 
    LCD_goto(3, 1);
    LCD_putstr(txt2);  
    while(TRUE)
    {
    };
}                                       
           


void setup()
{
    disable_interrupts(GLOBAL);
    setup_comparator(NC_NC_NC_NC);
    setup_ADC(ADC_off);
    setup_ADC_ports(no_analogs);
    setup_timer_0(T0_internal);      
    setup_timer_1(T1_disabled);
    set_timer0(0);  
    set_timer1(0);
    LCD_init();  
    LCD_clear_home();  
    delay_ms(200);        
}
    
