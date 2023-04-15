#include <18F452.h>  


#device *=16

                                                                       
#fuses NOWDT, PUT, H4, BROWNOUT, BORV42, NOLVP, NODEBUG, NOCPB, STVREN, CPD
#fuses PROTECT, NOWRT, NOWRTB, NOWRTC, NOWRTD, NOEBTR, NOEBTRB, NOOSCSEN


#use delay(clock = 40M)    

                                                                                 
#include "lcd.c"    

                                        
#define mode_button			input(pin_C0)   
                                              
#define C_cal_in_nF	    	100.0 
#define L_cal_in_uH	    	100.0              
#define pi					3.142                                                                                                                                              
#define cal_delay			1000   
                                                                             
#define scaling_factor_c	((10.0 / (4.0 * pi * pi * L_cal_in_uH)))    
#define scaling_factor_l	((10.0 / (4.0 * pi * pi * C_cal_in_nF)))          
                                                                                              
                                                                                                                                                     
unsigned int32 overflow_count = 0;                                                      
unsigned int32 pulse_ticks = 0;                                                   
unsigned int16 start_time = 0;  
unsigned int16 end_time = 0;    

                                   
                                                                                              
void setup(void);                                                                             


#int_TIMER1                                                                                                    
                                 
void TMR_ISR(void)                 
{                                                                                         
   overflow_count++;      
}              
                                                                                                        

#int_CCP1                                                
                                                 
void CCP_ISR(void)
{
   end_time = CCP_1;                                                                                         
   pulse_ticks = ((65536 * overflow_count) + end_time - start_time);
   start_time = end_time; 
   overflow_count = 0;
}



void main(void)  
{
    short calibration_done = 0;                                                                                           
    unsigned char mode = 0;        
    unsigned long t = 0;      
    double ref = 0.0; 
    double value = 0.0;
                                                                                         
    setup();                                                                                     
                                                                
    
    while(TRUE)                          
    {                                                                                                                                                                                                                    
        t = (pulse_ticks);  
        value = ((double)t * (double)t);
         
        if(mode_button == FALSE)
        {
           delay_ms(60);                                             
           while(mode_button == FALSE);    
           calibration_done = 0; 
           mode++;
           
           if(mode > 1)
           {
               mode = 0;
           }
        }  
        
                             
        if(calibration_done == 0) 
        {
            lcd_putc("\f");	              
            lcd_gotoxy(1, 1);
			lcd_putc("Calibrating....");                                                               
			lcd_gotoxy(1, 2);              
			lcd_putc("Place no part.");                                                                 
			delay_ms(cal_delay);        
			lcd_putc("\f");    
            
            if(mode == 0)
            {   
                ref = (value * scaling_factor_c);   
                lcd_gotoxy(1, 1);  
                lcd_putc("C.ref/nF:");         
                lcd_gotoxy(1, 2);
                printf(lcd_putc, "%3.1g   ", ref);   
                  
            }
            
            if(mode == 1)            
            {                                             
            	ref = (value * scaling_factor_l);
                lcd_gotoxy(1, 1);  
                lcd_putc("L.ref/uH:");        
                lcd_gotoxy(1, 2);
                printf(lcd_putc, "%3.1g   ", ref);   
            }  
            
            delay_ms(cal_delay);        
			lcd_putc("\f");
                                                 
            calibration_done = 1;
        } 
        
        else
        {   
            lcd_gotoxy(1, 1);  
            
            switch(mode)
            {
                case 1:
                {
                    value = (value * scaling_factor_c);  
                	lcd_putc("Ind./uH:");       
                                               
                    break;
                }   
                                         
                default:
                {   
                    value = (value * scaling_factor_l);   
                	lcd_putc("Cap./nF:");        
                                                                                                                                
                    break;
                }
            }
            
            value -= ref;
                     
            if((value < 0) || (value > 1000))             
            {              
               value = 0;
            }  
                         
            lcd_gotoxy(1, 2);                               
            printf(lcd_putc, "%3.1g         ", value);   
        }
      
        delay_ms(100);                   
    }; 
}


void setup(void)                                                                                                                                                             
{                         
	setup_wdt(WDT_OFF);  
	setup_adc(ADC_OFF);
	setup_adc_ports(NO_ANALOGS);                                                                                     
	setup_spi(SPI_DISABLED);    
	setup_psp(PSP_DISABLED);                  
	setup_ccp1(CCP_CAPTURE_RE);
	setup_ccp2(CCP_OFF);
	setup_low_volt_detect(LVD_43);                                                
	setup_timer_0(T0_OFF | T0_8_BIT); 
	setup_timer_1(T1_INTERNAL);  
	setup_timer_2(T2_DISABLED, T2_DIV_BY_1, 16); 
	setup_timer_3(T3_DISABLED);  
    set_timer0(0);
    set_timer1(0);  
    set_timer2(0);                                            
    set_timer3(0);
	enable_interrupts(INT_CCP1);       
	enable_interrupts(INT_TIMER1);
	enable_interrupts(global);  
	lcd_init();
    lcd_putc("\f");	                             
}                                                                                        
