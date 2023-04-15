#include <12F675.h>


#device *= 16

                                 
#fuses NOWDT, PROTECT, CPD, NOMCLR, INTRC_IO, BROWNOUT, PUT

                               
#use delay (internal = 4MHz)    
#include "lcd.c"                          


void setup();                       
                                         
                               
void main()
{                        
    unsigned char i = 0;      
    
    char txt1[] = {"MicroArena"};
    char txt2[] = {"SShahryiar"}; 
    char txt[6];
                                    
    setup();                   

    while(TRUE)
    {                                                                                     
        LCD_clear_home();    
             
        for(i = 0; i <= 9; i++)
        {                  
        	LCD_goto((i + 3), 0);   	
        	LCD_putchar(txt1[i]);
        	delay_ms(99);
        }                                                
        for(i = 0; i <= 9; i++)                                   
        {            
        	LCD_goto((i + 3), 1);                                                         
        	LCD_putchar(txt2[i]);                                         
        	delay_ms(99);
        }         
                                  
        delay_ms(4000); 
        
        LCD_clear_home();  
        
        LCD_goto(3, 0);
        LCD_putstr(txt1);                
        for(i = 0; i <= 100; i++)
        {
        	LCD_goto(7, 1); 
        	sprintf(txt,"%02u", i);
        	LCD_putstr(txt);    
        	delay_ms(500);
        }
        
        
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
    delay_ms(100);        
}                           
