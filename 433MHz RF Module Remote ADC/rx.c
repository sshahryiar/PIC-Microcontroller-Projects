#include <16F877A.h> 
                 
#device *= 16

#fuses HS, NOWDT, NOPROTECT, NOLVP, PUT
#fuses NOBROWNOUT, CPD, NODEBUG, NOWRT

#use delay(clock=20MHz) 
#use rs232(baud=1200, rcv=PIN_C7, bits=8, parity=N) 
                                    
#include <lcd.c>                 

void main()           
{
   byte c;                                                                               
   lcd_init();                
   lcd_putc("\f");                                    
   while(true)
    {                                                                                        
        c=getc();                                                                                                 
        printf(lcd_putc,"\fRx Data = %u",c);
        delay_ms(100);                               
    }                                           

}    
