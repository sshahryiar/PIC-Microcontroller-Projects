#include <12F615.h> 

              
#device *= 16
#device ADC = 8
                            
                                       
#fuses NOWDT, PROTECT, INTRC_IO, NOMCLR, PUT, NOBROWNOUT, IOSC4
                                                         
                                                                                                                                                       
#define button	!input(PIN_A3)  

                                                                          
#use delay (internal = 4MHz)  
                                                   
                   
#include "P9813.c" 
                     
                                         
void setup();
unsigned long adc_avg();                                                                      
                                                  
                                                                                
void main()                                            
{                                                         
	const unsigned char R_table[18] = {4, 44, 87, 128, 164, 195, 221, 240, 251, 255, 251, 240, 221, 195, 164, 128, 87, 44};  
	const unsigned char G_table[18] = {221, 240, 251, 255, 251, 240, 221, 195, 164, 128, 87, 44, 4, 44, 87, 128, 164, 195};      
	const unsigned char B_table[18] = {221, 195, 164, 128, 87, 44, 4, 44, 87, 128, 164, 195, 221, 240, 251, 255, 251, 240};  
   	
	unsigned char n = 0;
	unsigned char i = 0; 
   	
	unsigned char R_value = 0;
	unsigned char G_value = 0;
	unsigned char B_value = 0;     
   	
	unsigned long d = 0;                                        
	                                                                                  
	setup();
	                             
	while(TRUE)                               
	{ 
		d = adc_avg();
	   	
		if(button)
		{
		    while(button);                                                                     
		    i = 0x00;
		    n++;
		}
		if(n > 0x0F)
		{         
		    n = 0x00;
		}
	   	
		if(i > 0x11)
		{
		   i = 0x00;      
		}
	   	
		switch(n)
		{
		    case 0x01:
		    {
		    	R_value = R_table[i];   
		    	G_value = 0;
		    	B_value = 0;  
		    	break; 
		    }      
		    case 0x02:                
		    {
		    	G_value = G_table[i]; 
		    	R_value = 0;
		    	B_value = 0;  
		    	break; 
		    }
		    case 0x03:
		    {     
		    	B_value = B_table[i];
		    	G_value = 0;
		    	R_value = 0;  
		    	break; 
		    }
		    case 0x04:
		    {
		        R_value = R_table[i];
		        G_value = G_table[i];
		        B_value = B_table[i];
		        break;
		    }   
		    case 0x05:
		    {
		        R_value = 0xFE;   
		    	G_value = 0;
		    	B_value = 0;  
		    	break;   
		    }  
		    case 0x06:
		    {      
		        G_value = 0xFE;   
		    	R_value = 0;
		    	B_value = 0;  
		    	break;   
		    }
		    case 0x07:
		    {      
		        B_value = 0xFE;   
		    	R_value = 0;
		    	G_value = 0;  
		    	break;                  
		    }     
		    case 0x08:
		    {      
		        B_value = get_timer0();   
		    	R_value = (get_timer0() + 0x56);             
		    	G_value = (get_timer0() + 0xAC);  
		    	break;                      
		    }                                   
		    case 0x09:
		    {      
		        B_value = 0xFE;   
		    	R_value = 0xFE;             
		    	G_value = 0xFE;  
		    	break;                         
		    }    
		    case 0x0A:                 
		    {      
		    	R_value = d;
		    	G_value = 0x00; 
		    	B_value = 0x00; 
		    	break;                         
		    } 
		    case 0x0B:                 
		    {       
		    	G_value = d;
		    	R_value = 0x00; 
		    	B_value = 0x00; 
		    	break;                         
		    }       
		    case 0x0C:                 
		    {  
		    	B_value = d;
		    	G_value = 0x00; 
		    	R_value = 0x00; 
		    	break;                         
		    } 
		    case 0x0D:                 
		    {  
		    	R_value = d;
		    	G_value = d;          
		    	B_value = d; 
		    	break;                         
		    } 
		    case 0x0E:                 
		    {                    
		    	R_value = (0xFF - d);          
		    	G_value = (0xFF - d);          
		    	B_value = (0xFF - d);       
		    	break;                         
		    }
		    case 0x0F:                 
		    {                    
		    	B_value = B_table[(get_timer0() >> 4)]; 
		    	R_value = R_table[(get_timer0() >> 4)];            
		    	G_value = G_table[(get_timer0() >> 4)];         
		    	break;                         
		    }
		    default:
		    {                                                  
		        R_value = 0x00;
		        G_value = 0x00;
		        B_value = 0x00;
		        break;
		    }
		}                       
	   	
		i++;                        
		turn_off_strip();    
		set_colour(R_value, G_value, B_value);    
		turn_off_strip();              
		delay_ms((d << 2));     
	};                                                                             
}
                                                                                                       

void setup()
{
	disable_interrupts(global); 
	setup_comparator(NC_NC_NC_NC);
	setup_ADC(ADC_clock_div_8); 
	setup_ADC_ports(sAN0);
	set_ADC_channel(0x00);
	setup_CCP1(CCP_off);                  
	setup_timer_0(T0_internal);       
	setup_timer_1(T1_disabled); 
	setup_timer_2(T2_disabled, 0xFF, 0x01);    
	set_timer0(0x00);                      
	set_timer1(0x00);  
	set_timer2(0x00); 
	P9813_init();             
}


unsigned long adc_avg()        
{
   unsigned char s = 8;
   unsigned long avg = 0;
   
   while(s > 0)
   {
       read_adc(adc_start_only); 
       while(adc_done() == false);      
       avg += read_adc(adc_read_only);
       s--;
   }                          
                      
   avg >>= 3; 
   
   return avg;         
}
  
  

                             
