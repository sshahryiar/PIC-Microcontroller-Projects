#include "WS2811.h"


void WS2811_init()
{
    WS2811_reset();
}


void WS2811_update()
{
   delay_us(60);
}


void WS2811_reset()
{
    WS2811_set_common_colour_on_all_LEDs(0, 0, 0);
}


void WS2811_send_data(unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int8 s = number_of_bits;

    unsigned int32 value = 0x00000000;

    value = (((unsigned int32)g << 16) | ((unsigned int32)r << 8) | ((unsigned int32)b));

    while(s > 0)
    {
       if((value & 0x800000) != LOW)
       {
            spi_write(one);
       }

       else
       {
           spi_write(zero);
       }

       value <<= 1;
       s--;
    }
}


void WS2811_set_common_colour_on_all_LEDs(unsigned char r, unsigned char g, unsigned char b)
{
    signed int16 n = (number_of_LEDs - 1);

    while(n > -1)
    {
        LED_colours[n][0] = r;
        LED_colours[n][1] = g;
        LED_colours[n][2] = b;
        WS2811_send_data(LED_colours[n][0], LED_colours[n][1], LED_colours[n][2]);
        n--;
    }

    WS2811_update();
}


void WS2811_set_colour_on_channel(unsigned int16 channel, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int16 n = 0;

    LED_colours[channel][0] = r;
    LED_colours[channel][1] = g;
    LED_colours[channel][2] = b;

    for(n = 0; n < number_of_LEDs; n++)
    {
        WS2811_send_data(LED_colours[n][0], LED_colours[n][1], LED_colours[n][2]);
    }

    WS2811_update();
}
