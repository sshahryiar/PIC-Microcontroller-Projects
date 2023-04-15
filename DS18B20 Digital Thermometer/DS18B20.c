#include "DS18B20.h"


void init_DS18B20()                             
{                                       
	onewire_reset();
}              
                                        
                                                             
float get_temperature()
{                                               
	unsigned char msb = 0;
	unsigned char lsb = 0;
	register float temp = 0.0;  
	                       
	onewire_reset();     
	onewire_write(skip_ROM);        
	onewire_write(convert_T); 
   	
	switch(resolution)   
	{                                                                          
		case 12:
		{	                                         
			delay_ms(750);
			break;
		}                
		case 11:                                     
		{              
			delay_ms(375);
			break;
		}           
		case 10:                                                                                 
		{                                 
			delay_ms(188);   
			break;
		}                                        
		case 9:                                                   
		{                                                
			delay_ms(94);                 
			break;                            
		}                        
	}                  
	           
	onewire_reset(); 
	onewire_write(skip_ROM);                 
	onewire_write(read_scratchpad);
	lsb = onewire_read();
	msb = onewire_read();
	temp = msb;                           
	temp *= 256;
	temp += lsb;
   	
   	
	switch(resolution)   
	{                                  
		case 12:            
		{                                                
			temp *= 0.0625;                 
			break;                            
		}	   	
		case 11: 
		{	        
			temp *= 0.125;    	
			break;
		}                
		case 10:
		{            
			temp *= 0.25;   	
			break;
		}  
		case 9:                                 
		{                                 
			temp *= 0.5;    	
			break;      
		}                                                              
	}  
   	
	delay_ms(40);   	
	return (temp);       
}
