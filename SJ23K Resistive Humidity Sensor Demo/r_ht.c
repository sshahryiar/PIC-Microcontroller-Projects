#include <16F877A.h>

               
#device *= 16
#device ADC = 10


#fuses NOWDT, PROTECT, CPD, NOLVP, HS
#fuses NOBROWNOUT, NODEBUG, NOWRT, PUT 

                                                                                                     
#use delay (clock = 10MHz) 


#include "lcd.c" 
#include <math.h>


#define ADC_Max      1023.0
#define fixed_R      10000


const unsigned char symbol[8] = {0x00 ,0x0E, 0x0A ,0x0E, 0x00, 0x00, 0x00, 0x00};

              
void setup(); 
void lcd_symbols();
unsigned long adc_avg(unsigned char channel);
float get_temperature(void);
float get_resistance(void);
float get_RH(float resistance, float temperature);
       

void main()  
{                                                   
   float t = 0.0;
   float r = 0;
   float rh = 0.0;
      
   setup();                                                      
                                                            
   while(TRUE)           
   {                            
      r = get_resistance();
      t = get_temperature();
      rh = get_RH(r, t);
      
      lcd_gotoxy(13, 1);
      printf(lcd_putc, "%3.1g  ", rh);   
      lcd_gotoxy(13, 2);
      printf(lcd_putc, "%3.1g  ", t);
      delay_ms(100);
   };                       
}  


void setup()
{                                                                                               
   disable_interrupts(global);              
   setup_timer_0(T0_internal); 
   setup_timer_1(T1_disabled);
   setup_timer_2(T2_disabled, 255, 1);
   set_timer0(0);
   set_timer1(0); 
   set_timer2(0);                                                           
   setup_CCP1(CCP_off);
   setup_CCP2(CCP_off);
   setup_psp(PSP_disabled);                                
   setup_comparator(NC_NC_NC_NC); 
   setup_SPI(SPI_disabled);                   
   port_B_pullups(FALSE);  
   setup_ADC(ADC_clock_internal);
   set_analog_pins(AN0_AN1_AN3);  
   lcd_init();
   lcd_symbols(); 
   lcd_putc("\f");      
   lcd_gotoxy(1, 1);
   lcd_putc("RH/%:"); 
   lcd_gotoxy(1, 2);  
   lcd_putc("T/ C:");  
   lcd_gotoxy(3, 2);
   lcd_putc(0);
} 


void lcd_symbols() 
{
   unsigned char s = 0;
   
   lcd_send_byte(0, 0x40); 
   for(s = 0; s < 8; s++)        
   {                           
        lcd_send_byte(1, symbol[s]);          
   }
   lcd_send_byte(0, 0x80);
}   


unsigned long adc_avg(unsigned char channel)
{
    unsigned char samples = 16;
    unsigned long avg = 0;
    
    set_adc_channel(channel);
    delay_ms(2);
    
    while(samples > 0)
    {
        read_adc(ADC_START_ONLY);
        while(!ADC_done());
        avg += read_adc(ADC_read_only);
        samples--;
    };
    
    avg >>= 4;
    
    return avg;
}


float get_temperature(void)
{
    float t = 0.0;
    
    t = adc_avg(1);
    t *= 0.48876;
    
    return t;
}
            
                           
float get_resistance(void)
{
    unsigned long adc_value = 0;
    float r = 0;
    
    adc_value = adc_avg(0);
    
    r = ((ADC_Max / adc_value) - 1.0);
    r = (fixed_R / r);
    
    return r;
}


float get_RH(float resistance, float temperature)
{
   const float tab_A[11] = {373.01, 369.64, 402.15, 405.41, 404.26, 391.14, 394.88, 385.13, 386.62, 373.39, 371.05};
   const float tab_B[11] = {0.163, 0.166, 0.179, 0.183, 0.187, 0.188, 0.192, 0.193, 0.198, 0.199, 0.202};
   
   unsigned char index = 0;
   float rh = 0.0;
   
   index = (temperature / 5);
   
   rh = pow(resistance, -tab_b[index]);
   rh *= tab_A[index];
    
  return rh; 
}
