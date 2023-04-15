#include <12F675.h>

#fuses MCLR, NOWDT, PROTECT, CPD
#fuses NOBROWNOUT, INTRC_IO, PUT

#use delay (clock=4MHz)

#BYTE GPIO   = 0x005
#BYTE TRISIO = 0x085

#bit r_LED1  =  GPIO.0
#bit r_LED2  =  GPIO.1
#bit button  =  GPIO.2
#bit b_LED1  =  GPIO.4
#bit b_LED2  =  GPIO.5

unsigned int8 key = 0;

void setup_MCU();
void blink_LEDs();

#int_RA

void detect_button_press()
{
   if(button)
   {
      delay_us(99);
      if(button)
      {
         b_LED1 = 0;
         b_LED2 = 0;
         r_LED1 = 0;
         r_LED2 = 0;
         key++;
      }
   }
   if(key>4)
   {
      key=0;
   }
}

void main()
{
   setup_MCU();
   for(;;)
   {
      blink_LEDs();
   }
}

void setup_MCU()
{
   key = 0;
   disable_interrupts(GLOBAL);
   setup_ADC(ADC_off);
   setup_ADC_ports(no_analogs);
   TRISIO = 0x0C;
   GPIO  &= 0x00;
   ext_int_edge(L_to_H);
   enable_interrupts(GLOBAL);
   enable_interrupts(int_RA);
   delay_ms(499);
}

void blink_LEDs()
{
   unsigned int8 s = 0;
   if(key == 1)
   {
      s = 0;
      r_LED1 = 0;
      r_LED2 = 0;
      for(s=1;s<9;s++)
      {
         b_LED1 ^= 0x1;
         b_LED2 ^= 0x1;
         delay_ms(49);
      }
      s = 0;
      b_LED1 = 0;
      b_LED2 = 0;
      for(s=1;s<9;s++)
      {
         r_LED1 ^= 0x1;
         r_LED2 ^= 0x1;
         delay_ms(49);
      }
   }
   if(key == 2)
   {
      b_LED1 = 1;
      b_LED2 = 1;
      r_LED1 = 0;
      r_LED2 = 0;
      delay_ms(149);
      r_LED1 = 1;
      r_LED2 = 1;
      b_LED1 = 0;
      b_LED2 = 0;
      delay_ms(149);
   }
   if(key == 3)
   {
      b_LED1 = 1;
      b_LED2 = 0;
      r_LED1 = 1;
      r_LED2 = 0;
      delay_ms(199);
      r_LED1 = 0;
      r_LED2 = 1;
      b_LED1 = 0;
      b_LED2 = 1;
      delay_ms(199);
   }
   if(key == 4)
   {
      b_LED1 = 1;
      b_LED2 = 0;
      r_LED1 = 0;
      r_LED2 = 0;
      delay_ms(99);
      b_LED1 = 0;
      b_LED2 = 1;
      r_LED1 = 0;
      r_LED2 = 0;
      delay_ms(99);
      b_LED1 = 0;
      b_LED2 = 0;
      r_LED1 = 1;
      r_LED2 = 0;
      delay_ms(99);
      b_LED1 = 0;
      b_LED2 = 0;
      r_LED1 = 0;
      r_LED2 = 1;
      delay_ms(99);
      b_LED1 = 0;
      b_LED2 = 0;
      r_LED1 = 0;
      r_LED2 = 0;
      delay_ms(99);
      b_LED1 = 0;
      b_LED2 = 0;
      r_LED1 = 0;
      r_LED2 = 1;
      delay_ms(99);
      b_LED1 = 0;
      b_LED2 = 0;
      r_LED1 = 1;
      r_LED2 = 0;
      delay_ms(99);
      b_LED1 = 0;
      b_LED2 = 1;
      r_LED1 = 0;
      r_LED2 = 0;
      delay_ms(99);
      b_LED1 = 1;
      b_LED2 = 0;
      r_LED1 = 0;
      r_LED2 = 0;
      delay_ms(99);
      b_LED1 = 0;
      b_LED2 = 0;
      r_LED1 = 0;
      r_LED2 = 0;
      delay_ms(99);
   }
   else
   {
      b_LED1 = 0;
      b_LED2 = 0;
      r_LED1 = 0;
      r_LED2 = 0;
   }
}

