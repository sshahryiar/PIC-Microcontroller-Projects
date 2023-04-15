#include <16F785.h>


#device *= 16
#device ADC = 10


#fuses NOMCLR, INTRC_IO, PROTECT, CPD, BROWNOUT, FCMEN, PUT, NOIESO   


#use delay (internal = 4MHz)  


#define button				!input(pin_B4) 
#define buzzer				pin_B5 

#define no_of_samples	    160                        
                             

#include "sound.c"


const unsigned long dot_disp[11] =                                             
{0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000}; 
                                                                                
const unsigned long bar_disp[11] =                                                                                     
{0x03FF, 0x01FF, 0x00FF, 0x007F, 0x003F, 0x001F, 0x000F, 0x0007, 0x0003, 0x0001, 0x0000}; 
                                                                                  
                                                                     
void setup();
unsigned long adc_avg();
                                                                        
                             
void main()                
{                    
	short disp = FALSE;  
	unsigned long v = 0x0000;     
	unsigned long t = 0x0000;
	                       
	setup();
   	
	while(TRUE)
	{                    
		t = adc_avg();
		v = (t / 100);                                                               
		  
		    
		if(button)                                                                                  
		{
			while(button);
			disp = ~disp;
		}                                                                  
		                        
		switch(disp)
		{                                                                          
		    case TRUE:
		    {                  
		    	v = dot_disp[v];		    	         
		    	break;
		    }                     
		    default:                     
		    {                              
		    	v = bar_disp[v];   	
		    	break;                                   
		    }               
		}                                                                            
		                                                                                         
		output_C(v & 0x00FF);                          
		v = ((v & 0x0300) >> 2);      
		v = (v & 0xC0);               
		output_B(v);                                                           
		if(t < 900)
		{
		    generate_tone(2400, (t / 10)); 	
		}   
		else
		{     
			delay_ms(100); 	
		}
	};                                    
}                                                                                                                      

                            
void setup()
{     
	disable_interrupts(GLOBAL);                                              
	setup_oscillator(OSC_4MHz); 
	setup_opamp1(OPAMP_disabled); 
	setup_opamp2(OPAMP_disabled); 
	port_A_pullups(FALSE);    
	setup_comparator(NC_NC_NC_NC);                                                       
	setup_CCP1(CCP_off);                          
	setup_ADC(ADC_clock_internal);
	setup_ADC_ports(sAN0);
	set_ADC_channel(0);            
	setup_timer_0(T0_internal);         
	setup_timer_1(T1_disabled);                                                                                            
	setup_timer_2(T2_disabled, 255, 1);
	set_timer0(0); 
	set_timer1(0);
	set_timer2(0);  
	delay_ms(100); 
}


unsigned long adc_avg()
{                         
	unsigned char samples = no_of_samples;
	register unsigned long long avg = 0; 
	                      
	while(samples > 0)
	{                                     
		read_adc(ADC_start_only);
		while(!adc_done());                                                              
		avg += read_adc(ADC_read_only);                                                          
		samples--;
	};                                                                    
	                                          
	avg /= no_of_samples;
	return avg;                                                                  
}                                                                                                        
