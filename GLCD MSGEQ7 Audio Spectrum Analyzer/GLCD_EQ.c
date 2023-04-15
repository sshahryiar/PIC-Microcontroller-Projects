#include <18F4431.h>
                
                
#device *= 16 
#device ADC=10


#fuses HS, PUT, NOWDT, PROTECT, CPD, BROWNOUT, BORV45, NOWINEN
#fuses NOSTVREN, NOLVP, NODEBUG, NOCPB, NOWRT, NOWRTC, T1LOWPOWER
#fuses NOWRTB,  NOWRTD, NOEBTR, NOEBTRB, NOIESO, FCMEN, MCLR    


#use delay (clock = 10MHz)      
                    
                    
#include "HDM64GS12.c"             
#include "graphics.c"              


#define reset           pin_C0 
#define strobe          pin_C1
                                             
                                                                     
unsigned char msg[] = {"MSGEQ7 Audalyzer"};
unsigned char f_values[] = {"63       1k       16k"}; 

unsigned long spectrum_values[7];
   
                                                                                                 
void setup();      
void MSGEQ7_init();
void read_MSGEQ7();
void plot_data();
float map(float value, float x_min, float x_max, float y_min, float y_max);
                             

void main()                                                                                
{                        
    setup(); 
    
    while(TRUE)            
    {
        read_MSGEQ7();
        plot_data();        
    }                             
}                                                                                
                                                         
                                                  
void setup()                                                                   
{                                           
   disable_interrupts(global);  
   setup_WDT(WDT_off);
   port_b_pullups(FALSE);
   setup_spi(spi_ss_disabled|spi_disabled);
   setup_timer_0(T0_internal);    
   set_timer0(0);
   setup_timer_1(T1_disabled);                                                                         
   set_timer1(0);
   setup_timer_2(T2_disabled, 255, 1);
   set_timer2(0);
   setup_timer_5(T5_disabled); 
   setup_ccp1(ccp_off);       
   setup_ccp2(ccp_off);
   setup_power_pwm_pins(PWM_pins_disabled, PWM_pins_disabled, PWM_pins_disabled, PWM_pins_disabled);
   setup_power_pwm(PWM_disabled, 0, 0, 0, 0, 0, 0);
   setup_qei(QEI_disabled);
   setup_ADC_ports(sAN6);                        
   setup_ADC(ADC_clock_div_32);  
   set_ADC_channel(6);
   glcd_init(on);   
   glcd_fillscreen(0);           
                                               
   glcd_text57(17, 1, msg, 1, ON); 
   glcd_text57(1, 55, f_values, 1, ON); 
   
   MSGEQ7_init();
}              


void MSGEQ7_init()
{
   output_low(strobe);
   delay_ms(1);
   
   output_high(reset);
   delay_ms(1);
   
   output_low(reset);
   output_high(strobe);
   delay_ms(1);
}


void read_MSGEQ7()
{
   unsigned char s = 0;
   
   memset(spectrum_values, 0, sizeof(spectrum_values));
   
   output_high(reset);
   output_low(reset);

   for (s = 0; s < 7; s++)
   {
      output_low(strobe);
      delay_us(30);
      read_adc(ADC_start_only);
      while(ADC_done() == FALSE);
      spectrum_values[s] = read_adc(ADC_read_only);
      output_high(strobe);
      delay_us(1);    
   }
}


void plot_data()
{
   unsigned char s = 0;
   unsigned long h = 0;
   
      
   
   for(s = 0; s < 7; s++)
   {
       h = map((float)spectrum_values[s], 0, 1023, 50, 10);
       
       glcd_rect((1 + (s * 18)), 10, (17 + (s * 18)), 50, YES, NO);
       glcd_rect((1 + (s * 18)), 50, (17 + (s * 18)), h, YES, YES); 
       
       if(h > 12)
       {
            glcd_line((1 + (s * 18)), (h - 2), (17 + (s * 18)), (h - 2), ON);
       }
   }
}

                                  
float map(float value, float x_min, float x_max, float y_min, float y_max)   
{                         
    return (y_min + (((y_max - y_min) / (x_max - x_min)) * (value - x_min)));
}           
