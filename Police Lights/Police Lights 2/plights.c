#include <16F84A.H>
#fuses NOWDT, PUT, XT, PROTECT
#use Delay (clock = 4000000)

int8 x;

int8 read_in(int x);
void type_1();
void type_2();
void type_3();
void type_4();

void main()
{
   int8 i,Z;
   output_B(0x00);
   output_low(pin_a2);
   delay_ms(9);
   for(;;)
   {
      Z=read_in(I);
      if(Z==1)
      {
         type_1();
      }
      else if(Z==2)
      {
         type_2();
      }
      else if(Z==3)
      {
         type_3();
      }
      else if(Z==4)
      {
         type_4();
      }
      else
      {
         output_low(pin_a2);
         output_B(0x00);
      }
   }
}

int8 read_in(int x)
{
   x = read_eeprom(0);
   if(input(pin_a0))
   {
      delay_ms(99);
      x++;
      if(x>4)
      {
         x=0;
      }
      write_eeprom(0,x);
   }
   return x;
}

void type_1()
{
   output_B(0x0F);
   delay_ms(250);
   output_B(0xF0);
   delay_ms(250);
   output_toggle(pin_A2);
   delay_ms(10);
}

void type_2()
{
   output_B(0x00);
   delay_ms(999);
   output_B(0x0F);
   delay_ms(100);
   output_B(0xF0);
   delay_ms(100);
   output_B(0x00);
   delay_ms(999);
   output_toggle(pin_A2);
   delay_ms(20);
}

void type_3()
{
   int8 s;
   for(s=0;s<3;s++)
   {
      output_B(0xFF);
      delay_ms(150);
      output_B(0x00);
      delay_ms(150);
      output_toggle(pin_A2);
      delay_ms(30);
   }
   delay_ms(1500);
}

void type_4()
{
   char j;
   delay_ms(499);
   for(j=1;j<4;j++)
   {
      output_B(0xFF);
      delay_ms(220);
      output_toggle(pin_A2);
      delay_ms(40);
   }
   delay_ms(300);
   for(j=1;j<4;j++)
   {
      output_B(0x00);
      delay_ms(220);
      output_toggle(pin_A2);
      delay_ms(40);
   }
   delay_ms(499);
}
