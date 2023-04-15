#include <16F877A.h>


#device *= 16


#fuses NOWDT, PROTECT, CPD, NOLVP, HS, NODEBUG, NOWRT, PUT, BROWNOUT


#use delay (clock = 10MHz)


#define button       !input(pin_D1)


#include "lcd.c"
#include "MAX6675.c"


const unsigned char degree_symbol[8]= 
{
   0x00, 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00
}; 


void setup();
void lcd_load_symbols();


void main()
{
   unsigned char unit = deg_C;
   unsigned char state = close_contact;
   unsigned long ti = 0x0000;
   float tf = 0.0;
   
   setup();
   
   while(TRUE)
   {
      state = MAX6657_get_ADC(&ti);
      
      if(button)
      {
          delay_ms(40);
          while(button);
          unit++;
      }
      
      if(unit > tmp_K)
      {
         unit = deg_C;
      }
      
      switch(state)
      {
          case open_contact:
          {
              lcd_gotoxy(1, 2);
              lcd_gotoxy(1, 2);
              lcd_putc("     Error!     ");
              delay_ms(200);
              lcd_putc("                ");
              break;
          }
          case close_contact:
          {
              tf = MAX6675_get_T(ti, unit);  
              
              lcd_gotoxy(1, 2);
              lcd_putc("T/              ");
              
              
              switch(unit)
              {
                  case deg_F: 
                  {
                      lcd_gotoxy(3, 2);
                      lcd_putc(0);
                      lcd_gotoxy(4, 2);
                      lcd_putc("F:");
                      break;
                  }
                  case tmp_K: 
                  {
                      lcd_gotoxy(3, 2);
                      lcd_putc(" K:");
                      break;
                  }
                  case deg_C: 
                  {
                      lcd_gotoxy(3, 2);
                      lcd_putc(0);
                      lcd_gotoxy(4, 2);
                      lcd_putc("C:");
                      break;
                  }
              }
              
              if((tf >= 0) && (tf < 10))
              {
                  lcd_gotoxy(13, 2);
              }
              
              else if((tf >= 100) && (tf < 1000))
              {
                  lcd_gotoxy(12, 2);
              }
              
              else if((tf >= 100) && (tf < 1000))
              {
                  lcd_gotoxy(11, 2);
              }
              
              else
              {
                  lcd_gotoxy(10, 2);
              }
              
              printf(lcd_putc, "  %4.2g ", tf);
              delay_ms(100);
              
              break;
          }
      }
   };
}


void setup()
{
   disable_interrupts(global);
   setup_SPI(SPI_DISABLED | SPI_SS_DISABLED);
   setup_comparator(NC_NC_NC_NC);
   setup_adc(ADC_OFF);
   setup_adc_ports(no_analogs);
   setup_psp(FALSE);
   setup_CCP1(CCP_OFF);
   setup_CCP2(CCP_OFF);
   setup_timer_0(T0_INTERNAL | T0_DIV_256 | T0_8_BIT);
   set_timer0(0x00);
   setup_timer_1(T1_DISABLED);
   set_timer1(0x0000);
   setup_timer_2(T2_DISABLED, 0xFF, 1);
   set_timer2(0x00);
   
   MAX6675_init(); 
  
   lcd_init();
   lcd_putc("\f");
   
   lcd_gotoxy(1, 1);
   lcd_putc("MAX6675 KTC Demo");
   lcd_load_symbols() ;
}


void lcd_load_symbols()  
{
   unsigned char i = 0;     
   
   lcd_send_byte(0, 0x40); 
   
   for(i = 0; i < 8; i++)
   {
        lcd_send_byte(1, degree_symbol[i]);
   }
   
   lcd_send_byte(0, 0x80);
}      
