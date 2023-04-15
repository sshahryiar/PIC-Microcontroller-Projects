#include "XPT2046.h"


void tp_init()
{
    tp_IO_init();
    
    output_high(TP_CS);
    output_high(TP_MOSI);
    output_high(TP_SCK);
}


void tp_IO_init()
{
    set_TRIS_A(0xE9);
    set_TRIS_C(0xD7);
}


unsigned long TP_read()
{
    unsigned char i = 0x0C;
    unsigned long value = 0x0000;

    while(i > 0x00)
    {
        value <<= 1;

        output_high(TP_SCK);
        output_low(TP_SCK);

        if(TP_MISO == TRUE)
        {
            value++;
        }

        i--;
    };

    return value;
}


void tp_write(unsigned char value)
{
    unsigned char i = 0x08;

    output_low(TP_SCK);

    while(i > 0)
    {
        if((value & 0x80) != 0x00)
        {
            output_high(TP_MOSI);
        }
        else
        {
            output_low(TP_MOSI);
        }

        value <<= 1;

        output_low(TP_SCK);
        output_high(TP_SCK);
        
        i--;
    };
}


void TP_read_coordinates(unsigned long *x_pos, unsigned long *y_pos)
{
    unsigned long temp = 0x0000;
    unsigned long avg_x = 0x0000;
    unsigned long avg_y = 0x0000;

    unsigned char samples = 0x10;
    
    output_low(TP_CS);
    while(samples > 0)
    {
        tp_write(CMD_RDY);
        output_high(TP_SCK);
        output_low(TP_SCK);
        avg_y += TP_read();

        tp_write(CMD_RDX);
        output_high(TP_SCK);
        output_low(TP_SCK);
        avg_x += TP_read();
        
        samples--;
    };
    output_high(TP_CS);
    
    temp = (avg_x >> 4);
    temp = filter_data(temp, 0);
    *x_pos = temp;
    
    temp = (avg_y >> 4);
    temp = filter_data(temp, 1);
    *y_pos = temp;
}


unsigned long filter_data(unsigned long value, unsigned char axis)
{
    float temp[3];
    
    float sum = 0.0;
    float min_R = 0.0;
    float max_R = 0.0;
    float axis_max = 0.0;
    
    unsigned long res = 0x0000;
    
    unsigned char i = 0x00;
    
    switch(axis)
    {
        case 1:
        {
            min_R = RL_min;
            max_R = RL_max;
            axis_max = res_l;
            break;
        }
        default:
        {
            min_R = RW_min;
            max_R = RW_max;
            axis_max = res_w;
            break;
        }
    }
    
    temp[0] = map((((float)value) - error), min_R, max_R, 0.0, axis_max);
    temp[1] = map((((float)value) + error), min_R, max_R, 0.0, axis_max);
    temp[2] = map(((float)value), min_R, max_R, 0.0, axis_max);
    
    for(i = 0; i < 3; i++)
    {
        sum += temp[i];
    }
    
    sum /= 3.0;
    res = constrain(sum, 0.0, axis_max);
    
    return res;
}


float map(float value, float x_min, float x_max, float y_min, float y_max)
{
    return (y_min + (((y_max - y_min) / (x_max - x_min)) * (value - x_min)));
}


float constrain(float value, float value_min, float value_max)
{
      if(value >= value_max)
      {
           return value_max;
      }
      else if(value <= value_min)
      {
           return value_min;
      }
      else
      {
           return value;
      }
}
