#include "keypad.h"
                    
    
unsigned char read_keypad()
{               	                 
	output_low(COL1); 
	output_high(COL2);
	output_high(COL3);
	output_high(COL4);
   	
	if(ROW1)
	{               
	    generate_tone(2600, 10);      
	    return 0x01;
	}      
   	
	else if(ROW2)
	{ 
	    generate_tone(2600, 10);      
	    return 0x04;
	}     
   	
	else if(ROW3)
	{           
	    generate_tone(2600, 10);      
	    return 0x07;
	}   
	            
	else if(ROW4)
	{ 
	    generate_tone(2600, 10);      
	    return 0x0E;
	}     	
   	
	output_low(COL2); 
	output_high(COL1);
	output_high(COL3);
	output_high(COL4);
   	
	if(ROW1)
	{ 
	    generate_tone(2600, 10);      
	    return 0x02;
	}      
   	
	else if(ROW2)
	{ 
	    generate_tone(2600, 10);      
	    return 0x05;
	}             
   	
	else if(ROW3)
	{           
	    generate_tone(2600, 10);      
	    return 0x08;
	} 
	            
	else if(ROW4)
	{ 
	    generate_tone(2600, 10);      
	    return 0x00;
	}         	
	              
	output_low(COL3); 
	output_high(COL1);
	output_high(COL2);
	output_high(COL4);
   	
	if(ROW1)
	{   
	    generate_tone(2600, 10);      
	    return 0x03;
	}      
   	
	else if(ROW2)
	{ 
	    generate_tone(2600, 10);      
	    return 0x06;
	}     
   	
	else if(ROW3)
	{           
	    generate_tone(2600, 10);      
	    return 0x09;
	}         
	            
	else if(ROW4)
	{ 
	    generate_tone(2600, 10);      
	    return 0x0F;
	}        	
   	
	output_low(COL4); 
	output_high(COL1);
	output_high(COL2);
	output_high(COL3);
   	
	if(ROW1)
	{ 
	    generate_tone(2600, 10);      
	    return 0x0A;
	}      
   	
	else if(ROW2)
	{ 
	    generate_tone(2600, 10);      
	    return 0x0B;
	}     
   	
	else if(ROW3)
	{           
	    generate_tone(2600, 10);      
	    return 0x0C;
	} 
	            
	else if(ROW4)
	{ 
	    generate_tone(2600, 10);      
	    return 0x0D;
	}
	else                      
	{      	
		return 0xFF;
	}        
}                                    
