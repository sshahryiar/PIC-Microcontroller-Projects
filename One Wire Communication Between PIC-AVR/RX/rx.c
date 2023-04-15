#include <12F675.h>  

                             
#device *= 16


#fuses INTRC_IO, NOMCLR, PUT, NOBROWNOUT, PROTECT
                                   
                                                                  
#use delay (internal = 4MHz)    
#use fast_io(A)       
                                               
                                       
#define  Load1		pin_A0                 
#define  Load2		pin_A1            
#define  Load3		pin_A2                                                                                     
#define  LED		pin_A4                               
                                                               
#define  RX		input(pin_A3) 

                                                   
void setup();              
void start_timer();            
void stop_timer();
unsigned char data_RX();                                   
                                                             
                              
void main()                           
{                  
	unsigned char val = 0;	
	setup();                      
	while(TRUE)           
	{                 
	   val = data_RX();                   
	   if(val == 9)
	   {
			output_toggle(Load1); 
			delay_ms(100);     
	   }    
	   if(val == 119)
	   {
			output_toggle(Load2);    
			delay_ms(100);     
	   }        
	   if(val == 127)
	   {
			output_toggle(Load3);   
			delay_ms(100);     
	   } 
	};                          
}                               


void setup()
{                       
	disable_interrupts(global);
	set_TRIS_A(0x80);                   
	setup_comparator(NC_NC_NC_NC);
	setup_ADC(ADC_off);
	setup_ADC_ports(no_analogs);             
	port_A_pullups(FALSE); 
	setup_timer_0(T0_internal | T0_div_2 | T0_8_bit);    
	setup_timer_1(T1_disabled);                                                                                                                          
	set_timer0(0);                 
	set_timer1(0);                                                                  
	output_A(0x00);  
}
                                      
                       
unsigned char data_RX()     
{       	                                                          
		unsigned char data = 0;
		unsigned char period = 0;
		unsigned char bits = 0;     
		
		if(RX)
		{
			do                 
			{
			      while(!RX);     
			      output_high(LED); 
			      while(RX);         
			      period = get_timer0();           
			      set_timer0(0);   
			      output_low(LED);   
			}while(period < 100);      
			bits = 8;
			while(bits)                  
			{
					  while(!RX); 
					  output_high(LED); 
				      while(RX);  
				      period = get_timer0();             
			          set_timer0(0);                   
			          output_low(LED);  
				      if(period >= 100) 
				      {                 
				    		break;  
				      }
				                      
				      if(period < 60)     
				      {   
				    		data |= 0;	
				      }
				      else                
				      {              
				    		data |= 1;
				      }               
				      data <<= 1;   
				      bits--;                   
			}                    
			return (data >> 1);
		}
}                          
                                                                                   
