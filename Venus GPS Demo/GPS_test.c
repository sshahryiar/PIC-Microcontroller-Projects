#include <16F877A.h>
 

#fuses NOWDT, PROTECT, CPD, NOLVP, HS
#fuses NOBROWNOUT, NODEBUG, NOWRT, PUT  


#define TX_pin  				pin_C6 
#define RX_pin					pin_C7     
                                    
#define GPS_buffer_size			60  
#define GPRMC_check_size		6      
#define LED1					pin_D0    
#define LED2					pin_D1
                                                                                         
                                                                                                     
#use delay (clock = 20MHz) 
#use rs232(baud = 9600, UART1, stop = 1, bits = 8, parity = N, timeout= 100)

                                                           
#include "lcd.c"   

                                    
short acquired = FALSE;              
short show_data = FALSE;                                                          
                                 
unsigned char s = 0; 
unsigned char cpos = 0;                        
unsigned char ppos = 0; 
                                                
unsigned char comma_pos[13];                   
unsigned char point_pos[6];                                                                    
unsigned char GPS_check_buffer[GPRMC_check_size];
unsigned char GPS_data_buffer[GPS_buffer_size];


const unsigned char symbol[8] = {0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00};
                                       

                               
                          
void setup();             
void lcd_symbols();                                                   
unsigned char get_time(unsigned char *buffer, unsigned char loc);  
short get_location(unsigned char *buffer, unsigned long *tmp1, unsigned char *tmp2, unsigned char start_pos, unsigned char end_pos);     
unsigned char get_sec(unsigned char *buffer, unsigned char loc);
unsigned char get_directions(unsigned char *buffer, unsigned char loc);
void get_others(unsigned char *buffer, unsigned long *tmp1, unsigned char *tmp2, unsigned char start_pos); 

                                                                                                                                                         
#int_rda
  
void rx_ISR()
{            	
	output_toggle(LED2);
	if(acquired)
	{                                   
		for(s = 0; s < GPS_buffer_size; s++)
		{                           
			GPS_data_buffer[s] = getchar();    
		   	
			if(GPS_data_buffer[s] == ',')                                 
			{                          
				comma_pos[cpos] = s;   	
				cpos++;
			} 
			if(GPS_data_buffer[s] == '.')       
			{                                                
				point_pos[ppos] = s;   	
				ppos++;                
			}       
		 }    
		 
		 show_data = TRUE;
		 acquired = FALSE;                  
	}                                                                                          
	else      
	{                              
		GPS_check_buffer[s] = getchar();
		s++;                     
	   	
		if((GPS_check_buffer[0] == '$') 
		&& (GPS_check_buffer[1] == 'G') 
		&& (GPS_check_buffer[2] == 'P')                        
		&& (GPS_check_buffer[3] == 'R') 
		&& (GPS_check_buffer[4] == 'M') 
		&& (GPS_check_buffer[5] == 'C'))//(GPS_check_buffer == "$GPRMC")   	
		{                                                                
			s = 0;                                                
			memset(GPS_check_buffer, 0x00, GPRMC_check_size);  
			memset(GPS_data_buffer, 0x00, GPS_buffer_size);   
			memset(point_pos, 0x00, 6); 
			memset(comma_pos, 0x00, 13);  
			ppos = 0;
			cpos = 0;
			acquired = TRUE;
		}                     
		                           
		if(s >= GPRMC_check_size)                                               
		{                                                                               
			s = 0;
			memset(GPS_check_buffer, 0x00, GPRMC_check_size);      
		}                                                    
	}     
}       
                                                                                                                    
                                                                                                                        
void main()                                                                    
{                                                                 
	unsigned char hrs = 0;  
	unsigned char min = 0;  
	unsigned char day = 0;
	unsigned char month = 0;
	unsigned char year = 0;
   	
	unsigned long lat_deg = 0;
	unsigned char lat_min = 0;
	unsigned char lat_sec = 0;  
   	
	unsigned long lon_deg = 0;
	unsigned char lon_min = 0;
	unsigned char lon_sec = 0;   
	                              
	unsigned long speed_int = 0; 
	unsigned char speed_fra = 0;
	unsigned long bearing_int  = 0;  
	unsigned char bearing_fra  = 0;     
	                                               
	memset(GPS_check_buffer, 0x00, GPRMC_check_size);
	memset(GPS_data_buffer, 0x00, GPS_buffer_size);   
	                                                
	setup();
	                    
	while(TRUE)                               
	{                                
		if(show_data)                         
		{      
			hrs = get_time(GPS_data_buffer, (1 + comma_pos[0])); 
			min = get_time(GPS_data_buffer, (3 + comma_pos[0]));
		   	
			lat_sec = get_sec(GPS_data_buffer, (point_pos[1]));
			get_location(GPS_data_buffer, &lat_deg, &lat_min, (comma_pos[2]), (point_pos[1]));
			lon_sec = get_sec(GPS_data_buffer, (point_pos[2]));     
			get_location(GPS_data_buffer, &lon_deg, &lon_min, (comma_pos[4]), (point_pos[2]));  
            
            get_others(GPS_data_buffer, &speed_int, &speed_fra, comma_pos[6]); 
            get_others(GPS_data_buffer, &bearing_int, &bearing_fra, comma_pos[7]);
			                 
			day = get_time(GPS_data_buffer, (1 + comma_pos[8])); 
			month = get_time(GPS_data_buffer, (3 + comma_pos[8]));     
			year = get_time(GPS_data_buffer, (5 + comma_pos[8]));   
		   	
			                                                  
			lcd_gotoxy(7, 4);          
			printf(lcd_putc, "%02u:%02u", hrs, min); 
			        
			lcd_gotoxy(13, 4);                                       
			printf(lcd_putc, "%02u/%02u/%02u", day, month, year);  
		   	
			lcd_gotoxy(7, 1);	                                                              
			printf(lcd_putc, "%03lu", lat_deg);
			lcd_gotoxy(11, 1);	                                                              
			printf(lcd_putc, "%02u.%1u'", lat_min, lat_sec);
			lcd_gotoxy(17, 1);                                                       
			printf(lcd_putc, "%1c", get_directions(GPS_data_buffer, comma_pos[3])); 
			               
			lcd_gotoxy(7, 2);	                                                              
			printf(lcd_putc, "%03lu", lon_deg); 
			lcd_gotoxy(11, 2);	                                                              
			printf(lcd_putc, "%02u.%1u'", lon_min, lon_sec);
			lcd_gotoxy(17, 2);                                                       
			printf(lcd_putc, "%1c", get_directions(GPS_data_buffer, comma_pos[5])); 		                                
			                                  
			lcd_gotoxy(7, 3);	                                                              
			printf(lcd_putc, "%03lu.%1u", speed_int, speed_fra);    
			                                                                
			lcd_gotoxy(15, 3);	                                                              
			printf(lcd_putc, "%03lu.%1u", bearing_int, bearing_fra);      
			                                                   
			show_data = FALSE;
		}
		output_toggle(LED1);   
		delay_ms(100);
	};                                             
}                                                 
                                   
                                         
void setup()
{                                                                                               
	disable_interrupts(global);              
	setup_timer_0(T0_internal); 
	setup_timer_1(T1_disabled);
	setup_timer_2(T2_disabled, 255, 1);
	set_timer0(0);
	set_timer1(0);           
	set_timer2(0);                                                           
	setup_CCP1(CCP_off);
	setup_CCP2(CCP_off);
	setup_psp(PSP_disabled);                                
	setup_comparator(NC_NC_NC_NC);       
	setup_SPI(SPI_disabled);                   
	port_B_pullups(FALSE);  
	setup_ADC(ADC_off);                    
	set_analog_pins(no_analogs);
	enable_interrupts(global);  
	enable_interrupts(int_rda);  
	lcd_init();
	lcd_symbols();      	
	lcd_gotoxy(1, 1); 
	lcd_putc("Lat.:");
	lcd_gotoxy(10, 1);                                        
	lcd_putc(0);    
	lcd_gotoxy(1, 2);               
	lcd_putc("Lon.:");
	lcd_gotoxy(10, 2);                                 
	lcd_putc(0);  
	lcd_gotoxy(1, 3);                       
	lcd_putc("kmph:"); 
	lcd_gotoxy(20, 3);                                                
	lcd_putc(0);  
	lcd_gotoxy(1, 4);
	lcd_putc("UTC.: --:--");                     
	lcd_gotoxy(13, 4); 
	lcd_putc("--/--/--");  
}                                                            

                         
void lcd_symbols() 
{
   unsigned char s = 0;                 
                                                        
   lcd_send_byte(0, 0x40); 
   for(s = 0; s < 8; s++)        
   {                           
        lcd_send_byte(1, symbol[s]);          
   }                             
   lcd_send_byte(0, 0x80);   
} 

            
unsigned char get_time(unsigned char *buffer, unsigned char loc)  
{                                                
    unsigned char val;                                                                                                           
	val = ((buffer[loc] - 48) * 10 + (buffer[loc + 1] - 48));
	return val;                                                
}
                            

short get_location(unsigned char *buffer, unsigned long *tmp1, unsigned char *tmp2, unsigned char start_pos, unsigned char end_pos)
{
	unsigned char n = 0;                               
	unsigned char no_of_chr = 0; 
	unsigned char multiplier = 0;  
   	
	unsigned long val = 0;  
	                                                    
	for(n = start_pos; n < end_pos; n++)
	{
	    if((buffer[n] >= '0') && buffer[n] <= '9')
	    {
	    	no_of_chr++;                       
	    }     
	}  
   	
	if(no_of_chr == 5)
	{            
		multiplier = 100;
		for(n = (start_pos + 1); n < (start_pos + 4); n++)
		{ 
			val += ((buffer[n] - 48) * multiplier);
			multiplier /= 10;
		}  
		*tmp1 = val;  
	   	
		multiplier = 10;
		val = 0;
		for(n = (start_pos + 4); n < (start_pos + 6); n++)
		{                           
			val += ((buffer[n] - 48) * multiplier);
			multiplier /= 10;
		}   
		*tmp2 = val;       
		return 1;
	}               
	else if(no_of_chr == 4)
	{                               
		multiplier = 10;
		for(n = (start_pos + 1); n < (start_pos + 3); n++)
		{                                   
			val += ((buffer[n] - 48) * multiplier);
			multiplier /= 10;                     
		}       
		*tmp1 = val;                      
	   	
		multiplier = 10;
		val = 0;
		for(n = (start_pos + 3); n < (start_pos + 5); n++)
		{ 
			val += ((buffer[n] - 48) * multiplier);
			multiplier /= 10;              
		}  
		*tmp2 = val; 
		return 1;
	}
	else
	{
		return 0;
	}                                               
}
                                                            

unsigned char get_sec(unsigned char *buffer, unsigned char loc)
{                          
	return (buffer[loc + 1] - 48);    
}
   

unsigned char get_directions(unsigned char *buffer, unsigned char loc)
{
	return (buffer[loc + 1]);	                                                  
} 


void get_others(unsigned char *buffer, unsigned long *tmp1, unsigned char *tmp2, unsigned char start_pos)
{    
	unsigned char n = 0;   
	unsigned char multiplier = 100;   
   	
	unsigned long val = 0;           
	        	      
	for(n = (start_pos + 1); n < (start_pos + 4); n++)
	{ 
		val += ((buffer[n] - 48) * multiplier);
		multiplier /= 10;
	}  
	*tmp1 = val; 
   	
	*tmp2 = (buffer[start_pos + 5] - 48);
}
                                                                                          
