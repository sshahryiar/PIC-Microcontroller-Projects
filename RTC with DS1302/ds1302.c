#include "DS1302.h"


struct  
{             
	unsigned char year;  
    unsigned char month;                          
    unsigned char date;  
    unsigned char dow;   
    unsigned char hour;  
    unsigned char min;  
    unsigned char sec;     
}time;                     
                                                                             
                      
void DS1302_init()
{
	output_low(RST);
	output_low(SCK);
	delay_ms(2);
	DS1302_write(control_wreg, 0x00);  
	DS1302_write(charge_wreg, 0xA9);   
	if((time.date > 31) || (time.date < 1) || (time.month > 12) || (time.date < 1))
	{                                      
		set_time(10, 10, 10);        
		set_date(1, 1, 0, 7);
	}
}                                                                        
                                      
                                                               
void serial_write(unsigned char value) 
{   
	unsigned char n = 0; 
	unsigned char mask = 1;
	for(n = 0; n < 8; n += 1)       
	{      
		if((value & mask))
		{      
			output_high(IO);
		}
		else
		{
			output_low(IO); 
		}  
		output_high(SCK);
		output_low(SCK);  
		mask <<= 1; 
	}  
}       


unsigned char serial_read()
{     
	unsigned char n = 0; 
	unsigned char mask = 1;
	unsigned char value = 0;
	for(n = 0; n < 8; n += 1)
	{   
		if(input(IO))
		{               
			value |= mask;          
		}    
		output_high(SCK);  
		output_low(SCK);  
		mask <<= 1;
	}     
	return value;
}

                  
void DS1302_write(unsigned char addr, unsigned char value)        
{                                                                                                                
	output_high(RST);
	serial_write(addr);
	serial_write(value);                 
	output_low(RST); 
}     
                                            
                                                 
unsigned char DS1302_read(unsigned char addr)
{                                             
	unsigned char result = 0;                             
	output_high(RST);               
	serial_write(addr);                    
	result = serial_read();  
	output_low(RST);                           
	return result;        
}     


unsigned char read_ram(unsigned char addr)
{ 
	unsigned char value = 0;    
	if((ramlocstartr + addr) <= ramlocendr) 
	{
		value = DS1302_read((ramlocstartr + addr));              
	}    
	return value;
}            


void write_ram(unsigned char addr, unsigned char value)    
{    
	if((ramlocstartw + addr) <= ramlocendw) 
	{                                                           
		DS1302_write((ramlocstartw + addr), value);              
	}                                     
}


void get_time()
{          
	time.hour = DS1302_read(hr_rreg);  
	time.hour = bcd_to_decimal((0x3F & time.hour)); 
	time.min = DS1302_read(min_rreg); 
	time.min = bcd_to_decimal((0x7F & time.min));  
	time.sec = DS1302_read(sec_rreg);   
	time.sec = bcd_to_decimal((0x7F & time.sec)); 
}                                    


void get_date()
{    
	time.date = DS1302_read(date_rreg);
	time.date = bcd_to_decimal((0x3F & time.date));    
	time.month = DS1302_read(month_rreg);          
	time.month = bcd_to_decimal((0x1F & time.month));   
	time.dow = DS1302_read(dow_rreg);            
	time.dow = bcd_to_decimal((0x07 & time.dow));         
	time.year = DS1302_read(year_rreg);                   
	time.year = bcd_to_decimal((0xFF & time.year));  
}                                    


void set_time(unsigned char p1, unsigned char p2, unsigned char p3)
{                                                                 
	time.hour = p1;         
	p1 = decimal_to_bcd(time.hour);   
	DS1302_write(hr_wreg, p1);    
	time.min = p2;         
	p2 = decimal_to_bcd(time.min);    
	DS1302_write(min_wreg, p2);          
	time.sec = p3;         
	p3 = decimal_to_bcd(time.sec);   
	DS1302_write(sec_wreg, p3);   
}        
                                         

void set_date(unsigned char p1, unsigned char p2, unsigned char p3, unsigned char p4)
{            
	time.date = p1;         
	p1 = decimal_to_bcd(time.date);   
	DS1302_write(date_wreg, p1);    
	time.month = p2;                        
	p2 = decimal_to_bcd(time.month);   
	DS1302_write(month_wreg, p2);          
	time.year = p3;         
	p3 = decimal_to_bcd(time.year);   
	DS1302_write(year_wreg, p3);
	time.dow = p4;  
	p4 = decimal_to_bcd(time.dow);   
	DS1302_write(dow_wreg, p4);    
}                                  

                                       
unsigned char bcd_to_decimal(unsigned char d)                
{                                                                                          
    return ((d & 0x0F) + (((d & 0xF0) >> 4) * 10));
}                                
                                                             
                 
unsigned char decimal_to_bcd(unsigned char d)   
{                                                 
    return (((d / 10) << 4) & 0xF0) | ((d % 10) & 0x0F);
}          
