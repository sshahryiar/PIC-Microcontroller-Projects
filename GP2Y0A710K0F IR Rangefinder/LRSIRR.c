#include <16F887.h>


#device *= 16
#device ADC = 10

                                                                                   
#fuses NOWDT, NOLVP, BORV40, NOWRT, BROWNOUT, MCLR, PUT    
#fuses INTRC_IO, NOIESO, NODEBUG, CPD, PROTECT, FCMEN
             
                             
#use delay(internal = 4MHz)    


#define scale              !input(pin_D1)  

#define cm_ft_conv		   0.328084

                                                                      

const unsigned char num[12] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, 0x8E, 0xA7};


short cm_ft = 0;            
                                                                        
unsigned char p = 1; 
             
unsigned long d = 0;
                                                                         
                                 
void setup();                                                                 
void adc_avg(unsigned long *value);
unsigned long get_distance(unsigned long value); 
                                 

#INT_RTCC
                                   
void timer_0_ISR()
{                                                  
	unsigned char n = 0;  
                                                                                                 
     
	switch(p)
	{                    
	    case 1:                                                             
	    {  
	        n = (d / 100);    
	        if(n == 0)
	        {
	            output_B(0xFF);    
	        }
	        else
	        {
	            output_B(num[n]);    
	        }
	        output_A(0x21);  
	        break;                                                                                        
	    }                         
	    case 2:
	    {                                                                                            
	        n = ((d / 10) % 10);    
	        if(cm_ft)
	        {                                  
	        	n = (num[n] & 0x7F); 
	        }                
	        else
	        {                                                              
	            n = num[n]; 
	        }    
	        
	        output_B(n);
	        output_A(0x22);  
	        break;
	    } 
	    case 3:  
	    {  
	        n = (d % 10);    
	        output_B(num[n]);                                                                    
	        output_A(0x24);    
	        break;
	    } 
	    case 4:                                                   
	    {            
	        if(cm_ft) 
	        {                                                                          
                n = num[10];    
	        }                                                             
	        else                                                         
	        {                           
	            n = num[11];                                                                       
	        }   
	                                                                         
	        output_B(n);  
	        output_A(0x28);
	        break;              
	    }                                                                                    
	}                   
	                
	p++;   
   	
	if(p > 4)          
	{
		p = 1;
	}             	
}
                    

void main()
{ 
	unsigned long l = 0;
   	
	setup();
	                    
	while(TRUE)
	{                                                                                                                 
		adc_avg(&l);  
		l = get_distance(l); 
	   	
		if(scale)             
		{
		    while(scale);
		    cm_ft = ~cm_ft;                                                                               
		}
	   	
		if(cm_ft)
		{                         
		    d = (l * cm_ft_conv);
		}                     
		else
		{
		    d = l;
		}
	   	
		delay_ms(600);
	};                     
}    
                                                                                                  

void setup()             
{     
	disable_interrupts(GLOBAL);                                                 
	setup_oscillator(OSC_4MHZ);
	setup_WDT(WDT_off);   
	setup_CCP1(CCP_off);    
	setup_CCP2(CCP_off);  
	set_TRIS_A(0x00);                                                                
	set_TRIS_B(0x00);                              
	port_B_pullups(FALSE); 
	setup_SPI(SPI_disabled | SPI_SS_disabled);
	setup_comparator(NC_NC_NC_NC);
	setup_timer_0(T0_internal | T0_div_8 | T0_8_BIT);   
	setup_timer_1(T1_disabled);
	setup_timer_2(T2_disabled, 255, 1);                                                             
	set_timer0(0);     
	set_timer1(0);                                                               
	set_timer2(0);                                                               
	setup_ADC(ADC_clock_div_32);                                            
	set_analog_pins(sAN6, VSS_VDD);       
	set_ADC_channel(6);         
	enable_interrupts(int_RTCC);
	enable_interrupts(GLOBAL);
}                                     


void adc_avg(unsigned long *value) 
{
	unsigned char samples = 64;
	register unsigned long avg = 0;
	                                                                                         
	while(samples > 0)
	{                                        
		read_adc(adc_start_only);
		while(adc_done() == 0);
		avg += read_adc(adc_read_only);         
		samples--;      
	}  
   	
	*value = (avg >> 6);             
} 
                                             
   
unsigned long get_distance(unsigned long value)                    
{
	register float dis = 0.0;                                                    
   	
	dis = (30500.0 / (value - 206));  
   	
	if(dis >= 600)
	{
	    dis = 600;            
	}
	if(dis <= 30)
	{           
	    dis = 30;               
	}    
                            
	return ((unsigned long)dis);                                      
}   
