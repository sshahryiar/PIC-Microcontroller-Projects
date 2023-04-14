#define polarity_button RA3_bit


bit pol;
unsigned char seq;
unsigned long t;


static void setup_mcu();
void perform_demo_works();
void common_display();


void main()
{
   setup_mcu();
   while(1)
   {
      t = (adc_read(0) + 0x1);
      delay_us(20);
      perform_demo_works();
   }
}


static void setup_mcu()
{
       ADC_Init();
       TRISC = 0x00;
       PORTC = 0x00;
}


void perform_demo_works()
{
   if(!polarity_button)
   {

      delay_ms(20);
      pol = ~pol;

   }

   else
   {

      if(pol)
      {

         for(seq=1; seq<=8; seq=seq<<1)
         {

            common_display();

         }

      }

      else
      {

         for(seq=8; seq>=1; seq=seq>>1)
         {

            common_display();

         }

      }
   }
}


void common_display()
{
   PORTC = seq;
   Vdelay_ms(t);
}