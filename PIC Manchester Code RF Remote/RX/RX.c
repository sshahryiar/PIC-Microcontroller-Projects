#include <16F630.h>


#device *= 16

                                          
#fuses NOMCLR, NOWDT, PROTECT, CPD, INTRC_IO, BROWNOUT
        

#use delay (internal = 4MHz)       
#use RS232 (xmit = pin_C5, rcv = pin_C4, baud = 1200, stop = 1, parity = N) 
                                    
   
#define OUT1		pin_C0  
#define OUT2		pin_C1
#define OUT3		pin_C2
#define OUT4		pin_C3     


void setup();               
unsigned char receive_data();
                            

void main()                                                                             
{                         
    unsigned char value = 0;                         
               
	setup();
	             
	while(TRUE)
	{                

			value = receive_data();
		   	
			if(value == 0x04)
			{  
				output_toggle(OUT1);
			}   
			if(value == 0x06)
			{                    
				output_toggle(OUT2);
			}  
			if(value == 0x09)
			{                            
				output_toggle(OUT3);
			}   
			if(value == 0x11)
			{                    
				output_toggle(OUT4);
			}    
	};  
}    
                        

void setup()
{
	disable_interrupts(global);  
	setup_comparator(NC_NC_NC_NC);
	setup_timer_0(T0_internal);     
	setup_timer_1(T1_disabled); 
	set_timer0(0);
	set_timer1(0);  
}   


unsigned char receive_data()
{  
	unsigned char s = 0;   
	unsigned char cmd[4];     
	unsigned char tmp1 = 0;
	unsigned char tmp2 = 0;   
	unsigned long temp = 0; 
   	
	if(kbhit())             
	{                                   
		if(getc() == 0x55)        
		{          
			for(s = 0; s < 4; s++)
			{               
		    	cmd[s] = getc();    
			}  
		   	
			tmp1 = cmd[0]; 
	        tmp2 = cmd[1];
	       
			  
			if(((tmp2 & tmp1) == cmd[2]) && (cmd[3] == 0xAA))                              
			{                     
				temp = tmp1;  
				temp <<= 8;  
				temp |= tmp2;
			   	
				tmp2 = 0;                  
				                                             
				for(s = 0; s < 8; s++)
				{                                                     
					tmp2 <<= 1;   
				   	
					if((temp & 0xC000) == 0x8000)
					{
						tmp2 |= 0x00;  
					}                     
					else if((temp & 0xC000) == 0x4000)    
					{                              
						tmp2 |= 0x01;                     
					}              
					         
					temp <<= 2;
				}
			} 
		}   
	}
	return tmp2;
}
