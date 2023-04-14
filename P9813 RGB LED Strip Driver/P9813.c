#include "P9813.h"            


void P9813_init() 
{                         
    output_low(data_pin);                     
    output_low(clock_pin);
    turn_off_strip();
}
                                        

void P9813_clock()              
{                      
	output_low(clock_pin);
	delay_us(20);               
	output_high(clock_pin);                         
	delay_us(20);                                                                           
}   
            
                                     
void turn_off_strip() 
{                                              
   	
	unsigned char s = 32;        
                                                           
	output_low(data_pin);
	while(s > 0)
	{                                                                        
	    P9813_clock();
	    s--;
	}
    
}  


void send_data(unsigned long long value)
{
	unsigned char s = 0;
	               
	for(s = 0; s < 32; s++)
	{                                     
		if((value & 0x80000000) != 0)
		{
			output_high(data_pin);                        
		}
		else                              
		{                                    
			output_low(data_pin);  
		}                
		         
		value <<= 1;
		P9813_clock();    
	}
}
                              

unsigned char make_anti_code(unsigned char value)
{
  unsigned char s = 0;
 
  if((value & 0x80) == 0)
  {
	s |= 0x02; 
  }            
  if((value & 0x40) == 0)
  {
	s |= 0x01;                  
  }
        
  return s;
}  


void set_colour(unsigned char R, unsigned char G, unsigned char B)
{
  unsigned long long tmp = 0;
                
  tmp |= (((unsigned long long)0x03) << 30);            
  tmp |= (((unsigned long long)make_anti_code(B)) << 28);
  tmp |= (((unsigned long long)make_anti_code(G)) << 26);   	
  tmp |= (((unsigned long long)make_anti_code(R)) << 24);
  tmp |= (((unsigned long long)B) << 16);
  tmp |= (((unsigned long long)G) << 8); 
  tmp |= ((unsigned long long)R);                     
  
  send_data(tmp);
}
                                             



                                        

