#include "TM1640.h"
             
               
void TM1640_init(unsigned char brightness_level)
{                
	output_high(DIN);
	output_high(SCLK);
	                        
	TM1640_send_command(auto_address);
	TM1640_send_command(brightness_level);   
	TM1640_clear_display();                    
}                                
                           
                                                                             
void TM1640_write(unsigned char value)  
{                                                       
	unsigned char s = 0x00;  
	                      
	for(s = 0x00; s < 0x08; s += 0x01)                                                                   
	{                                                
		output_low(SCLK); 
	   	
		if((value & 0x01))
		{                                                                                 
		    output_high(DIN);
		}                                 
		else                       
		{                      
			output_low(DIN);   
		}                                              
		                   
		output_high(SCLK);  
                                  
		value >>= 0x01; 
	};          
}                                 


void TM1640_send_command(unsigned char value)    
{                            
	output_low(DIN); 
	TM1640_write(value);                  
	output_high(DIN);  
}               


void TM1640_send_data(unsigned char address, unsigned char value)
{                  
    TM1640_send_command(fixed_address); 
    output_low(DIN);                           
	TM1640_write((0xC0 | (0x0F & address)));   
	TM1640_write(value); 
	output_high(DIN);  
}  


void TM1640_clear_display()  
{ 
	unsigned char s = 0x00;	 
   	
	output_low(DIN);  
	TM1640_write(start_address);
	for(s = 0x00; s < no_of_segments; s += 0x01)  
	{	       
		TM1640_write(font[0]);         
	};              
	output_high(DIN);  
}
                       
