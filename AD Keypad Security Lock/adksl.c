#include <16F690.h>


#device *= 16
#device ADC = 10


#fuses NOWDT, INTRC_IO, PROTECT, PUT, CPD
#fuses NOBROWNOUT, NOMCLR, NOIESO, FCMEN

                                                      
#use delay (internal = 4MHz)


#include "lcd.c" 


#define lock_LED		pin_C0   
#define unlock_LED		pin_C1
#define solenoid		pin_C2           
                                    
#define pass_entry      0x01 
#define pass_change		0x02             
#define new_pass        0x03
#define lock_mode		0x00               
#define unlock_mode     0x09  

//Ideal readout

#define k1_min		0
#define k1_max		6  
#define k2_min		40
#define k2_max		80  
#define k3_min		110
#define k3_max		140 
#define k4_min		170
#define k4_max		210  
#define k5_min		240 
#define k5_max		270  
#define k6_min		300   
#define k6_max		340  
#define k7_min		360
#define k7_max		400  
#define k8_min		420
#define k8_max		470  
#define k9_min		500
#define k9_max		530 
#define k10_min		550
#define k10_max		590  
#define k11_min		620 
#define k11_max		660  
#define k12_min		690 
#define k12_max		716  
#define k13_min		740
#define k13_max		790  
#define k14_min		820
#define k14_max		850      
#define k15_min		880
#define k15_max		910 
#define k16_min		940
#define k16_max		980

/*actual readout

#define k1_min		0
#define k1_max		6  
#define k2_min		250
#define k2_max		290  
#define k3_min		400
#define k3_max		470 
#define k4_min		520
#define k4_max		580  
#define k5_min		600 
#define k5_max		660  
#define k6_min		680   
#define k6_max		710  
#define k7_min		720
#define k7_max		750  
#define k8_min		760
#define k8_max		790  
#define k9_min		800
#define k9_max		816 
#define k10_min		826
#define k10_max		844  
#define k11_min		856 
#define k11_max		864  
#define k12_min		869 
#define k12_max		880  
#define k13_min		890
#define k13_max		900  
#define k14_min		906
#define k14_max		916  
#define k15_min		919
#define k15_max		926 
#define k16_min		929
#define k16_max		939 
*/


#ROM 0x00002100 = {1}             
#ROM 0x00002101 = {2}              
#ROM 0x00002102 = {3}                     
#ROM 0x00002103 = {4}  


const unsigned char symbols[32]= 
{   
    0x0E, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x0E, 0x00,
    0x0E, 0x11, 0x01, 0x1F, 0x1F, 0x1F, 0x0E, 0x00,
    0x07, 0x04, 0x06, 0x04, 0x0E, 0x0A, 0x0E, 0x00                
};     


void setup();    
void lcd_symbols();     
unsigned long read_password();           
void save_password(unsigned long key);  
unsigned long adc_avg();
unsigned char get_key();       
unsigned long get_password(unsigned char type);                               
short check_password(unsigned long key1, unsigned long key2); 
                                  
                                
void main()
{                                                          
	short chk = 0;
	short chg = 0;        
   	
	unsigned char state = 0;
	               
	unsigned char tmp1 = 0x00;        
	unsigned char tmp2 = 0x00; 
	                                                                              
	setup();
	                       
	while(TRUE)        
	{                   
		switch(state)          
		{                                                    
		    case pass_entry:                                        
		    {                                                                              
    			tmp1 = read_password(); 
    			tmp2 = get_password(0);       
    			                                  
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
        	   	
        		if(get_key() == 0x0E)  
				{
					delay_ms(900);
					lcd_putc("\f");   
					         
					if(get_key() == 0x0E)  
					{                
						state = pass_change;
						chg = TRUE;
					}           
				}   
			   	
				if(get_key() == 0x0D)  
				{
					delay_ms(600);
					lcd_putc("\f");  
				   	
					if(get_key() == 0x0D)  
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
        	   	
        		if(get_key() == 0x0F)  
				{
					while(get_key() == 0x0F);
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
	disable_interrupts(GLOBAL);                           
	setup_WDT(WDT_off);
	setup_oscillator(OSC_4MHz);
	setup_comparator(NC_NC_NC_NC);
	setup_SPI(SPI_disabled | SPI_SS_disabled);
	setup_ADC(ADC_clock_div_8);
	setup_ADC_ports(sAN0);
	set_ADC_channel(0);  
	port_B_pullups(FALSE);                                 
	setup_CCP1(CCP_off);
	setup_timer_0(T0_internal);
	setup_timer_1(T1_disabled);      
	setup_timer_2(T2_disabled, 255, 1);
	set_timer0(0);       
	set_timer1(0);  
	set_timer2(0);    
	lcd_init();
	lcd_putc("\f"); 
	lcd_symbols();
}    


void lcd_symbols()
{
   unsigned char s = 0;
   
   lcd_send_byte(0, 0x40);      
   
   for(s = 0; s < 24; s++)
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


unsigned long adc_avg()
{                 
	unsigned char samples = 64;
	unsigned long avg = 0;  
   	
	while(samples > 0) 
	{
	    read_adc(adc_start_only); 
	    while(!adc_done());
	    avg += read_adc(adc_read_only);      
	    samples--;           
	}        
	avg >>= 6;                                          

	return avg;
}          



                       

unsigned char get_key()
{
	unsigned long key = 0; 
   	
	key = adc_avg();
	delay_ms(160);
	                          
	if((key >= k1_min) && (key <= k1_max))   
	{
		key = 0x00;  
	}                        
	else if((key >= k2_min) && (key <= k2_max))   
	{          
		key = 0x01;
	}  
	else if((key >= k3_min) && (key <= k3_max))   
	{             
		key = 0x02;
	}               
	else if((key >= k4_min) && (key <= k4_max))   
	{          
		key = 0x03;
	}        
	else if((key >= k5_min) && (key <= k5_max))   
	{          
		key = 0x04;                 
	}                                                                                                                                                
	else if((key >= k6_min) && (key <= k6_max))   
	{                                                                                                                                                    
		key = 0x05;
	}              
	else if((key >= k7_min) && (key <= k7_max))   
	{                               
		key = 0x06;                                                                                                            
	} 
	else if((key >= k8_min) && (key <= k8_max))                                                                                    
	{                                                  
		key = 0x07;             
	}  
	else if((key >= k9_min) && (key <= k9_max))   
	{                                                                                                                                                                            
		key = 0x08;             
	}                                     
	else if((key >= k10_min) && (key <= k10_max))   
	{             
		key = 0x09;
	}
	else if((key >= k11_min) && (key <= k11_max))   
	{             
		key = 0x0A;
	}                        
	else if((key >= k12_min) && (key <= k12_max))   
	{             
		key = 0x0B;
	}                                                                  
	else if((key >= k13_min) && (key <= k13_max))   
	{             
		key = 0x0C;
	} 
	else if((key >= k14_min) && (key <= k14_max))   
	{                                     
		key = 0x0D;                    
	}  
	else if((key >= k15_min) && (key <= k15_max))   
	{                               
		key = 0x0E; 
	}                                                                                         
	else if((key >= k16_min) && (key <= k16_max))   
	{             
		key = 0x0F;
	} 
	else    							//Key > 940  
	{
		key = 0xFF;
	}
   	
	return key;
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
		while(TRUE)
		{
			value[s] = get_key();          
			if((value[s] >= 0) && (value[s] <= 9))  
	    	{                            
	    		lcd_gotoxy((s + 7), 2);             
	    		printf(lcd_putc, "%u", value[s]); 
	    		delay_ms(90);
	    		lcd_gotoxy((s + 7), 2);      
	    		lcd_putc("*");
	    		break;
	    	} 
		}          
		s++;
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
       return 0;
    }                                                
    else            
    {                           
       return 1;
    }
}                     
                                                                                    
      
