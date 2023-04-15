#include <16F690.h>


#device *=16

                                               
#fuses INTRC_IO, PROTECT, PUT, BROWNOUT ,CPD, NOWDT, NOMCLR, FCMEN, NOIESO
                  
                         
#use delay(internal = 4MHz)   
                                                                          
                                            
#include "lcd.c"  
#include "DS1302.c"   
#include "sound.c"
                                     
            
#define set		  input(pin_A0)                                            
#define chg		  input(pin_A1) 
                                                

void setup();            
void dow_disp(unsigned char dow);  
unsigned char get_buttons();
void settings();
unsigned char set_value(unsigned char x, unsigned char y, unsigned char value, unsigned char max, unsigned char min, short type);     
void display_time();


void main()                                                                     
{   	
	setup();                   
	while(TRUE)    
	{                  
	   	
		if(get_buttons() == 1)  
		{   
			while(get_buttons() == 1);
			settings();            
		}                  
		else          
		{
			get_time();
			get_date();
			display_time();
		}
	};                                                                         
}                              
                                              
                                                                                                          
void setup()                     
{                                               
    disable_interrupts(global);
    setup_oscillator(OSC_4MHz);
    setup_WDT(WDT_off);         
    setup_comparator(NC_NC_NC_NC);
    setup_SPI(SPI_disabled | SPI_SS_disabled);         
    setup_adc(ADC_off);                               
    setup_adc_ports(no_analogs);       
    setup_timer_0(T0_internal | T0_8_BIT);        
    setup_timer_1(T1_disabled);    
    setup_timer_2(T2_disabled, 255, 1);
    set_RTCC(0);      
    set_timer1(0);                      
    set_timer2(0);                                                                                                      
    setup_CCP1(CCP_off);
    DS1302_init();                    
    lcd_init();
    lcd_putc("\f");         
}    
                                            

void dow_disp(unsigned char dow) 
{                       
	lcd_gotoxy(3, 2);
	switch(dow) 
	{
		case 1:
		{  
			lcd_putc("SUN");
			break;
		}
		case 2:
		{            
			lcd_putc("MON");
			break;  
		}
		case 3:
		{
			lcd_putc("TUE");
			break;  
		}
		case 4:
		{  
			lcd_putc("WED");
			break;  
		}              
		case 5:
		{      
			lcd_putc("THR");
			break;  
		}
		case 6:
		{        
			lcd_putc("FRI");
			break;  
		}
		case 7: 
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


unsigned char get_buttons()
{
	if(set)
	{
		generate_tone(1900, 20);  
		return 1;
	}  
	else if(chg)
	{
		generate_tone(2900, 20);  
		return 2;                  
	} 
	else
	{
		return 0;
	}
}


void settings()
{
	unsigned char temp1 =  0;    
	unsigned char temp2 =  0;  
	unsigned char temp3 =  0;  
	unsigned char temp4 =  0;                           
	                      
	temp1 = set_value(5, 1, time.hour, 23, 0, 1); 
	delay_ms(200);                              
	temp2 = set_value(8, 1, time.min, 59, 0, 1);
	delay_ms(200); 
	temp3 = set_value(11, 1, time.sec, 59, 0, 1);  
	delay_ms(200); 
	                            
	set_time(temp1, temp2, temp3);  
	        	                          
	temp4 = set_value(3, 2, time.dow, 7, 1, 0); 
	delay_ms(200);          
	temp1 = set_value(7, 2, time.date, 31, 1, 1);
	delay_ms(200);                                           
	temp2 = set_value(10, 2, time.month, 12, 1, 1);  
	delay_ms(200);  
	temp3 = set_value(13, 2, time.year, 99, 0, 1);  
	delay_ms(200);    
   	
	set_date(temp1, temp2, temp3, temp4);      
}                                               

                                       
unsigned char set_value(unsigned char x, unsigned char y, unsigned char value, unsigned char max, unsigned char min, short type)
{
	short tgl = 0;
	while(TRUE)
	{   	
		tgl = ~tgl; 
		delay_ms(90);
		if(get_buttons() == 2)
		{
			value += 1; 
		}
	   	
		if(value > max)
		{  	
			value = min;
		}     
	   	
		switch(type)
		{    
			case 1:
			{
				lcd_gotoxy(x, y); 
				switch(tgl)
				{
					case 1:
					{                                
						printf(lcd_putc, "%02u", value);    
						break;
					}   
					default:             
					{
						lcd_putc("  ");
						break;
					}     
				} 
				break;
			}    
			default:
			{
				switch(tgl)                                
				{
					case 1:
					{
						dow_disp(value); 
						break;                         
					}   
					default:
					{          
						dow_disp(0); 
						break;
					}   
				} 
				break;      
			}
		}     

		if((get_buttons() == 1) && (tgl)) 
		{                   
			return value;  
		}
	}     
}


void display_time()
{
	lcd_gotoxy(5, 1);               
	printf(lcd_putc, "%02u:%02u:%02u ", time.hour, time.min, time.sec);           
	lcd_gotoxy(6, 2);            
	printf(lcd_putc, " %02u.%02u.%02u ", time.date, time.month, time.year); 
	dow_disp(time.dow); 
}


                       
