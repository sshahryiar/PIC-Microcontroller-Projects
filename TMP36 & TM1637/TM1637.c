#include "TM1637.h"


void TM1637_init(void)
{
    TM1637_DAT_LOW;
    TM1637_CLK_LOW;  
    TM1637_send_command(TM1637_CMD_SET_DSIPLAY | TM1637_BRIGHTNESS_4 | TM1637_SET_DISPLAY_ON);
    TM1637_clear();
}


void TM1637_start(void)
{
    TM1637_DAT_HIGH;
    TM1637_CLK_HIGH;  
    delay_us(TM1637_DELAY_US);
    TM1637_DAT_LOW;
}


void TM1637_stop(void)
{

    TM1637_CLK_LOW;
    delay_us(TM1637_DELAY_US);

    TM1637_DAT_LOW;
    delay_us(TM1637_DELAY_US);

    TM1637_CLK_HIGH;
    delay_us(TM1637_DELAY_US);

    TM1637_DAT_HIGH;
}


unsigned char TM1637_write_byte(unsigned char value)
{
    unsigned char i = 0x08;
    unsigned char ack = 0x00;

    while(i)
    {
        TM1637_CLK_LOW;
        delay_us(TM1637_DELAY_US);

        if(value & 0x01)
        {
            TM1637_DAT_HIGH;
        } 

        else 
        {
            TM1637_DAT_LOW;
        }

        TM1637_CLK_HIGH;
        delay_us(TM1637_DELAY_US);

        value >>= 1;
        i--;
    }

    TM1637_CLK_LOW;

    delay_us(TM1637_DELAY_US);
                                 
    ack = input(TM1637_DAT_PIN);

    if(ack != 0)
    {
        TM1637_DAT_LOW;
    }

    delay_us(TM1637_DELAY_US);

    TM1637_CLK_HIGH;
    delay_us(TM1637_DELAY_US);

    TM1637_CLK_LOW;
    delay_us(TM1637_DELAY_US);


    return (ack);
}


void TM1637_send_command(unsigned char value)
{

    TM1637_start();
    TM1637_write_byte(value);
    TM1637_stop();
}


void TM1637_clear(void)
{
    signed char i = (TM1637_POSITION_MAX - 1);

    while(i > -1)
    {
        TM1637_display_segments(i, 0x00, 0x00);
        i--;
    };
}



void TM1637_display_segments(unsigned char position, unsigned char segment_value, unsigned char colon_state)
{
    if(position == 1)
    {
        switch(colon_state)
        {
            case 1:
            {
                segment_value |= 0x80;
                break;
            }

            default:
            {
                segment_value &= 0x7F;
                break;
            }
        }
    }

    TM1637_send_command(TM1637_CMD_SET_DATA | TM1637_SET_DATA_F_ADDR);
    TM1637_start();
    TM1637_write_byte(TM1637_CMD_SET_ADDR | (position & (TM1637_POSITION_MAX - 1)));
    TM1637_write_byte(segment_value);
    TM1637_stop();
}
