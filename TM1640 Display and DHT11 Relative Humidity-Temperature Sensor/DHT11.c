#include "DHT11.h"


void DHT11_init()
{
   output_float(DHT11_pin);
   delay_ms(1000);
}


unsigned char get_byte()
{
   unsigned char s = 0;                                    
   unsigned char value = 0;
   for(s = 0; s < 8; s += 1) 
   {
      value <<= 1;
      while(!input(DHT11_pin));
      delay_us(30);
      if(input(DHT11_pin))
      {
          value |= 1;
      }
      while(input(DHT11_pin));
   }
   return value;
}


unsigned char get_data()
{
   short chk = 0;
   unsigned char s = 0;
   unsigned char check_sum = 0;

   output_high(DHT11_pin);
   output_low(DHT11_pin);
   delay_ms(18);
   output_high(DHT11_pin);
   delay_us(26);
   
   chk = input(DHT11_pin);
   if(chk)
   {
      return 1;
   }
   delay_us(80);
   
   chk = input(DHT11_pin);
   if(!chk)
   {
      return 1;
   }
   delay_us(80);

   for(s = 0; s <= 4; s += 1)
   {
       values[s] = get_byte();
   }
  
   output_high(DHT11_pin); 
   
   for(s = 0; s < 4; s += 1)
   {
       check_sum += values[s];
   }
   
   if(check_sum != values[4])
   {
      return 1;
   }
   else
   {
      return 0;
   }
}
