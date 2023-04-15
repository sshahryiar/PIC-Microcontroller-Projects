#include <16F88.h> 

                                
#device *= 16 
#device ADC = 10
                                                        
                                              
#fuses HS, NOWDT, PUT, CPD, NOLVP, FCMEN, NOIESO                       
#fuses NOBROWNOUT, NOMCLR, PROTECT, CCPB3, NOWRT           
                                                                               
                                                       
#use delay (clock=20MHz)        
#use rs232(baud=1200, xmit=PIN_B2, parity=n, bits=8, stop=1)                                                                                                                 
                                                                                                                             
                                                                                                                      
#define BT1     input(PIN_B4)                                                                                                
#define BT2     input(PIN_B5)                           
#define BT3     input(PIN_B6)                                                   
#define BT4     input(PIN_B7)                   
                                          
#define LED     PIN_B0                      
                                                                                             
                                                                            
void main()                        
{                                        
    disable_interrupts(GLOBAL); 
    setup_ADC(ADC_off);
    setup_ADC_ports(no_analogs);
    setup_comparator(NC_NC_NC_NC);
    setup_SPI(SPI_DISABLED);
    setup_WDT(WDT_OFF);
    setup_timer_0(0|0|0);        
    setup_timer_1(T1_disabled);    
    setup_timer_2(T2_DIV_BY_1,130,1);   
    setup_ccp1(CCP_PWM);  
    set_pwm1_duty(263);                 
    set_timer1(0);   
    set_timer0(0);                             
    delay_ms(500);                 
    while(TRUE)                   
    {                    
        if(BT1)                 
        {                
            output_high(LED);
            delay_ms(50);
            while(BT1);
            putc(83);                       
            putc(83);    
            putc(73);
            putc(82);  
        }                         
        if(BT2)
        {
            output_high(LED);
            delay_ms(50); 
            while(BT2);
            putc(73);                       
            putc(82);    
            putc(83);
            putc(83); 
            delay_ms(20);  
        }   
        
        if(BT3)
        {
            output_high(LED);
            delay_ms(50);
            while(BT3);        
            putc(82);                       
            putc(73);    
            putc(83);              
            putc(83);
            delay_ms(20);
        }  
        
        if(BT4)
        {
            output_high(LED);
            delay_ms(50);
            while(BT4);     
            putc(84);                       
            putc(73);    
            putc(83);              
            putc(83);       
            delay_ms(20);                      
        }
        output_low(LED); 
    }                           
}                                   
                     
