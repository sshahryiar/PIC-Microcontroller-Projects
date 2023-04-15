#include "keypad_display.h"                            
                           

void display_keypad_init()
{
	set_TRIS_A(0xD1);
	set_TRIS_B(0xFF);        	
	set_TRIS_C(0xFE);
	set_TRIS_D(0x00); 
	set_TRIS_E(0xF0);
	                
	output_D(0x00);                        
   	
	setup_comparator(NC_NC_NC_NC);
	setup_SPI(SPI_disabled | SPI_SS_disabled);
	setup_ADC(ADC_off);                          
	setup_ADC_ports(no_analogs); 
	                              
	port_B_pullups(FALSE); 
   	
	setup_timer_0(T0_internal | T0_div_64 | T0_8_bit);
	set_timer0(0xD8);                                  
   	
	enable_interrupts(GLOBAL);               
	enable_interrupts(INT_RTCC);    
}                                 


#int_RTCC

void scan_display()
{
	set_timer0(0xD8);     
	output_D(buffer[pos]);    
	   
	switch(pos)
	{
		case 7: 
		{                 
			output_high(col1);  
			output_low(col2); 
			output_low(col3);  
			output_low(col4);                
			output_low(col5);    
			output_low(col6); 
			output_low(col7);  
			output_low(col8);  
	   	
		    if(sw0)
			{
				key_pressed = TRUE;
				key = 0x01;
			}                             
			if(sw1)                   
			{            
				key_pressed = TRUE;
				key = 0x02;
			}          
			if(sw2)     
			{           
				key_pressed = TRUE;
				key = 0x03;
			}                     
			if(sw3)
			{                     
				key_pressed = TRUE;
				key = 0x0A;
			}   
		   	
			break;              
		}  	
	   	
		case 6:
		{                 
			output_high(col2);  
			output_low(col1); 
			output_low(col3);  
			output_low(col4);                
			output_low(col5);    
			output_low(col6); 
			output_low(col7);  
			output_low(col8);  
		   	
			if(sw0)
			{                      
				key_pressed = TRUE;   	
				key = 0x04;
			}                             
			if(sw1)
			{            
				key_pressed = TRUE;
				key = 0x05;
			}          
			if(sw2)     
			{           
				key_pressed = TRUE;   	
				key = 0x06;
			}   
			if(sw3)
			{    
				key_pressed = TRUE;
				key = 0x0B;
			}     
		   	
			break;               
		}
	   	
		case 5:
		{                 
			output_high(col3);  
			output_low(col1); 
			output_low(col2);  
			output_low(col4);                
			output_low(col5);    
			output_low(col6); 
			output_low(col7);  
			output_low(col8); 
		   	
			if(sw0)                              
			{                    
				key_pressed = TRUE;
				key = 0x07;
			}                             
			if(sw1)
			{            
				key_pressed = TRUE;
				key = 0x08;
			}          
			if(sw2)                            
			{           
				key_pressed = TRUE;
				key = 0x09;
			}   
			if(sw3)
			{    
				key_pressed = TRUE;
				key = 0x0C;
			}    
		   	
			break;              
		} 
	   	
		case 4:                                
		{                 
			output_high(col4);  
			output_low(col1); 
			output_low(col2);  
			output_low(col3);                
			output_low(col5);    
			output_low(col6); 
			output_low(col7);
			output_low(col8);
	   	
		    if(sw0)                                                               
			{
				key_pressed = TRUE;
				key = 0x0F;
			}                             
			if(sw1)
			{            
				key_pressed = TRUE;
				key = 0x00;
			}          
			if(sw2)     
			{           
				key_pressed = TRUE;
				key = 0x0E;
			}   
			if(sw3)             
			{    
				key_pressed = TRUE;
				key = 0x0D;
			}  
		   	
			break;              
		}
		     
		case 3:                                
		{                                                       
			output_high(col5);  
			output_low(col1); 
			output_low(col2);  
			output_low(col3);                
			output_low(col4);    
			output_low(col6); 
			output_low(col7);  
			output_low(col8);  
			break;              
		}  
	   	
		case 2:                                
		{                                                       
			output_high(col6);  
			output_low(col1); 
			output_low(col2);  
			output_low(col3);                
			output_low(col4);    
			output_low(col5); 
			output_low(col7);  
			output_low(col8);  
			break;              
		}  
		     
		case 1:                                
		{                                                       
			output_high(col7);  
			output_low(col1); 
			output_low(col2);  
			output_low(col3);                
			output_low(col4);    
			output_low(col5); 
			output_low(col6);  
			output_low(col8);  
			break;                 
		}  
	   	
		case 0:                                
		{                                                       
			output_high(col8);  
			output_low(col1); 
			output_low(col2);  
			output_low(col3);                
			output_low(col4);    
			output_low(col5); 
			output_low(col6);  
			output_low(col7);  
			break;              
		}    
	}           
	    
	pos++;     
   	
	if(pos >= 8)
	{          
		pos = 0;          
	}
}  


void clear_buffer()
{
	memset(buffer, 0x00, sizeof(buffer));
}

