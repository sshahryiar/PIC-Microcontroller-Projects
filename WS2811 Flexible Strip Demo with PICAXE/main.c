#include <12F1840.h>


#device ADC = 10


#FUSES PUT                     
#FUSES MCLR                     
#FUSES PROTECT                  
#FUSES CPD                     
#FUSES NOBROWNOUT              
#FUSES NOCLKOUT                 
#FUSES NOIESO                    
#FUSES FCMEN                   
#FUSES NOWRT
#FUSES NOWDT
#FUSES NODEBUG
#FUSES STVREN                  
#FUSES BORV25                  
#FUSES NOLVP        
#FUSES INTRC_IO
#FUSES PLL


#use delay(internal = 32MHz)
#use SPI(MASTER, SPI1, BAUD = 8000000, MODE = 1, BITS = 8, STREAM = SPI_1)


#include "WS2811.c"


#define delay1                      40
#define delay2                      20


void setup(void);


void main(void)
{
   signed int8 i = 0;
   signed int8 j = 0;
   
   setup();
   
   while(TRUE)
   {
      WS2811_reset();
      for(i = 0; i < number_of_LEDs; i++)
      {
         WS2811_set_colour_on_channel(i, 255, 0, 0);
         if(i > 0)
         {
            j = (i - 1);
            WS2811_set_colour_on_channel(j, 0, 0, 0);
         }
         delay_ms(delay1);
      }
      
      WS2811_reset();
      for(i = 0; i < number_of_LEDs; i++)
      {
         WS2811_set_colour_on_channel(i, 0, 255, 0);
         if(i > 0)
         {
            j = (i - 1);
            WS2811_set_colour_on_channel(j, 0, 0, 0);
         }
         delay_ms(delay1);
      }
      
      WS2811_reset();
      for(i = 0; i < number_of_LEDs; i++)
      {
         WS2811_set_colour_on_channel(i, 0, 0, 255);
         if(i > 0)
         {
            j = (i - 1);
            WS2811_set_colour_on_channel(j, 0, 0, 0);
         }
         delay_ms(delay1);
      }
      
      WS2811_reset();
      for(i = 0; i < number_of_LEDs; i++)
      {
         WS2811_set_colour_on_channel(i, 255, 0, 255);
         if(i > 0)
         {
            j = (i - 1);
            WS2811_set_colour_on_channel(j, 0, 0, 0);
         }
         delay_ms(delay1);
      }
      
      WS2811_reset();
      for(i = 0; i < number_of_LEDs; i++)
      {
         WS2811_set_colour_on_channel(i, 0, 255, 255);
         if(i > 0)
         {
            j = (i - 1);
            WS2811_set_colour_on_channel(j, 0, 0, 0);
         }
         delay_ms(delay1);
      }
      
      WS2811_reset();
      for(i = 0; i < number_of_LEDs; i++)
      {
         WS2811_set_colour_on_channel(i, 255, 255, 0);
         if(i > 0)
         {
            j = (i - 1);
            WS2811_set_colour_on_channel(j, 0, 0, 0);
         }
         delay_ms(delay1);
      }
      
      WS2811_reset();
      for(i = 0; i < number_of_LEDs; i++)
      {
         WS2811_set_colour_on_channel(i, 255, 255, 255);
         if(i > 0)
         {
            j = (i - 1);
            WS2811_set_colour_on_channel(j, 0, 0, 0);
         }
         delay_ms(delay1);
      }
            
      WS2811_reset();
      for(i = 0; i < number_of_LEDs; i++)
      {
         WS2811_set_colour_on_channel(i, 255, 0, 0);
         delay_ms(delay2);
      }
      delay_ms(2000);

      WS2811_reset();
      for(i = 0; i < number_of_LEDs; i++)
      {
         WS2811_set_colour_on_channel(i, 0, 255, 0);
         delay_ms(delay2);
      }
      delay_ms(2000);

      WS2811_reset();
      for(i = 0; i < number_of_LEDs; i++)
      {
         WS2811_set_colour_on_channel(i, 0, 0, 255);
         delay_ms(delay2);
      }
      delay_ms(2000);

      WS2811_reset();
      for(i = 0; i < number_of_LEDs; i++)
      {
         WS2811_set_colour_on_channel(i, 255, 0, 255);
         delay_ms(delay2);
      }
      delay_ms(2000);

      WS2811_reset();
      for(i = 0; i < number_of_LEDs; i++)
      {
         WS2811_set_colour_on_channel(i, 0, 255, 255);
         delay_ms(delay2);
      }
      delay_ms(2000);

      WS2811_reset();
      for(i = 0; i < number_of_LEDs; i++)
      {
         WS2811_set_colour_on_channel(i, 255, 255, 0);
         delay_ms(delay2);
      }
      delay_ms(2000);

      WS2811_reset();
      for(i = 0; i < number_of_LEDs; i++)
      {
         WS2811_set_colour_on_channel(i, 255, 255, 255);
         delay_ms(delay2);
      }
      delay_ms(2000);
   };
}


void setup(void)
{
   setup_wdt(WDT_OFF);
   setup_comparator(NC_NC);
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_dac(DAC_OFF);
   setup_vref(VREF_OFF);
   setup_timer_0(T0_INTERNAL);
   set_timer0(0);
   setup_timer_1(T1_DISABLED);
   set_timer1(0);
   setup_timer_2(T2_DISABLED, 0, 1);
   set_timer2(0);
   setup_ccp1(CCP_OFF);
   WS2811_init();
}
