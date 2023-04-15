#include <16F690.h> 
             

#device *= 16


#fuses NOWDT, PROTECT, CPD, INTRC_IO
#fuses NOIESO, FCMEN, PUT, BROWNOUT             

                                                    
#use delay (internal = 4MHz)


#include "HMC1022.c" 
#include "lcd.c"   


const unsigned char symbol[8] = {0x04, 0x0A, 0x0A, 0x04, 0x00, 0x00, 0x00, 0x00};  


void setup(); 
void lcd_symbol();


void main()
{
    float H = 0.0;
    
    setup();
    
    while(TRUE)               
    {          
        lcd_putc("\f");
        
        H = read_heading();
                                 
        if(H < 0.0)
        {
            lcd_gotoxy(2, 1);
            lcd_putc("Compass Error!");
        }
        else 
        {                     
            lcd_gotoxy(4, 1); 
    		lcd_putc("Heading  N"); 
    		lcd_gotoxy(12, 1);                          
    		lcd_putc(0);  
            lcd_gotoxy(7, 2);        
    		printf(lcd_putc, "%03.1g", h); 
        } 
                
        delay_ms(1000);                   
    };              
}                           
      

void setup()                            
{                        
    disable_interrupts(GLOBAL);    
    setup_WDT(WDT_off);
    setup_oscillator(OSC_4MHz);
    setup_comparator(NC_NC_NC_NC); 
    setup_ADC(ADC_off);           
    setup_ADC_ports(no_analogs);
    setup_SPI(SPI_disabled | SPI_SS_disabled); 
    setup_CCP1(CCP_off);
    setup_timer_0(T0_internal);    
    setup_timer_1(T1_disabled);             
    setup_timer_2(T2_disabled, 255, 1);
    set_timer0(0);
    set_timer1(0);
    set_timer2(0);
    lcd_init();
    lcd_putc("\f");
    lcd_symbol();
} 


void lcd_symbol()                    
{
	unsigned char i=0;
	lcd_send_byte(0, 0x40); 
	for(i = 0; i < 8; i += 1)
	{
	    lcd_send_byte(1, symbol[i]);
	}
	lcd_send_byte(0, 0x80);    
} 
