#include <16F877A.h>                                      


#device *= 16      


#fuses HS, PUT, PROTECT, CPD, NODEBUG, NOLVP, NOWDT, NOWRT, BROWNOUT 


#use delay (clock = 10MHz)    

                
#include "keypad_display.c" 


#define no_op				0x0E
#define add					0x0A  
#define sub					0x0B
#define mul					0x0C
#define div					0x0D


unsigned char operation = 0x00;  

                              
void setup();  
unsigned long long get_number(short num_sel); 
void show_value(signed long long  value);  
unsigned long long power(unsigned char value);

                                
void main()                                                                           
{                                            
	signed long long num1 = 0;                                                                      
	signed long long num2 = 0;                     
	                            
	setup();                        
	                   
	while(TRUE)                                                                                                         
	{                                                                                                                                    
		 while(key != 0x0F);     
		 
		 num1 = get_number(TRUE);
		 show_value(num1);      
		 delay_ms(1000);
		                       
		 num2 = get_number(FALSE);      
		 show_value(num2);
		 delay_ms(1000);
		                   
		 switch(operation)
		 {
		    case add:
		    {    
		    	show_value((num1 + num2)); 
		    	break;
		    } 
		    case sub:
		    {                     
		    	show_value((num1 - num2)); 
		    	break;
		    }    
		    case mul:
		    {                    
		    	show_value((num1 * num2)); 
		    	break;
		    }                   
		    case div:
		    {    
		    	show_value((num1 / num2)); 
		    	break;
		    } 
		    case no_op:
		    {    
		    	show_value(19999999); 
		    	break;
		    } 
		 }      
	};                              
}                                               
                                                                                            

void setup()
{
	disable_interrupts(GLOBAL);
	setup_timer_1(T1_disabled);
	setup_timer_2(T2_disabled, 255, 1);
	set_timer1(0x0000);
	set_timer2(0x00);
	setup_CCP1(CCP_off);
	setup_CCP2(CCP_off);                     
	display_keypad_init();
   	
	delay_ms(100);     
}                            
                          

unsigned long long get_number(short num_sel)
{     
	unsigned char n = 0x00; 
	unsigned char digits = 0x00;
   	
	unsigned long long number = 0;
   	
	unsigned char value[8];

	memset(value, 0x00, sizeof(value));
	clear_buffer();                    
   	
	while(TRUE)
	{   	
		if(key_pressed)
		{                                                                                                   
		    if((key >= 0) && (key <= 9))
		    {                             
		        buffer[n] = num[key]; 
		    	value[n] = key;
		    	n++; 
		    	digits++;
		    }                                     
		    delay_ms(400);                                            
		    key_pressed = FALSE;
		    
		    if((n >= 7) || ((key >= 0x0A) && (key <= 0x0E)))                                                                                                      
		    {
                if(num_sel == TRUE)                
                {
                	operation = key;  
                }       
		        break;                                  
		    }                               
		} 
	};                                                                                                                         
	    
    for(n = 1; n <= digits; n++)
    {                     
    	number += value[(n - 1)] * power(digits - n);  
    }   
                
    return number;               
}                 

                   
void show_value(signed long long value)
{
	unsigned char ch = 0;
   	
	clear_buffer();
   	
	if(value < 0)
	{
		value = -value;
		buffer[0] = 0x40;
	}
   	
	if((value >= 0) && (value <= 9))
	{
	    ch = value;             
	    buffer[7] = num[ch];
	} 
   	
	else if((value > 9) && (value <= 99))
	{                                 
	    ch = (value / 10);             
	    buffer[6] = num[ch];  
	    ch = (value % 10);             
	    buffer[7] = num[ch]; 
	}  
   	
	else if((value > 99) && (value <= 999))
	{                                 
	    ch = (value / 100);             
	    buffer[5] = num[ch];  
	    ch = ((value / 10) % 10);             
	    buffer[6] = num[ch];    
	    ch = (value % 10);                                   
	    buffer[7] = num[ch]; 
	}       
   	
	else if((value > 999) && (value <= 9999))
	{                                 
	    ch = (value / 1000);             
	    buffer[4] = num[ch];       
	    ch = ((value / 100) % 10);   
	    buffer[5] = num[ch];       
	    ch = ((value / 10) % 10);             
	    buffer[6] = num[ch];    
	    ch = (value % 10);             
	    buffer[7] = num[ch];   
	}   
   	
	else if((value > 9999) && (value <= 99999))
	{                                   
	    ch = (value / 10000);             
	    buffer[3] = num[ch];   
	    ch = ((value / 1000) % 10);   
	    buffer[4] = num[ch];   
	    ch = ((value / 100) % 10);   
	    buffer[5] = num[ch];       
	    ch = ((value / 10) % 10);             
	    buffer[6] = num[ch];                         
	    ch = (value % 10);             
	    buffer[7] = num[ch];   
	} 
   	
	else if((value > 99999) && (value <= 999999))
	{                                   
	    ch = (value / 100000);             
	    buffer[2] = num[ch]; 
	    ch = ((value / 10000) % 10);   
	    buffer[3] = num[ch];     
	    ch = ((value / 1000) % 10);   
	    buffer[4] = num[ch];   
	    ch = ((value / 100) % 10);   
	    buffer[5] = num[ch];             
	    ch = ((value / 10) % 10);             
	    buffer[6] = num[ch];                         
	    ch = (value % 10);             
	    buffer[7] = num[ch];   
	} 
   	
	else if((value > 999999) && (value <= 9999999))
	{                                   
	    ch = (value / 1000000);             
	    buffer[1] = num[ch];
	    ch = ((value / 100000) % 10);   
	    buffer[2] = num[ch]; 
	    ch = ((value / 10000) % 10);   
	    buffer[3] = num[ch];   
	    ch = ((value / 1000) % 10);   
	    buffer[4] = num[ch];   
	    ch = ((value / 100) % 10);   
	    buffer[5] = num[ch];             
	    ch = ((value / 10) % 10);             
	    buffer[6] = num[ch];                         
	    ch = (value % 10);             
	    buffer[7] = num[ch];   
	} 
   	
	else
	{
		memset(buffer, 0x40, sizeof(buffer));               
	}                           
}


unsigned long long power(unsigned char value)
{
	unsigned char p = 0;
	unsigned long long temp = 1;
	                      
	for(p = 0; p < value; p++)
	{                         
		temp *= 10; 
	}                          
	  
	return temp;
}                              

                   
