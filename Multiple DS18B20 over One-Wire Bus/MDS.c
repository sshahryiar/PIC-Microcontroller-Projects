#include <16F690.h> 
             

#device *= 16
                                          

#fuses NOWDT, PROTECT, CPD, INTRC_IO, PUT, BROWNOUT, NOMCLR, FCMEN, NOIESO
                                                        

#use delay (internal = 4MHz)

                                         
#include "DS18x20.c"      
#include "lcd.c"               


#define LED		pin_B4

                                                               
const unsigned char symbol[8] = {0x00, 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00}; 

                      
void setup();
void lcd_symbols(); 
                                                 

void main()
{                              
	setup();
	                                    
	float t = 0;      
	unsigned char sensor_count = 0;
   	
	lcd_gotoxy(1, 1);
	                                 
	while(TRUE)
	{                  
		output_high(LED);
		sensor_count = 0;
		if(DS1820_find_first_device())
		{
			do
			{                             
				t = DS1820_get_temp();     
				lcd_gotoxy(1, (sensor_count + 1)); 
				printf(lcd_putc, "Tmp %1u/ C: %3.3g ", sensor_count, t); 
				lcd_gotoxy(7, (sensor_count + 1));                                            
				lcd_putc(0);             
				sensor_count++;           
			}while(DS1820_find_next_device()); 
			sensor_count = 0;                            
		}                
		output_low(LED);  
	};                                  
}                                        
                                         
                          
void setup()
{                                       
	disable_interrupts(GLOBAL);  
	setup_oscillator(OSC_4MHz);
	setup_WDT(WDT_off);
	setup_comparator(NC_NC_NC_NC);
	setup_ADC(ADC_off);
	setup_ADC_ports(no_analogs);
	setup_SPI(SPI_disabled | SPI_SS_disabled);
	setup_CCP1(CCP_off); 
	setup_timer_0(T0_internal);  
	setup_timer_1(T1_disabled); 
	setup_timer_2(T2_disabled, 255, 1);  
	set_timer0(0);
	set_timer1(0);
	set_timer2(0);
	lcd_init();              
	lcd_putc("\f"); 
	lcd_symbols();
}

 
void lcd_symbols() 
{
   unsigned char s = 0;
   
   lcd_send_byte(0, 0x40); 
   for(s = 0; s < 8; s++)        
   {                                          
        lcd_send_byte(1, symbol[s]);          
   }
   lcd_send_byte(0, 0x80);
}  
                         



