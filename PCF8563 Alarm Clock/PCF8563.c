#include <PCF8563.h> 


unsigned char bcd_to_decimal(unsigned char value)                
{                                                                                          
	return ((value & 0x0F) + (((value & 0xF0) >> 0x04) * 0x0A));
}                                          
                                                             
        
unsigned char decimal_to_bcd(unsigned char value) 
{                                                            
	return (((value / 0x0A) << 0x04) & 0xF0) | ((value % 0x0A) & 0x0F);   
}                                 


void PCF8563_init()
{   		              
	PCF8563_stop_RTC(); 
	clear_alarm();           
}  


void PCF8563_start_RTC() 
{  
	PCF8563_write(PCF8563_CONTROL1, 0x08);
}


void PCF8563_stop_RTC()
{
	PCF8563_write(PCF8563_CONTROL1, 0x28); 
}


void PCF8563_write(unsigned char address, unsigned char value)
{
	i2c_start();           
	i2c_write(PCF8563_write_address); 
	i2c_write(address);  
    i2c_write(value);
    i2c_stop();
}

                                  
unsigned char PCF8563_read(unsigned char address)
{                 
	unsigned char value = 0;
   	
	i2c_start();
	i2c_write(PCF8563_write_address);  
	i2c_write(address);  
	i2c_start(); 
	i2c_write(PCF8563_read_address);              
	value = i2c_read(0);
	i2c_stop();    
   	
	return value;
}    


void read_time()
{                                       
	sec = PCF8563_read(PCF8563_SECONDS);       
	sec &= 0x7F;
	sec = bcd_to_decimal(sec);
	                                     
	min = PCF8563_read(PCF8563_MINUTES);        
	min &= 0x7F;
	min = bcd_to_decimal(min);   
   	
	hr = PCF8563_read(PCF8563_HOURS);
	hr &= 0x3F;
	hr = bcd_to_decimal(hr); 
}    


void read_date()
{              
	dt = PCF8563_read(PCF8563_DATE);
	dt &= 0x3F;             
	dt = bcd_to_decimal(dt); 
   	
	wk = PCF8563_read(PCF8563_WEEKDAYS);
	wk &= 0x07;
	wk = bcd_to_decimal(wk);   	
   	
	mt = PCF8563_read(PCF8563_MONTHS);
	mt &= 0x1F;
	mt = bcd_to_decimal(mt);
	                    
	                   
	yr = PCF8563_read(PCF8563_YEARS);
	yr = bcd_to_decimal(yr);
}                                             

                                 
void read_al_time()
{                             
	unsigned char temp = 0;
   	
	al_min = PCF8563_read(PCF8563_MINUTE_ALARM); 
	temp = (al_min & 0x80);
	al_min &= 0x7F;
	al_min = bcd_to_decimal(al_min);   
	           	
	al_hr = PCF8563_read(PCF8563_HOUR_ALARM);
    al_state = ((temp | (al_hr & 0x80)) >> 7);
	al_hr &= 0x3F;                         
	al_hr = bcd_to_decimal(al_hr);   
}


void write_time()
{      
	sec = decimal_to_bcd(sec);	  
	PCF8563_write(PCF8563_SECONDS, sec);
   	
	min = decimal_to_bcd(min);   	
	PCF8563_write(PCF8563_MINUTES, min); 
   	
	hr = decimal_to_bcd(hr);   	
	PCF8563_write(PCF8563_HOURS, hr);  
}

                                                                      
void write_date()
{    
	dt = decimal_to_bcd(dt);   	
	PCF8563_write(PCF8563_DATE, dt);
	                   
	wk = decimal_to_bcd(wk);	     
	PCF8563_write(PCF8563_WEEKDAYS, wk); 
   	
	mt = decimal_to_bcd(mt);   	
	PCF8563_write(PCF8563_MONTHS, mt);
   	
	yr = decimal_to_bcd(yr);	                                 
	PCF8563_write(PCF8563_YEARS, yr);  
}
                                                                                                                         

void write_al_time()
{                   
	al_min = decimal_to_bcd(al_min);
	                  
	if(al_state == alarm_off)    
	{     
	   al_min |= 0x80; 
	}                   
	else
	{                    
	   al_min &= 0x7F;  
	}    
   	
	PCF8563_write(PCF8563_MINUTE_ALARM, al_min); 
	                                   
	al_hr = decimal_to_bcd(al_hr);	    
	                                              
	if(al_state == alarm_off)
	{               
	   al_hr |= 0x80;                          
	}                
	else
	{
	   al_hr &= 0x7F;                   
	   PCF8563_write(PCF8563_CONTROL2, 0x02);     
	} 
	                                         
	PCF8563_write(PCF8563_HOUR_ALARM, al_hr);   
}                                     
                                            

short check_alarm() 
{
	unsigned char temp = 0;
	                                     
	temp = (PCF8563_read(PCF8563_CONTROL2) & 0x08);
   	
	if(temp != 0)
	{
		return 1;                           
	}               
	else                                  
	{                              
		return 0;
	}
}
                                                
                   
void clear_alarm()                               
{                                        
	unsigned char temp = 0;
   	
	temp = PCF8563_read(PCF8563_CONTROL2);
	PCF8563_write(PCF8563_CONTROL2, (temp & 0xF7));
}                                              
                                                      
          
                                                
