#include <16F684.h>

                                               
#fuses INTRC_IO, PROTECT, PUT, BROWNOUT ,CPD, NOWDT, FCMEN, NOMCLR, NOIESO
    
                         
#use delay(clock = 8MHz)  
#use fast_IO(C)
                               
                                            
#include "lcd.c"             
                                     
                                                                   
#define reload				170
#define pwm_max				100                                     
#define pwm_min				  1    
                                          
#define LED				 pin_C5  
#define SSR				 pin_C4         
                                            
#define inc		  input(pin_A0)
#define dec		  input(pin_A1)          
                                      
           
short zcd = FALSE;                                    
                                      
unsigned char PWM = 50;       
unsigned char cnt = 0;  

unsigned long long t = 0;
                               
const unsigned char symbols[8]= 
{
    0, 14, 14, 14, 14, 14, 0, 0
};                          
                             
               
void lcd_symbols();
                                 
                                       
#int_RTCC                                                     
                                               
void pwm_generate()                         
{                                
    set_RTCC(reload);                        
    cnt += 1;
    if(cnt > PWM)
    {              
        output_high(LED);
        output_high(SSR);                     
    }                        
}                                
                             
                            
#int_ext                    

void zero_crossing_detection()
{                                    
    zcd = ~zcd;
                                  
    output_low(SSR);
    output_low(LED);
    cnt = 0;           
      
    if(zcd)
    {              	
		set_timer1(0); 	
		setup_timer_1(T1_internal);
    }                                        
    else
    {                             
		t = get_timer1();
		setup_timer_1(T1_disabled);   
    }  
}                               
                                         
                                                                                                                             
void setup()                     
{                              
    setup_WDT(WDT_off);      
    setup_oscillator(OSC_8MHz);
    setup_comparator(NC_NC_NC_NC);
    set_TRIS_C(0x00); 
    output_C(0x00);            
    setup_adc(ADC_off); 
    setup_adc_ports(no_analogs);   
    setup_timer_0(T0_internal | T0_div_2 | T0_8_BIT);        
    setup_timer_1(T1_disabled);    
    setup_timer_2(T2_disabled, 255, 1);
    set_RTCC(reload);      
    set_timer1(0);             
    set_timer2(0);
    setup_CCP1(CCP_off);              
    ext_int_edge(L_to_H);  
    enable_interrupts(global);
    enable_interrupts(int_RTCC);
    enable_interrupts(int_ext);        
    lcd_init(); 
    lcd_putc("\f");
    lcd_gotoxy(1, 1);                          
	lcd_putc("Triac Controller"); 
	lcd_symbols();
}                                       
                                               
                                       
void main()
{                                                                                            
    unsigned char s = 0; 
    unsigned char n = 0; 
    
    float f = 0.0;        
    
    setup();
                          
    while(TRUE)                                   
    {                            
        if((t >= 12500) && (t <= 50000))
        {
       	f = (1000000.0 / t);
        }                                   
        else                 
        {
       	f = 0;
        }                   
        if(inc)               
        {                        
			PWM += 1; 
			delay_ms(40);            
        }
        if(dec)                   
        {                                            
			PWM -= 1;              
			delay_ms(40);
        } 
        
        if(PWM >= pwm_max)                                       
        {                    
			PWM = pwm_max;
        }    
        if(PWM < pwm_min)                
        {                                 
			PWM = pwm_min;               
        }     
          
        n = ((100 - PWM) / 20);
        for(s = 0; s < n; s += 1)
        {                                       
		lcd_gotoxy((5 + s), 2);
	    lcd_putc(0);  
        }
        for(s = n; s < 10; s += 1)
        {   
	lcd_gotoxy((5 + s), 2);
		lcd_putc(" ");                         
        }
              
        lcd_gotoxy(1, 2);             
        printf(lcd_putc, "%03u", (100 - PWM));    
        lcd_gotoxy(11, 2);                
        printf(lcd_putc, "%2.1gHz", f);       
        delay_ms(100);                                    
    }                                                
}


void lcd_symbols()  
{
   unsigned char i = 0;              
   lcd_send_byte(0,0x40); 
   for(i = 0; i < 8; i++)
   {
        lcd_send_byte(1,symbols[i]);
   }
   lcd_send_byte(0,0x80);
}                
                                                
                                
                                
