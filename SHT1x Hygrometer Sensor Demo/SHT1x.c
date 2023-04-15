#include "SHT1x.h"
                    

void SHT1x_init(void)                     
{                                            
    SHT1x_reset();    
    delay_ms(20);     
}   

                                                     
void SHT1x_wait(void)
{
    unsigned long wait_time = 30000;
                    
    output_float(SHT_SDA_pin);                    
    output_low(SHT_SCL_pin);                  
    delay_us(1);                   
                          
    while((wait_time > 0) && (input(SHT_SDA_pin) != 0))
    {
        wait_time--;
        delay_us(10);   
    }    
}  
     

void SHT1x_reset(void)
{                 
    unsigned char s = 0x09;                 
    
    output_float(SHT_SDA_pin);    
    output_low(SHT_SCL_pin);   
    delay_us(2);
    
    while(s > 0)
    {       
        output_high(SHT_SCL_pin);  
        delay_us(2);
        output_low(SHT_SCL_pin);
        delay_us(2);
        s--;
    };      
    
    SHT1x_start_command();      
}
                                    
                                               
void SHT1x_software_reset(void)
{
    SHT1x_reset();        
    SHT1x_write(soft_reset_cmd);       
    delay_ms(15);         
}  
                    

void SHT1x_start_command(void)        
{                            
    output_float(SHT_SDA_pin);
    output_low(SHT_SCL_pin); 
    delay_us(1);
    output_high(SHT_SCL_pin);
    delay_us(1);
    output_low(SHT_SDA_pin);
    delay_us(1);
    output_low(SHT_SCL_pin); 
    delay_us(2);
    output_high(SHT_SCL_pin); 
    delay_us(1);                                                          
    output_float(SHT_SDA_pin);
    delay_us(1);
    output_low(SHT_SCL_pin);       
}


unsigned char SHT1x_write(unsigned char value)                             
{                               
    unsigned char s = 0x80;    
    unsigned char error = 0x00;   
                             
    delay_us(4);
    
    while(s > 0)
    {
        output_low(SHT_SCL_pin);     
        
        if((value & s) != 0)          
        {
            output_float(SHT_SDA_pin);  
        }                          
        else 
        {
            output_low(SHT_SDA_pin);                   
        }     
        
        delay_us(1);
        
        output_high(SHT_SCL_pin);
        delay_us(1);     
        
        s >>= 1;
    }
                      
    output_low(SHT_SCL_pin); 
    delay_us(1);             
    
    error = input(SHT_SDA_pin);   
    
    output_high(SHT_SCL_pin);     
    delay_us(1);                 
    
    output_low(SHT_SCL_pin);        
    
    return error;   
}          


unsigned char SHT1x_read_byte(void)
{
    signed char s = 0x08;
    unsigned char value = 0x00;    
    
    while(s  > 0)
    {
        value <<= 1;   
        
        output_high(SHT_SCL_pin);               
        delay_us(1);
        
        if(input(SHT_SDA_pin))
        {            
            value |= 0x01;           
        }
        
        output_low(SHT_SCL_pin);            
        delay_us(1);                
        
        s--;                                                                  
    };
    
    return value;
}        


void SHT1x_send_ack(unsigned char ack_val)   
{
    switch(ack_val)
    {
        case 1:
        {
            output_float(SHT_SDA_pin);    
            break;
        }
        
        default:
        {   
            output_low(SHT_SDA_pin);
            break;
        }   
    }
    
    delay_us(1);
    output_high(SHT_SCL_pin);  
    delay_us(2);
    output_low(SHT_SCL_pin);  
}


unsigned long SHT1x_read(void)
{
    unsigned long value = 0x0000;  
    
    value = (unsigned long)SHT1x_read_byte();   
    SHT1x_send_ack(0);             
                      
    value <<= 8;                   
    delay_us(1);
    output_float(SHT_SDA_pin);    
        
    value |= (unsigned long)SHT1x_read_byte();  
    SHT1x_send_ack(1);        
                                 
    return(value);  
}                                     
                                                 

unsigned long SHT1x_get_temperature(void)
{
    unsigned char error = 0x00;
    unsigned long value = 0x0000;
    
    SHT1x_start_command();           
    error = SHT1x_write(measure_T_cmd);   
    
    if(error != 0)
    {
        return;
    }
    
    SHT1x_wait();             
    
    value = SHT1x_read();     
    
    return value;
}

                               
unsigned long SHT1x_get_relative_humidity(void) 
{                                    
    unsigned char error = 0x00;
    unsigned long value = 0x0000;
                            
    SHT1x_start_command();           
    error = SHT1x_write(measure_RH_cmd); 
              
    if(error != 0)          
    {
        return;
    }
    
    SHT1x_wait();              
               
    value = SHT1x_read();    
    
    return value;            
}


void SHT1x_calculate_values(unsigned long temp, unsigned long humid, float &tc, float &rhlin, float &rhtrue)
{
    float  rh = 0.0;
    //float truehumid1;
    
    //calculate temperature reading
    tc = ((float)temp * 0.01) - 40.0;
    
    //calculate Real RH reading
    rh = (float)humid;
                                                      
    rhlin = (rh * 0.0405) - (rh * rh * 0.0000028) - 4.0;
    
    //calculate True RH reading          
    rhtrue = ((tc - 25.0) * (0.01 + (0.00008 * rh))) + rhlin; 
}
                                                     
       
void SHT1x_get_data(float &temp, float &truehumid)
{                          
    unsigned long restemp = 0x0000;
    unsigned long reshumid = 0x0000;           
    float realhumid = 0.0;
    
    truehumid = 0.0;
                                    
    restemp = SHT1x_get_temperature(); 
    reshumid = SHT1x_get_relative_humidity();   
    
    SHT1x_calculate_values(restemp, reshumid, temp, realhumid, truehumid);        
}      
                    
