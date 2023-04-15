#include <16F1503.h> 


#device *= 16
#device ADC = 10


#fuses NOWDT, INTRC_IO, PUT, NOCLKOUT, BROWNOUT
#fuses NOWRT, STVREN, BORV25, LPBOR, NOLVP, NOMCLR 


#use delay (internal = 4MHz) 
#include "lcd.c"           


                  
void setup();       
void adc_avg(unsigned long *value); 
float get_distance(unsigned long value);


void main()
{ 
	unsigned long v = 0;   
	float d = 0.0;
   	
	setup();
	                                   
	while(TRUE)
	{    
		 adc_avg(&v);
		 d = get_distance(v);   
		 dac_write((v >> 2));  
		          
		 lcd_gotoxy(7, 1);                
		 printf(lcd_putc, "%2.2g ", d);
		 
		 lcd_gotoxy(1, 2);
		 lcd_putc("                ");   
		            
		 for(v = 0; v < (d / 3.1); v++)
		 {                        
			lcd_gotoxy((v + 1), 2);
		    lcd_putc(">");   
		 }                          
		            
		 delay_ms(100);        
	}                  
}
                                    

void setup()
{        
	setup_wdt(WDT_OFF);
	setup_oscillator(OSC_4MHZ | OSC_INTRC);
	port_A_pullups(FALSE);         
	port_C_pullups(FALSE);     
	setup_pwm1(PWM_disabled);
	setup_pwm2(PWM_disabled);
	setup_pwm3(PWM_disabled);
	setup_pwm4(PWM_disabled);
	set_pwm1_duty(0); 
	set_pwm2_duty(0);               
	set_pwm3_duty(0); 
	set_pwm4_duty(0);
	setup_SPI(SPI_disabled | SPI_SS_disabled); 
	setup_comparator(NC_NC_NC_NC);
	setup_dac(DAC_VSS_VDD | DAC_OUTPUT2);      
	setup_cwg(CWG_DISABLED, CWG_SHUTDOWN_ON_FLT, 0, 0);  
	setup_nco(NCO_DISABLED, 0);   
	set_nco_inc_value(0);
    set_nco_accumulator(0);      
    setup_clc1(CLC_disabled);
    setup_clc2(CLC_disabled);
	setup_timer_0(RTCC_internal | RTCC_DIV_1); 
	setup_timer_1(T1_disabled);                        
	setup_timer_2(T2_disabled, 255, 1);
	set_timer0(0);
	set_timer1(0);
	set_timer2(0);
	setup_adc(ADC_CLOCK_DIV_8);
	setup_adc_ports(sAN0 | VSS_VDD);   
	set_ADC_channel(0);
	lcd_init();
	lcd_putc("\f");  
	lcd_gotoxy(1, 1);                                                                                                                           
	lcd_putc("D/cm:"); 
} 


void adc_avg(unsigned long *value)
{
	unsigned char samples = 16;
	unsigned long avg = 0;
   	
	while(samples > 0)
	{
		read_adc(adc_start_only);
		while(adc_done() == 0);
		avg += read_adc(adc_read_only);         
		samples--;
	}  
   	
	*value = (avg >> 4);
} 


float get_distance(unsigned long value)
{
	float dis = 0.0;  
                
	if(value <= 52)           
	{                                      
		value = 52;                    
	}     
   	
	dis = (2076.0 / (value - 11));    
                            
	return dis;                                      
}                                    
