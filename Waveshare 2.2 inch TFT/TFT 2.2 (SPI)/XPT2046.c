#include "XPT2046.h"


void tp_init()
{
    tp_IO_init();
    output_high(TP_CS);
    setup_SPI(SPI_MASTER | SPI_XMIT_L_TO_H | SPI_SCK_IDLE_HIGH | SPI_SAMPLE_AT_MIDDLE);
}


void tp_IO_init()
{
    set_TRIS_A(0xE9);
    set_TRIS_C(0xD7);
}


unsigned long TP_read()
{
    unsigned char lb = 0x00;
    unsigned char hb = 0x00;
    unsigned long value = 0x0000;

    hb = SPI_read(0);
    lb = SPI_read(0);
    value = hb;
    value <<= 8;
    value |= lb;
    value >>= 3;
    value &= 0x0FFF;

    return value;
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
        SPI_write(CMD_RDY);
        avg_y += TP_read();

        SPI_write(CMD_RDX);
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
