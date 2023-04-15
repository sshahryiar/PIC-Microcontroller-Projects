#include <12F683.h>


#device *=16
#device ADC=10;
                                                                   
                                                      
#fuses INTRC_IO, PUT, CPD, NOMCLR, PROTECT, BROWNOUT, NOWDT, NOIESO, FCMEN  
                 

#use delay(internal = 8MHz)    

                                                                                  
#include "TM1637.c"    


#define D_CH        0
#define T_CH        1                                                               
                                                                      
                                                        
void setup(void); 
unsigned long adc_avg(unsigned char channel);
                                                           
                                                                                                                           
void main(void)                                                   
{                  
    unsigned char d = 0;
    unsigned long t = 0;
                      
    setup();                                                                                           
    
    while(TRUE)                 
    {                                                                         
        d = (adc_avg(D_CH) >> 7);
        TM1637_send_command(TM1637_CMD_SET_DSIPLAY | d | TM1637_SET_DISPLAY_ON);
                
        t = adc_avg(T_CH);                                          
        t = (t / 0.61384);                                                                                                                       
                                                                             
        if(t > 999)                                                                                                                    
        {
            TM1637_display_segments(0, 0x40, 0);                              
            TM1637_display_segments(1, 0x40, 0);                   
            TM1637_display_segments(2, 0x40, 0);    
        }                                                                                             
          
        else
        {
            TM1637_display_segments(0, seg_data[(t / 100)], 0);
            TM1637_display_segments(1, seg_data[((t % 100) / 10)], 1);                                                 
            TM1637_display_segments(2, seg_data[(t % 10)], 0);         
        }   
                                                                                        
        TM1637_display_segments(3, 0x39, 0);                                                              
        
        delay_ms(600);                                                                                   
    };
}                                            


void setup(void)
{                                                                  
    disable_interrupts(global);            
    setup_oscillator(OSC_8MHZ);
    setup_comparator(NC_NC_NC_NC);
    setup_ccp1(CCP_OFF);                                                            
    setup_timer_0(RTCC_INTERNAL | RTCC_DIV_2 | RTCC_8_BIT);
    set_timer0(0);               
    setup_timer_1(T1_DISABLED);
    set_timer1(0); 
    setup_timer_2(T2_DISABLED, 255, T2_DIV_BY_1);
    set_timer2(0);                
    setup_vref(VREF_HIGH);                                                                
    setup_adc(ADC_CLOCK_INTERNAL);
    setup_adc_ports(sAN0 | sAN1);
    TM1637_init();
    delay_ms(10);
}  

                                                                       
unsigned long adc_avg(unsigned char channel)                                                       
{                                                                                   
    unsigned char s = 128;                    
                                    
    unsigned long long value = 0;  
                                   
    set_adc_channel(channel);  
    delay_ms(10);                                 
                                       
    while(s > 0)          
    {                           
        read_adc(adc_start_only);
        while(!adc_done());                 
        value += read_adc(adc_read_only); 

        s--;
    }; 
    
    value >>= 7;
             
    return value;                                                                                                                               
}                                                                             


                                                                                                                                     
