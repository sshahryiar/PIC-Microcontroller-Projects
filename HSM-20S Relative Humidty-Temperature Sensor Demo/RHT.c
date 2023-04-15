#include <16F877A.h>

               
#device *= 16
#device ADC = 10


#fuses NOWDT, PROTECT, CPD, NOLVP, HS
#fuses NOBROWNOUT, NODEBUG, NOWRT, PUT 

                                                                                                     
#use delay (clock = 10MHz) 


#include "lcd.c"  


const unsigned char symbol[8] = {0x00 ,0x0E, 0x0A ,0x0E, 0x00, 0x00, 0x00, 0x00};

              
void setup(); 
void lcd_symbols();
void get_parameters(float *relative_humidity, float *temperature);
       

void main()  
{                                                   
	float t = 0;
	float rh = 0;
   	
	setup();
	                                                                       
	                                                         
	while(TRUE)           
	{   		               
		get_parameters(&rh, &t);   	
		lcd_gotoxy(9, 1);
		printf(lcd_putc, "%3.2g  ", rh);   
		lcd_gotoxy(9, 2);
		printf(lcd_putc, "%3.2g  ", t); 
	};                       
}  


void setup()
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
	setup_ADC(ADC_clock_internal);
	set_analog_pins(AN0_AN1_AN3);  
	lcd_init();
	lcd_symbols(); 
	lcd_putc("\f");      
	lcd_gotoxy(3, 1);
	lcd_putc("RH/%:"); 
	lcd_gotoxy(3, 2);  
	lcd_putc("T/ C:");  
	lcd_gotoxy(5, 2);
	lcd_putc(0);
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
            
                           
void get_parameters(float *relative_humidity, float *temperature)  
{
	unsigned long samples = 256;    
   	
	register unsigned long long avg_rh = 0; 
	register unsigned long long avg_t = 0;                    
   	
	while(samples > 0)
	{                          
		set_ADC_channel(0);          
		delay_ms(1);     
		read_adc(adc_start_only);                     
		while(adc_done() == 0);                
		avg_rh += read_adc(adc_read_only);                                      
	   	
		set_ADC_channel(1);
		delay_ms(1);     
		read_adc(adc_start_only);                     
		while(adc_done() == 0);                
		avg_t += read_adc(adc_read_only); 
	   	
		samples--;                         
	}; 
   	
	avg_rh >>= 8; 
	avg_t >>= 8; 
   	
	*relative_humidity = ((0.1507 * avg_rh) - 11.4378);
	*temperature = ((-0.0002 * avg_t * avg_t) + (avg_t * 0.2268) - 11.4945);
}                 
