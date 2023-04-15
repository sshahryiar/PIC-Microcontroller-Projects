#include "DS18B20.h"


void DS18B20_init()                             
{                                       
   onewire_reset();
}              
                                        
                                                             
signed long get_temperature()
{                                               
   unsigned char msb = 0;
   unsigned char lsb = 0;
   signed long temp = 0.0;  
                          
   onewire_reset();     
   onewire_write(skip_ROM);        
   onewire_write(convert_T); 
   delay_ms(750);             
              
   onewire_reset(); 
   onewire_write(skip_ROM);                 
   onewire_write(read_scratchpad);
   lsb = onewire_read();
   msb = onewire_read();
   temp = msb;                           
   temp *= 256;
   temp += lsb;
      
   delay_ms(40);      
   return (temp);       
}
