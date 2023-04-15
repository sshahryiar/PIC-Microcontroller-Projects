#include "8255.h"


void _8255_init()
{
   output_high(RST);
   delay_ms(400);
   output_low(RST);
   
   write_8255(CTRL_REG, (PORTA_input | PORTB_input | PORTC_lower_output | PORTC_upper_output | Group_A_basic_IO | Group_B_basic_IO | mode_set_flag_active));
}                                


void select_reg(unsigned char reg_cmd)
{
    switch(reg_cmd)
    {
        case PORT_A:
        {
            output_low(Addr_0);
            output_low(Addr_1);
            break;
        }
        case PORT_B:
        {
            output_high(Addr_0);
            output_low(Addr_1);
            break;
        }
        case PORT_C:
        {
            output_low(Addr_0);
            output_high(Addr_1);
            break;
        }
        case CTRL_REG:
        {
            output_high(Addr_0);
            output_high(Addr_1);
            break;
        }
    }
}


void write_8255(unsigned char reg_cmd, unsigned char value)
{
    output_low(CS);
    output_high(RD);
    output_low(WR);
    data_dir_out;
    select_reg(reg_cmd);
    data_out(value);
    output_high(CS);
}


unsigned char read_8255(unsigned char reg_cmd)
{
    unsigned char value = 0x00;
    
    output_low(CS);
    output_low(RD);
    output_high(WR);
    data_dir_in;
    select_reg(reg_cmd);
    value= data_in;
    output_high(CS);
    
    return value;
}
