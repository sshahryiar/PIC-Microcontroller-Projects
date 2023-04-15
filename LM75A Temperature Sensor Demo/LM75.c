#include "LM75.h"


void LM75_init(void)
{   
    LM75_write_byte(pointer_conf_reg, \
                    (queue_value_1 
                     | OS_POL_LOW \
                     | OS_Comp \
                     | normal \
                     | _12_bit_resolution \ 
                     | OS_mode_disable));
    delay_ms(60);
}                                                                                            


void LM75_write_byte(unsigned char pointer, unsigned char value)
{
    I2C_start();                  
    I2C_write(LM75_write_cmd); 
    I2C_write(pointer);
    I2C_write(value);                  
    I2C_stop();      
}                                                               
                                       

void LM75_write_bytes(unsigned char pointer, signed long value)
{
    unsigned char values[0x02] = {0x00, 0x00};

    values[0] = ((value & 0xFF00) >> 8);                                                                 
    values[1] = (value & 0x00FF);
                                                    
    I2C_start();                                         
    I2C_write(LM75_write_cmd); 
    I2C_write(pointer);                
    I2C_write(values[0]);                                                        
    I2C_write(values[1]);   
    I2C_stop();  
}                                                 
               

signed long LM75_read_byte(unsigned char pointer)
{
    unsigned char value = 0x00;                                     
    
    I2C_start();                                                     
    I2C_write(LM75_write_cmd);     
    I2C_write(pointer);           
    I2C_start();                 
    I2C_write(LM75_read_cmd);   
    value = I2C_read(0);                      
    I2C_stop();
                 
    return value; 
}                     

                           
signed long LM75_read_bytes(unsigned char pointer)  
{
    register signed long value = 0;                         
    
    unsigned char lb = 0;
    unsigned char hb = 0;    
    
    I2C_write(LM75_write_cmd);     
    I2C_write(pointer);     
    I2C_start();                 
    I2C_write(LM75_read_cmd);   
    hb = I2C_read(1);        
    lb = I2C_read(0);      
    I2C_stop();       
                                
    value = make16(hb, lb);  
    
    return value;            
}                                               


float LM75_get_temp(void)
{
    float temp = 0.0;
    
    temp = (LM75_read_bytes(pointer_temp_reg) >> 5);
    temp *= 0.125;                                                                                      
    
    return temp;
}
