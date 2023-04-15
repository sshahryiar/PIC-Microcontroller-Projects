#include <18F452.h> 
             

#device *= 16


#fuses H4, NOWDT, PUT, PROTECT, NOSTVREN
#fuses NOCPB, NOWRT, NOWRTB NOEBTRB, NOEBTR
#fuses BROWNOUT, CPD, NOOSCSEN, BORV27                                                                              
#fuses NOLVP, NODEBUG, NOWRTC, NOWRTD
    
                                          
#use delay(clock = 40MHz)   

                                 
#include "SSD1306.c"   
                                  

void setup();
                                                 

void main()
{
    unsigned char p = 0;
    signed long i = -9;
    float f = -10.0;   
    
    
    char text[] = {"MicroArena"};	    
	                    
	setup();
	                    	                                                                                                     
	while(TRUE)
	{     
		 i = -9;
         f = -10.0;                
         
         OLED_clear_buffer();   
         OLED_fill(0x00);    
         delay_ms(600);       
         OLED_fill(0xFF);             
         delay_ms(600); 
         OLED_fill(0xAA);
         delay_ms(600); 
         OLED_fill(0x55);                                                    
         delay_ms(600);           
                                                                                                                                      
         OLED_fill(0x00);               
         OLED_clear_buffer();                                                                                                                    
         OLED_print_string(36, 1, text);                                           
         Draw_Line(0, 19, 127, 19, ON);           
         Draw_Line(0, 60, 127, 60, ON);                                                       
         delay_ms(900);  
         Draw_Line(3, 16, 3, 63, ON);  
         Draw_Line(124, 16, 124, 63, ON);                                                  
         delay_ms(900);    
         Draw_Rectangle(122, 58, 5, 21, OFF, ON, SQUARE);                                                                 
         delay_ms(900);       
         Draw_Circle(63, 40, 7, ON, ON);                                                    
         delay_ms(2000);                 
                                         
         OLED_fill(0x00);               
         OLED_clear_buffer();                                                                                                                    
         OLED_print_string(36, 1, text);   
         Draw_Rectangle(5, 21, 122, 58, ON, ON, SQUARE);
         delay_ms(900);                              
         Draw_Circle(63, 40, 7, ON, OFF);                                                       
         delay_ms(2000);              
                                                          
                                                         
         OLED_fill(0x00); 
         OLED_clear_buffer(); 
         OLED_print_string(36, 0, text);                         
         for(p = 0; p < 250; p++)
         {
             f += 0.1;            
             i += 1;                                                           

             OLED_print_float(42, 5, f, 1);                          
             OLED_print_int(42, 6, i);
             OLED_print_chr(42, 7, p);
                         
             delay_ms(99);
         }
	};
}                                          
                                                                                          


void setup()                 
{
   disable_interrupts(global);
   setup_WDT(WDT_off);
   setup_adc_ports(no_analogs);
   setup_adc(ADC_clock_internal);                                   
   setup_timer_0(T0_OFF);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED, 255, 1);
   setup_timer_3(T3_DISABLED);
   setup_CCP1(CCP_off);
   setup_CCP2(CCP_off);
   setup_SPI(SPI_DISABLED);
   set_timer0(0);
   set_timer1(0);
   set_timer2(0);
   set_timer3(0); 
   OLED_init();                    
} 
             
