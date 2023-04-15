#include <16F84A.h>


#device *= 16


#fuses NOWDT, PUT, PROTECT, XT


#use delay (clock = 4MHz)


#include <lcd420.c>
#include "DHT11.c"


const unsigned char symbols[8]= 
{        
    0x07, 0x05, 0x05, 0x07, 0x00, 0x00, 0x00, 0x00
};


void setup();
void lcd_symbols();


void main()
{

   unsigned char state = 0;
   setup();
   
   while(TRUE)
   {
      state = get_data();
      
      switch(state)
      {
            case 1:
            {
            }
            case 2:
            {
               lcd_putc("\fNo Sensor Found!");
               break;
            }
            case 3:
            {
               lcd_putc("\fChecksum Error!");
               break;
            }
            default:
            {
               lcd_gotoxy(1, 1);
               lcd_putc("Tmp/ C: ");
               lcd_gotoxy(5, 1);
               lcd_putc(0);
               lcd_gotoxy(1, 2);
               lcd_putc("R.H/ %: ");
               lcd_gotoxy(9, 1);
               printf(lcd_putc, "%3u.%03u ", values[2], values[3]);
               lcd_gotoxy(9, 2);
               printf(lcd_putc, "%3u.%03u ", values[0], values[1]);
               break;
            }
      }
      delay_ms(1000);
   };
}


void setup()
{
   disable_interrupts(global);
   port_B_pullups(FALSE);
   setup_timer_0(T0_internal | T0_8_bit);
   set_timer0(0);
   DHT11_init();
   lcd_init();
   lcd_putc("\f");
   lcd_symbols();
}


void lcd_symbols()  
{
   unsigned char i = 0;              
   lcd_send_byte(0, 0x40); 
   for(i = 0; i < 8; i++)
   {
        lcd_send_byte(1, symbols[i]);
   }
   lcd_send_byte(0, 0x80);
} 
