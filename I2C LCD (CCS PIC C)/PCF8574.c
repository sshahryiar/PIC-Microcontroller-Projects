#include "PCF8574.h"
     
                                       
unsigned char PCF8574_read()
{
    unsigned char port_byte = 0;        
    i2c_start();                              
    i2c_write(PCF8574_read_cmd);        
    port_byte = i2c_read(0);             
    i2c_stop();
    return port_byte;
}                                                          


void PCF8574_write(unsigned char data_byte)                      
{
    i2c_start(); 
    i2c_write(PCF8574_write_cmd);               
    i2c_write(data_byte); 
    i2c_stop();
}                             
