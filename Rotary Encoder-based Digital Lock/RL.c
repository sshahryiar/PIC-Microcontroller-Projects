#include <16F690.h>


#device *= 16
           
                                                                   
#fuses INTRC_IO, PUT, NOWDT, FCMEN, CPD, PROTECT, NOIESO, BROWNOUT, NOMCLR


#use delay (internal = 4MHz)

                                                                            
#include "lcd.c" 
#include "sound.c"


#define encoder_A       !input(pin_B4)     
#define encoder_B       !input(pin_B5)
#define mid_button      !input(pin_B6)  
                         
#define lock_LED		pin_C3
#define unlock_LED		pin_C4
#define solenoid		pin_C5           

#define pass_entry      0x01 
#define pass_change		0x02             
#define new_pass        0x03
#define lock_mode		0x00               
#define unlock_mode     0x09          

                                 
#ROM 0x00002100 = {1}             
#ROM 0x00002101 = {1}              
#ROM 0x00002102 = {0}                     
#ROM 0x00002103 = {0} 

                         
const unsigned char symbols[32]= 
{   
    0x0E, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x0E, 0x00,
    0x0E, 0x11, 0x01, 0x1F, 0x1F, 0x1F, 0x0E, 0x00,
    0x07, 0x04, 0x06, 0x04, 0x0E, 0x0A, 0x0E, 0x00,
    0x00, 0x04, 0x08, 0x1F, 0x08, 0x04, 0x00, 0x00                 
};                                             
                                            
                              
void setup();                            
unsigned char enc_decode();
void lcd_symbols();  
unsigned long read_password();
void save_password(unsigned long key);
unsigned long get_password(unsigned char type);                               
short check_password(unsigned long key1, unsigned long key2);  
                                                            

void main()
{                                 
	short chk = 0;
	short chg = 0;                                                 
	                   
	unsigned char state = 0; 
   	
	unsigned long tmp1 = 0;        
	unsigned long tmp2 = 0;              
	                                     
	setup();                                             
	                        
	while(TRUE)                 
	{                                       
		switch(state)          
		{                                                    
		    case pass_entry:                                        
		    {                                                                              
    			tmp1 = read_password(); 
    			tmp2 = get_password(0); ;       
    			                                  
    			chk = check_password(tmp1, tmp2);  
    		   	
    			lcd_putc("\f");                              
    			switch(chk)
    			{                                       
    				case 1:               
    				{        
    					lcd_gotoxy(5, 1);
    					lcd_putc("Password");    
    					lcd_gotoxy(5, 2);      
    					lcd_putc("Accepted"); 
    				   	
    					state = unlock_mode;
    					if(chg)
    					{
    						state = new_pass;
    					}  
    				   	
    					break;                         
    				}                         
    				default:     
    				{                                  
    					lcd_gotoxy(5, 1);
    					lcd_putc("Incorrect");    
    					lcd_gotoxy(5, 2);      
    					lcd_putc("Password!"); 
    				   	
    					chg = FALSE;
    					state = lock_mode;
    					break;
    				}                             
    			}   
    			delay_ms(2000);  
    			lcd_putc("\f");
    		   	
    			break;             
			}    
		   	
			case pass_change:
			{       
				lcd_gotoxy(1, 1); 
				lcd_putc("Password Change"); 
				lcd_gotoxy(1, 2);
				lcd_putc("Command...."); 
				delay_ms(2000);                                  
				state = pass_entry;
				break;
			}   
		   	
			case new_pass:
			{                         
				chg = FALSE; 
			   	
				tmp1 = get_password(1); 
				tmp2 = get_password(2);  
                
                chk = check_password(tmp1, tmp2);
    			lcd_putc("\f");                              
    			if(chk)
    			{
    				save_password(tmp1);
    				lcd_gotoxy(1, 1); 
					lcd_putc("Password Changed"); 
					lcd_gotoxy(1, 2);
					lcd_putc("Successfully.");  
    			}  
    			else
    			{   
    				lcd_gotoxy(3, 1);  
					lcd_putc("Password Not"); 
					lcd_gotoxy(5, 2);
					lcd_putc("Changed!");  
    			}  
    			delay_ms(2000);
				lcd_putc("\f");  
				state = lock_mode;
				break;      
			}
		   	
		    case unlock_mode:         
			{   
				output_low(lock_LED);
				output_high(unlock_LED);   
				output_high(solenoid);  
				lcd_gotoxy(4,1);  
        		lcd_putc("UnLocked");
    			lcd_gotoxy(13,1);          
        		lcd_putc(1);    
        	   	
        		if(mid_button)  
				{
					delay_ms(900);
					generate_tone(2900, 10);
					lcd_putc("\f");   
				   	
					if(mid_button) 
					{                
						state = pass_change;
						chg = TRUE;
					}  
					else
					{
						state = lock_mode;     
					}            
				}    
        	   	
				break;             
			}    
		   	
			case lock_mode:
			{
				output_high(lock_LED); 
				output_low(unlock_LED);   
				output_low(solenoid); 
				lcd_gotoxy(5,1);
        		lcd_putc("Locked");
        		lcd_gotoxy(12,1);   
        		lcd_putc(0);   
        	   	
        		if(mid_button)  
				{
					generate_tone(2900, 10);
					while(mid_button);
					lcd_putc("\f");
					state = 1;
				}       
				                        
				break;
			}
		}
	};                                
}                           


void setup()
{                                                                
	disable_interrupts(global);   
	setup_oscillator(OSC_4Mhz);  
	setup_WDT(WDT_off);  
	port_A_pullups(TRUE);  
	setup_adc(ADC_off);             
	setup_comparator(NC_NC_NC_NC);
	setup_ADC_ports(no_analogs);                    
	setup_SPI(SPI_disabled);
	setup_timer_0(T0_internal); 
	setup_timer_1(T1_disabled); 
	setup_timer_2(T2_disabled, 255, 1);
	set_timer0(0);
	set_timer1(0);   
	set_timer2(0);
	setup_CCP1(CCP_off);  
	lcd_init();      
	lcd_symbols(); 
	lcd_putc("\f");  
}                                                      


unsigned char enc_decode() 
{
     static short previous_A;   
     static short present_A;
     static signed char n;
                                                                    
     present_A = encoder_A;
     
     if((!previous_A) && (present_A))
     {                                                                     
         if(encoder_B)
         {                  
             generate_tone(1600, 4);  
             n++;
             if(n > 0x0A)
             {        
                n = 0x00;
             }        
         }                
         else        
         {
             generate_tone(1400, 4);  
             n--;
             if(n < 0x00)  
             {             
                n = 0x0A;
             }
         }                                           
     }
     previous_A = present_A; 
     
     return n;
}
        

void lcd_symbols()
{
   unsigned char s = 0;
   
   lcd_send_byte(0, 0x40);      
   
   for(s = 0; s < 32; s++)
   {                                           
        lcd_send_byte(1, symbols[s]);
   }
   
   lcd_send_byte(0, 0x80);
}                             
           

unsigned long read_password()           
{                          
	unsigned char s = 0; 
	unsigned long temp = 0; 
	unsigned long value = 0;  
	unsigned long multiplier = 1;   

	while(s < 4)        
	{                                           
        temp = read_eeprom((s + 0x00002100));  
        delay_ms(40);        
        temp *= multiplier;
        value += temp;
        multiplier *= 10; 
        s++;
    }                      
	        	                     
	return value; 
}                   

                       
void save_password(unsigned long key)
{                     
	signed char s = 3;          
	unsigned char tmp = 0;  
	unsigned long divisor = 1000;        

	while(s > -1)
	{              
		tmp = ((key / divisor) % 10);
		write_eeprom((0x00002100 + s), tmp);  
		delay_ms(40);
		divisor /= 10;          
		s--;                                 
	}                                
}                                
                                        
                          
unsigned long get_password(unsigned char type)  
{                                   
	unsigned char s = 0;    
	unsigned long tmp = 0;
	unsigned long temp = 0;
	unsigned long multiplier = 1;  
	unsigned char value[4];  
	                                      
	memset(value, 0xFF, sizeof(value));
   	
	lcd_putc("\f"); 
	switch(type)
	{
		case 1:       
		{
			lcd_gotoxy(2, 1);                                     
    		lcd_putc("Enter New Code");    
    		break;
		}   
		case 2:
		{                                                                 
		    lcd_gotoxy(1, 1);                                     
    		lcd_putc("Confirm New Code");   
    		break;
		}
		default:
		{  
			lcd_gotoxy(1, 1);                                     
    		lcd_putc("Enter Password"); 
    		lcd_gotoxy(16, 1);
    		lcd_putc(2); 
    		break; 
		}
	}
                               
    while(s < 4)                                   
    {
    	tmp = enc_decode(); 
    	                         
    	if((tmp >= 0) && (tmp <= 9))  
    	{                            
    		lcd_gotoxy((s + 7), 2);      
    		printf(lcd_putc, "%lu", tmp);
    	}                   
    	else                            
    	{
    		lcd_gotoxy((s + 7), 2);  
    		lcd_putc(3);
    	}                      
       	
    	if(mid_button)                
		{                        
			generate_tone(2900, 10);  
			while(mid_button);
			if(tmp == 0x0A)
			{
				lcd_gotoxy((s + 7), 2);    	
				lcd_putc(" ");    
				s--;
				if(s <= 0)
				{                  
					s = 0;
				}
			}                        
			else                           
			{ 
				lcd_gotoxy((s + 7), 2);
				lcd_putc("*");           
				value[s] = tmp;          
				s++;
			}                                
		}       
    }       
    
    s = 0;
    tmp = 0;
    while(s < 4)
    {
    	tmp = value[s];
    	tmp = (tmp * multiplier);
    	multiplier *= 10;      
    	temp += tmp;   
    	s++;
    }                       
                              
    return temp;        
}                                    

                                                                    
short check_password(unsigned long key1, unsigned long key2)                                         
{	      
    if(key1 != key2)
    {
       generate_tone(2800, 40);   
       return 0;
    }                                                
    else
    {
       generate_tone(1600, 40);  
       return 1;
    }
}                     
                                                                                    
               
