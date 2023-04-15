#include "BH1750.h"
                                      
              
void BH1750_init()
{ 
	delay_ms(100);  
	BH1750_write(power_down);
}                  
              

void BH1750_write(unsigned char cmd)
{ 
	I2C_start();
	I2C_write(BH1750_address_w);        
	I2C_write(cmd);                
	I2C_stop();	
}

              
                                   
unsigned long BH1750_read_word()
{                      
	register unsigned long value = 0; 
	unsigned char lb = 0;
	unsigned char hb = 0;
	I2C_start();
	I2C_write(BH1750_address_r);      
	hb = I2C_read(1);  
	lb = I2C_read(0);
	I2C_stop();                           
	value = make16(hb, lb);     
	return value;
} 


unsigned long get_lux_value(unsigned char mode, unsigned long delay_time)
{
	register unsigned long lux_value = 0;  
	BH1750_write(power_up);
	BH1750_write(mode);
	lux_value = BH1750_read_word(); 
	delay_ms(delay_time);
	BH1750_write(power_down);
	return lux_value;                                 
}                                
