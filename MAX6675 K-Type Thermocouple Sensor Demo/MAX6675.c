#include "MAX6675.h"


void MAX6675_init()
{
   set_tris_C(0x04);
   output_high(CS_pin); 
   output_high(SCK_pin);
}


unsigned char MAX6657_get_ADC(unsigned long *ADC_data)
{
   unsigned char clk_pulses = 0x00;
   unsigned char samples = no_of_samples;
   unsigned long temp_data = 0x0000;
   unsigned long long avg_value = 0x00000000;
   
   
   while(samples > 0)
   {
         clk_pulses = no_of_pulses;
         temp_data = 0x0000;
         
         output_low(CS_pin);
         
         while(clk_pulses > 0)
         {    
            temp_data <<= 1;
            if(SO_pin == TRUE)
            {
                temp_data |= 1;
            }
            output_high(SCK_pin);
            output_low(SCK_pin);
            
            clk_pulses--;
         };   
   
         output_high(CS_pin);
         temp_data &= 0x7FFF;
         
         avg_value += ((unsigned long long)temp_data);
         
         samples--;
         delay_ms(10);
   };
   
   temp_data = ((unsigned long)(avg_value >> 4));
   
   if((temp_data & 0x04) == close_contact)
   {
      *ADC_data = (temp_data >> 3);
      return close_contact;
   }
   
   else
   {
      *ADC_data = (count_max + 1);
      return open_contact;
   }
   
}


float MAX6675_get_T(unsigned long ADC_value, unsigned char T_unit)
{
   float tmp = 0.0;
   
   tmp = (ADC_value * scalar_deg_C);
   
   switch(T_unit)
   {
      case deg_F:
      {
         tmp *= scalar_deg_F_1;
         tmp += scalar_deg_F_2;
         break;
      }
      case tmp_K:
      {
        tmp += scalar_tmp_K;
        break;
      }
      default:
      {
        break;
      }
   }

   return tmp;
}
