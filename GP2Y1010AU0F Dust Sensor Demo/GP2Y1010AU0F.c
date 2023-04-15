#include "GP2Y1010AU0F.h"


void GP2Y1010AU0F_init(void)
{
   GP2Y1010AU0F_LED_OFF;
   delay_ms(100);
}


unsigned long GP2Y1010AU0F_ADC_running_avg(unsigned long previous_avg)
{
   unsigned char samples = 0x08;
   unsigned long current_avg = 0x0000;
   
   GP2Y1010AU0F_LED_ON;
   GP2Y1010AU0F_SAMPLE_TIME;
   
   while(samples > 0)
   {
      read_adc(adc_start_only);
      while(!adc_done());
      current_avg += read_adc(adc_read_only);
      samples--;
   };
   
   GP2Y1010AU0F_HOLD_TIME;
   GP2Y1010AU0F_LED_OFF;
   GP2Y1010AU0F_SLEEP_TIME;
   
   current_avg >>= 3;
   
   current_avg += previous_avg;
   current_avg >>= 1;
   
   if(current_avg > GP2Y1010AU0F_MAX_LIMIT)
   {
      current_avg = GP2Y1010AU0F_MAX_LIMIT;
   }
   
   else if(current_avg < GP2Y1010AU0F_MIN_LIMIT)
   {
      current_avg = GP2Y1010AU0F_MIN_LIMIT;
   }
   
   return current_avg;
}


unsigned long GP2Y1010AU0F_read(unsigned long value)
{
   float tmp = 0.0;
   
   tmp = value;
   tmp /= ADC_CONV_FACTOR;
   tmp *= 167.0;
   tmp -= 110.0;
   
   return ((unsigned long)tmp);
}

