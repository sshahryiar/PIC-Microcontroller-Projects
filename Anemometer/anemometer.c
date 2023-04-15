#include <16F690.h>

            
#device ADC = 10


#fuses NOWDT, INTRC_IO, PROTECT, PUT, CPD, BROWNOUT, FCMEN, NOIESO, NOMCLR


#use delay (internal = 4MHz)


#include "lcd.c"


#define no_of_samples  16
                     

void setup();
unsigned long adc_avg();   
float get_wind_speed(float value); 


void main()              
{          
    unsigned char s = 0;
    
    float current_wind_speed = 0;
    float avg_wind_speed = 0;
    float reading[no_of_samples];
    float sum = 0;  
    
    for(s = 0; s < no_of_samples; s++)
    {
        reading[s] = 0;
    }
    
    setup(); 
    
    lcd_gotoxy(2, 1);
    lcd_putc("PIC Anemometer");   
    delay_ms(4000);
    lcd_putc("\f");
    lcd_gotoxy(1, 1); 
    lcd_putc("Wind Speed m/s:");  
    lcd_gotoxy(7, 2); 
    
    while(TRUE)         
    {                              
             current_wind_speed = adc_avg();
             current_wind_speed = get_wind_speed(current_wind_speed);
             
             sum -= reading[s];
             reading[s] = current_wind_speed;
             sum += reading[s];
             s++;    
             
             if(s >= no_of_samples)
             {                                 
                  s = 0;
             }                     
                                
             avg_wind_speed = (sum / no_of_samples);   
                                                           
             lcd_gotoxy(1, 2);                
             printf(lcd_putc, "%2.1g ", current_wind_speed); 
             lcd_gotoxy(13, 2);                
             printf(lcd_putc, "%2.1g ", avg_wind_speed);    
             delay_ms(100);
    };                              
}


                             
void setup()
{                            
    disable_interrupts(global);    
    setup_oscillator(OSC_4MHz);        
    setup_WDT(WDT_off);                 
    setup_comparator(NC_NC_NC_NC);  
    setup_SPI(SPI_disabled | SPI_SS_disabled); 
    setup_ADC(ADC_clock_div_8);
    setup_ADC_ports(sAN0);  
    set_ADC_channel(0); 
    setup_CCP1(CCP_off);                       
    setup_timer_0(T0_internal | T0_8_BIT);   
    setup_timer_1(T1_disabled);
    setup_timer_2(T2_disabled, 255, 1);
    set_timer0(0);     
    set_timer1(0);
    set_timer2(0);    
    lcd_init();
    lcd_putc("\f");
} 


unsigned long adc_avg() 
{
    unsigned char samples = no_of_samples;
    unsigned long avg = 0;
    
    while(samples > 0)                
    {
        //read_adc(adc_start_only);
        //while(adc_done() == FALSE);
        avg += read_adc();//adc_read_only);
        samples--;   
    };
                   
    avg >>= 4;
    return avg;
}          


float get_wind_speed(float value)
{                                  
    value -= 1;
    value /= 34.0; 
 
    return value;
} 
