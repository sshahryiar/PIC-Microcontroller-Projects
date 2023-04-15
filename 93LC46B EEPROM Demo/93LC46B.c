#include "93LC46B.h"


void EE_93LC46B_init()
{
    set_TRIS_C(0xD3);
    output_C(0x00);
}


void write_value(unsigned char value)
{
   unsigned char clk = 0x08;
   
   while(clk > 0)
   {
        if((value & 0x80) != 0)
        {
            output_high(D_in_pin);
        }
        else
        {
            output_low(D_in_pin);
        }
        
        output_high(CLK_pin);
        delay_us(1);
        output_low(CLK_pin);
        delay_us(1);
        
        value <<= 1;
        clk--;
   };
}


unsigned char read_value()
{
    unsigned char clk = 0x08;
    unsigned char temp = 0x00;
   
    while(clk > 0)
    {
        output_high(CLK_pin);
        delay_us(1);
        output_low(CLK_pin);
        delay_us(1);
        
        temp <<= 1;
        
        if(D_out_pin == 1)
        {
            temp |= 1;
        }
        
        clk--;
    };
    
    return temp;
}


void EE_93LC46B_write(unsigned char address, unsigned long value)
{
    unsigned char hb = 0x00;
    unsigned char lb = 0x00;
    unsigned long timeout = 0x0000;
    
    hb = ((value & 0xFF00) >> 0x08);
    lb = (value & 0x00FF);
    
    EE_93LC46B_erase_and_write_enable();
    
    output_high(CS_pin);
    
    write_value(start);
    write_value(opcode_write | address);
    write_value(hb);
    write_value(lb);
    
    output_low(CS_pin);
    delay_us(20);    
    
    output_high(CS_pin);
    
    while((!D_out_pin) && (timeout < 10000))
    {
        delay_us(1);
        timeout++;
    };
    
    output_low(CS_pin);
    delay_us(20);    
    
    EE_93LC46B_erase_and_write_disable();
}


unsigned long EE_93LC46B_read(unsigned char address)
{
    unsigned char lb = 0x00;
    unsigned long hb = 0x00;
    
    output_high(CS_pin);
    
    write_value(start);
    write_value(opcode_read | address);
    hb = read_value();
    lb = read_value();
    
    output_low(CS_pin);
    delay_us(20);   
    
    hb <<= 0x08;
    hb |= lb;
    
    return hb;
}


void EE_93LC46B_erase_and_write_enable()
{
    output_high(CS_pin);
    write_value(start);
    write_value(opcode_write_and_erase_enable);
    output_low(CS_pin);
    delay_us(10);
}


void EE_93LC46B_erase_and_write_disable()
{
    output_high(CS_pin);
    write_value(start);
    write_value(opcode_write_and_erase_disable);
    output_low(CS_pin);
    delay_us(10);
}


void EE_93LC46B_erase(unsigned char address)
{ 
    output_high(CS_pin);
    write_value(start);
    write_value(opcode_erase | address);
    output_low(CS_pin);
    delay_us(10);
}


void EE_93LC46B_erase_all()
{
    output_high(CS_pin);
    write_value(start);
    write_value(opcode_erase_all);
    output_low(CS_pin);
    delay_us(10);
}
