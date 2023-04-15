#include "DS18x20.h"


static short done;
static unsigned char last_discrepancy;
static unsigned char RomAddr[addr_len];    


short DS1820_Reset()
{
   short pres_pulse_state = 0;

   disable_interrupts(GLOBAL);
   
   output_low(DS18x20_pin);
   delay_us(reset_pulse);
   output_high(DS18x20_pin);
                                      
   delay_us(presence_delay);
   
   pres_pulse_state = input(DS18x20_pin);
   delay_us(424);                   
   
   enable_interrupts(GLOBAL);

   return pres_pulse_state;
}  
                                   

short DS1820_read_bit() 
{              
   short bit_value = 0;

   disable_interrupts(GLOBAL);     
   output_low(DS18x20_pin);
   delay_us(bit_start_pulse);   
   input(DS18x20_pin);
   delay_us(bit_read_delay);   
   bit_value = input(DS18x20_pin);
   enable_interrupts(GLOBAL);

   return bit_value;
}   


void DS1820_write_bit(short bit_value)   
{
   disable_interrupts(GLOBAL); 
   output_low(DS18x20_pin);
   delay_us(bit_start_pulse);                           

   if (bit_value != 0)        
   {
      output_high(DS18x20_pin);
   }

   delay_us(bit_write_delay);
   output_high(DS18x20_pin);
   
   enable_interrupts(GLOBAL);
}   


unsigned char DS1820_read_byte()
{                         
   unsigned char s = 0; 
   unsigned char value = 0;
                                                      
   for(s = 0; s < 8; s++)
   {
      if (DS1820_read_bit())
      {
         value |= (1 << s);
      }
      delay_us(120); 
   }
   return value;
} 


void DS1820_write_byte(unsigned char value)
{
   unsigned char s = 0;
   unsigned char tmp = 0;

   for(s = 0; s < 8; s++)      
   {                                    
      tmp = (value >> s);    
      tmp &= 0x01;          
      DS1820_write_bit(tmp); 
   }
                        
   delay_us(105); 
}   


void DS1820_device_addr(unsigned char addr_method)
{
   unsigned char s = 0;
   
   if(addr_method == match_rom_cmd)
   {
      DS1820_write_byte(match_rom_cmd);    
      for (s = 0; s < addr_len; s++)
      {
         DS1820_write_byte(RomAddr[s]);
      }
   }
   else
   {
      DS1820_write_byte(skip_rom_cmd);     
   }
}      


short DS1820_find_next_device()
{                         
    unsigned char state = 0;
    unsigned char byte_index = 0;
    unsigned char mask = 1;
    unsigned char bit_pos = 1;
    unsigned char discrepancy_marker = 0;
    short bit_b = FALSE;                   
    short b_Status = FALSE;   
    short next_b = FALSE;

    for(byte_index = 0; byte_index < 8; byte_index++)
    {
        RomAddr[byte_index] = 0x00;
    }         
                                                             
    b_Status = DS1820_reset(); 

    if(b_Status || done)    
    {
        last_discrepancy = 0;  
        return FALSE;
    }

    DS1820_write_byte(search_rom_cmd);

    byte_index = 0;
    do
    {
        state = 0;

        if(DS1820_read_bit() != 0)
        {
            state = 2;
        }
        delay_us(120); 

        if ( DS1820_read_bit() != 0)
        {
            state |= 1;
        }
        delay_us(120); 
        
        if (state == 3)
        {
            break;
        }
        else
        {
            if (state > 0)
            {
                bit_b = (short)(state >> 1);
            }
            else
            {
                if (bit_pos < last_discrepancy)
                {
                    bit_b = ((RomAddr[byte_index] & mask) > 0 );
                }
                else
                {
                    bit_b = (bit_pos == last_discrepancy);
                }

                if (bit_b == 0)
                {
                    discrepancy_marker = bit_pos;
                }
            }

           if (bit_b != 0)
           {
               RomAddr[byte_index] |= mask;
           }
           else
           {
               RomAddr[byte_index] &= ~mask;
           }

           DS1820_write_bit(bit_b);
           
           bit_pos++;                    
           mask <<= 1;

           if(mask == 0)
           {
               byte_index++;  
               mask = 1; 
           }
        }
    }while (byte_index < addr_len);

    if(bit_pos < 65)
    {
        last_discrepancy = 0;
    }
    else
    {
        last_discrepancy = discrepancy_marker;
        done = (last_discrepancy == 0);
        next_b = TRUE;
    }

    return next_b;
}



short DS1820_find_first_device()
{               
    last_discrepancy = 0;
    done = FALSE;

    return (DS1820_find_next_device());
}

                                          
void DS1820_write_EEPROM(unsigned char THigh, unsigned char TLow)
{           
    DS1820_reset();
    DS1820_device_addr(match_rom_cmd);
    DS1820_write_byte(write_mem_cmd); 
    DS1820_write_byte(THigh);
    DS1820_write_byte(TLow);
	delay_us(10);
    DS1820_reset();
    DS1820_device_addr(match_rom_cmd);
    DS1820_write_byte(copy_mem_cmd); 
    delay_ms(10);
}


signed long DS1820_get_raw_temp()
{
    unsigned char s = 0;
    unsigned long tmp = 0;
    unsigned long highres;
    unsigned char scrpad[scrpd_len];
    
    DS1820_reset();
    DS1820_device_addr(match_rom_cmd);   
    output_high(DS18x20_pin);
    DS1820_write_byte(convert_temp_cmd); 
    delay_ms(750);

    DS1820_reset();
    DS1820_device_addr(match_rom_cmd);
    DS1820_write_byte(read_mem_cmd); 

    for(s = 0; s < scrpd_len; s++)
    {
        scrpad[s] = DS1820_read_byte();
    }
    
    tmp = 0;                              
    tmp = (unsigned long)((unsigned long)scrpad[temp_MSB] << 8);
    tmp |= (unsigned long)(scrpad[temp_LSB]);

    if(RomAddr[0] == DS18S20_family_code)
    {                                       \
        tmp >>= 1;             
        tmp = ((unsigned long)tmp << 8);
                                           
        tmp -= ((unsigned long)temp_res >> 2);
                                                       
        highres = scrpad[cnt_per_s] - scrpad[cnt_remain];
        highres = ((unsigned long)highres << 8);
        if(scrpad[cnt_per_s])
        {                             
            highres = highres / ((unsigned long)scrpad[cnt_per_s]);
        }
                   
        highres = highres + tmp;
    }
    else
    {
        highres = tmp;
        highres <<= 4;
    }                      
                     
    return highres;                                  
}    

   
float DS1820_get_temp()
{
    return ((float)DS1820_get_raw_temp() / (float)temp_res);
}                                               
