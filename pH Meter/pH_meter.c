#include <16F684.h>


#device *= 16
#device ADC = 10


#fuses NOWDT, INTRC_IO, PROTECT, PUT, CPD
#fuses NOBROWNOUT, NOMCLR, NOIESO, FCMEN
    
                                                      
#use delay (internal = 4MHz)


#include "lcd.c" 


#define ADC_const		0.004887585     
#define LED				pin_C3

                                                                                    
const unsigned char symbol[16]= 
{   
    0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, 0x00, 
    0x00, 0x00, 0x0E, 0x0E, 0x0E, 0x00, 0x00, 0x00
};


void setup();               
void lcd_symbol();                                 
unsigned long adc_avg();
void show_bar(unsigned char value);  


void main()   
{
	unsigned char samples_A = 0;  
	unsigned char samples_B = 0; 
	unsigned long temp = 0;  
	unsigned long avg = 0;                    
	unsigned long buffer[10];                 
	float pH_value = 0.0;                   
   	
	memset(buffer, 0x00, sizeof(buffer));  
   	
	setup();
   	
	while(TRUE)
	{       
		for(samples_A = 0; samples_A < 10; samples_A++)
		{
			buffer[samples_A] = adc_avg();
			delay_ms(9);
		}
	   	
		for(samples_A = 0; samples_A < 9; samples_A++) 
		{     
			for(samples_B = (1 + samples_A); samples_B < 10; samples_B++)  
			{       
				if(buffer[samples_A] > buffer[samples_B]) 
				{       
					temp = buffer[samples_A];    
					buffer[samples_A] = buffer[samples_B];
					buffer[samples_B] = temp; 
				}
			}
		} 
	   	
		avg = 0;  
	   	
		for(samples_A = 3; samples_A <= 6; samples_A++)
		{
			avg += buffer[samples_A];
		} 
	   	
		avg >>= 2;
	   	
		pH_value = (avg * ADC_const);
		pH_value *= 3.5; 
	   	
		lcd_gotoxy(1, 1);
		printf(lcd_putc, "pH Value: %2.2g ", pH_value);
		show_bar(((unsigned char)pH_value));
	};
}
                      

void setup()
{                                                
	disable_interrupts(GLOBAL);                           
	setup_WDT(WDT_off);
	setup_oscillator(OSC_4MHz);
	setup_comparator(NC_NC_NC_NC);
	setup_ADC(ADC_clock_div_8);
	setup_ADC_ports(sAN0);
	set_ADC_channel(0);                                   
	setup_CCP1(CCP_off);
	setup_timer_0(T0_internal);
	setup_timer_1(T1_disabled);      
	setup_timer_2(T2_disabled, 255, 1);
	set_timer0(0);       
	set_timer1(0);  
	set_timer2(0);    
	lcd_init();
	lcd_putc("\f"); 
	lcd_symbol();
} 


void lcd_symbol()
{
   unsigned char s = 0;
   
   lcd_send_byte(0, 0x40);      
   
   for(s = 0; s < 16; s++) 
   {                                           
        lcd_send_byte(1, symbol[s]);
   }
   
   lcd_send_byte(0, 0x80);
}  


unsigned long adc_avg() 
{                 
	unsigned char samples = 8;
	unsigned long avg = 0;  
	      
	while(samples > 0) 
	{
	    read_adc(adc_start_only); 
	    while(!adc_done());
	    avg += read_adc(adc_read_only);      
	    samples--;           
	}        
	avg >>= 3;                                          

	return avg;
}


void show_bar(unsigned char value)
{
	unsigned char x_pos = 0;
	    
	for(x_pos = 1; x_pos <= 16; x_pos++)
	{                    
		lcd_gotoxy(x_pos, 2);
		lcd_putc(0);                      
	}                  
   	
	lcd_gotoxy((value + 1), 2);  
	lcd_putc(1);
   	
	output_high(LED);  
	delay_ms(200); 
	output_low(LED);  
}
                                    


