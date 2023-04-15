#include <16F877A.h>           
                      

#device *= 16
#device ADC = 10 


#fuses NOWDT, XT, PROTECT, CPD, NOWRT, NOLVP, BROWNOUT, NODEBUG, PUT

                                                          
#use delay (clock = 4MHz)  
#use fast_io(C)   

 
#include "lcd.c"   
#include "sound.c"
#include "keypad.c"                                      
#include "PCF8563.c"                
                          
                                                                   
void setup();
void startup_check(); 
void show_day(unsigned char value);       
unsigned char make_number(unsigned char digit_10, unsigned char digit_1);  
void show_time_date();
void show_alarm(); 
void show_alam_state(unsigned char state);
unsigned char set_alarm();
unsigned char set_dow();
unsigned char set_value(unsigned char x_pos, unsigned char y_pos, unsigned char value_max, unsigned char value_min); 
                                                                         
                                           
void main()
{	                      
	short disp = 0;   
	short alarm = 0;
   	
	unsigned char menu = 0; 
	unsigned char settings = 0;   
     
	setup();                           
	startup_check();
   	
	                                        
	while(TRUE)  
	{                                                    
		if(read_keypad() == 0x0D)  
		{    
			disp = ~disp;
			delay_ms(100);
		} 
	   	
		if(read_keypad() == 0x0A)
		{
			if(alarm)
			{
				alarm = 0;
			    clear_alarm();
			}
		}  
		                 
		if(read_keypad() == 0x0E)
		{                                     
			PCF8563_stop_RTC();  
			settings = 1;      
			menu = 1; 
			disp = 0;           
			delay_ms(100);   
		}                  
	   	
		if(read_keypad() == 0x0F)         
		{                   
			PCF8563_stop_RTC();    	
			settings = 2;
			disp = 1;   
			delay_ms(100);
		}         
	   	
		switch(disp)
		{
			case 1:
			{
				show_alarm();
				break;
			}
			default:                
			{         
				show_time_date();
				break;
			}
		}   
	   	
		if(settings == 1) 
		{        
			hr = set_value(3, 1, 23, 0);  
			min = set_value(6, 1, 59, 0); 
			sec = set_value(9, 1, 59, 0);
		   	
			dt = set_value(4, 2, 31, 1);  
			mt = set_value(7, 2, 12, 1);
			yr = set_value(12, 2, 99, 0);
			wk = set_dow();
			                
			write_time();                      
			write_date();  
		   	
			PCF8563_start_RTC();    
			settings = 0;
		} 
		                                           
		else if(settings == 2)
		{       
			al_hr = set_value(4, 2, 23, 0);  
			al_min = set_value(7, 2, 59, 0);   
			al_state = set_alarm();
		   	
		    write_al_time();  
			                    
			PCF8563_start_RTC();
			delay_ms(4000);
			disp = 0;         
			settings = 0;
		}                                
		    
		else                
		{                                           
			read_time();                             
			read_date();           
			read_al_time();
			alarm = check_alarm(); 
		   	
			if(alarm)                                   
			{                               
				generate_tone(2400, 60);    
				generate_tone(3200, 60); 
			}
			else
			{
				output_low(TONE_PIN);
			}
		} 
		    
		delay_ms(400);     
	};                                    
}                                                                            
                                    
                                                                                             
void setup()
{                              
	disable_interrupts(GLOBAL); 
	set_TRIS_C(0xF0);	    
	setup_ADC(ADC_off); 
	setup_CCP1(CCP_off); 
	setup_CCP2(CCP_off); 
	setup_ADC_ports(no_analogs);                                                  
	setup_comparator(NC_NC_NC_NC);              
	setup_SPI(SPI_disabled | SPI_SS_disabled); 
	setup_timer_0(T0_internal | T0_8_bit);  
	setup_timer_1(T1_disabled);             
	setup_timer_2(T2_disabled, 0xFF, 0x01);      
	set_timer0(0x00);                     
	set_timer1(0x0000);
	set_timer2(0x00); 
	lcd_init();  
	PCF8563_init();
	lcd_putc("\f"); 
}


void startup_check()
{
	read_time();
	read_date();           
	read_al_time();  
	                
	if((min > 59) || (hr > 23))
	{     
		sec = 10; 
		min = 10;                                 
		hr = 10;
		write_time();
	}         
	                                                
	if((dt > 31) || (wk > 6) || (mt > 12) || (yr > 99))
	{                         
		dt = 1;
		wk = 1;
		mt = 1;
		yr = 1;
		write_date();
	}                              
   	
	if((min > 59) || (hr > 23))
	{     
		al_min = 10;
		al_min = 10;
		al_state = 0;           
		write_al_time();
	} 
   	
	PCF8563_start_RTC();  
}  


void show_day(unsigned char value)
{
	lcd_gotoxy(12, 1);
   	
	switch(value)
	{                               
		case 0: 
		{              
			lcd_putc("SUN");
			break;
		}   
		case 1:
		{
			lcd_putc("MON");
			break;
		}     
		case 2:
		{
			lcd_putc("TUE");
			break;
		}               
		case 3:
		{
			lcd_putc("WED");
			break;
		}               
		case 4:
		{
			lcd_putc("THR");
			break;
		}
		case 5:
		{
			lcd_putc("FRI");
			break;
		}    
		case 6:
		{           
			lcd_putc("SAT");
			break;
		} 
		default:
		{             
			lcd_putc("   ");
			break;
		} 
	}
} 


unsigned char make_number(unsigned char digit_10, unsigned char digit_1)
{
	return ((digit_10 * 10) + digit_1);
}


void show_time_date()
{   
	lcd_gotoxy(3, 1);
	printf(lcd_putc, "%02u %02u %02u ", hr, min, sec);  
	show_day(wk);                              
	lcd_gotoxy(4, 2);                               
	printf(lcd_putc, "%02u/%02u/20%02u", dt, mt, yr); 
	delay_ms(400);
	lcd_gotoxy(5, 1);
	lcd_putc(":");
	lcd_gotoxy(8, 1);
	lcd_putc(":");    
}


void show_alarm()            
{      
	lcd_gotoxy(3, 1);
	lcd_putc(" ALARM TIME     ");  
	lcd_gotoxy(4, 2);                  
	printf(lcd_putc, "%02u:%02u  ", al_hr, al_min);       
	show_alam_state(al_state);
}    


void show_alam_state(unsigned char state) 
{
	lcd_gotoxy(11, 2);
	if(state == alarm_on)
	{           
		lcd_putc("ON ");
	}
	else
	{
		lcd_putc("OFF");
	}    
}


unsigned char set_alarm()
{     
	unsigned char temp = 0;                  
   	
	do               
	{     
	   if(read_keypad() >= alarm_on && read_keypad() <= alarm_off)  
	   {          
	      temp = read_keypad();                  
	   }      
	   
	   lcd_gotoxy(11, 2);
	   lcd_putc("   ");     
	   delay_ms(100);               
	   show_alam_state(temp);
	   delay_ms(100);
	                      
	}while(read_keypad() != 0x0C); 
   	
	return temp;  
}  


unsigned char set_dow()
{  
	unsigned char temp = 0;                  
   	
	do               
	{     
	   if(read_keypad() >= 0 && read_keypad() <= 6)  
	   {          
	      temp = read_keypad();                  
	   }      
	                             
	   lcd_gotoxy(12, 1);
	   lcd_putc("   ");     
	   delay_ms(100);               
	   show_day(temp);  
	   delay_ms(100);
	            
	}while(read_keypad() != 0x0C); 
   	
	return temp;  
}

                                 
unsigned char set_value(unsigned char x_pos, unsigned char y_pos, unsigned char value_max, unsigned char value_min)  
{                
	unsigned char temp = 0; 
	unsigned char temp1 = 0; 
	unsigned char temp2 = 0;
   	
	do               
	{     
	   if(read_keypad() >= 0 && read_keypad() <= 9)  
	   {                                                     
	      temp1 = read_keypad(); 
	   }      
	   
	   lcd_gotoxy(x_pos, y_pos);
	   lcd_putc(" ");     
	   delay_ms(100);               
	   lcd_gotoxy(x_pos, y_pos);                                   
	   printf(lcd_putc, "%1u", temp1); 
	   delay_ms(100);     
	                               
	}while(read_keypad() != 0x0C);                             
	                               
	do               
	{         
	   if(read_keypad() >= 0 && read_keypad() <= 9)  
	   {
	      temp2 = read_keypad(); 
	   }      
	   
	   lcd_gotoxy((x_pos + 1), y_pos);    
	   lcd_putc(" ");             
	   delay_ms(100);                          
	   lcd_gotoxy((x_pos + 1), y_pos);                 
	   printf(lcd_putc, "%1u", temp2);                 
	   delay_ms(100);   
	   
	}while(read_keypad() != 0x0C);  
   	
	temp = make_number(temp1, temp2);
   	
	if((temp > value_max) || (temp < value_min)) 
	{   
		return 1;
	}
	else
	{
		return temp;
	}
}                          
