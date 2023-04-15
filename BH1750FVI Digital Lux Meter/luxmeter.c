#include <16F87.h>
             

#device *= 16


#fuses INTRC_IO, PROTECT, NOWDT, BROWNOUT, NOWRT
#fuses NODEBUG, NOIESO, PUT, NOMCLR, FCMEN, CPD
                                                                            
                          
#use delay (internal = 4MHz) 

                       
#include <lcd420.c>
#include "BH1750.c"
                               
                               
void setup();    


void main()
{
		unsigned long lux = 0;   
		setup();
		while(TRUE)
		{
			lux = get_lux_value(cont_H_res_mode1, 180);  	   
			lcd_gotoxy(6, 2);  
			printf(lcd_putc, "%05lu    ", lux);  
			delay_ms(100);                          
		};     
}
          

void setup()
{                                          
	disable_interrupts(global);
	setup_oscillator(OSC_4MHz);            
	setup_WDT(WDT_OFF);
	setup_comparator(NC_NC_NC_NC);
	setup_SPI(SPI_disabled | SPI_SS_disabled);
	setup_timer_0(0 | 0 | 0);
	setup_timer_1(T1_disabled);
	setup_timer_2(T2_disabled, 255, 1);
	setup_CCP1(CCP_off);
	set_timer0(0);
	set_timer1(0);
	set_timer2(0);
	BH1750_init();
	lcd_init();
	lcd_putc("\f");
	lcd_gotoxy(1, 1);  
	lcd_putc("Ambient Light/lx");
}
                    
