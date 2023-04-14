#include <16F877A.h>   

#device *= 16
#device adc=8      

#fuses HS, NOWDT, NOPROTECT, NOLVP, PUT   
#fuses NOBROWNOUT, CPD, NODEBUG, NOWRT         

#use delay(clock=10MHz)       
#use rs232(baud=1200, xmit=PIN_C6, bits=8, parity=N)     
                                 
#include <lcd.c>               
                                     
                                                     
void main()                               
{

   byte s=0;                             
   lcd_init();
   lcd_putc("\f");                                                                        
   setup_adc_ports(adc_clock_internal); 
   setup_adc(AN0);                               
   set_adc_channel(0);                                                  
   while(true)                  
   {                                                                                                          
      read_adc(adc_start_only);
      while(!adc_done());
      s = read_adc(adc_read_only);
      lcd_gotoxy(1,1);
      printf(lcd_putc, "\fTx Data = %u" s); 
      putc(s);           
      delay_ms(100);
   }
} 
