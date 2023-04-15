#include <16F690.h>            

                       
#device *= 16                      
#device ADC = 10                                        
                                                        
                      
#fuses INTRC_IO, NOWDT, FCMEN, PUT, NOIESO    
#fuses NOBROWNOUT, NOMCLR, PROTECT, CPD   
                                                              

#use delay (internal=8MHz)                                                                                                
#use rs232(baud=1200, rcv=PIN_B5, parity=n, bits=8, stop=1)                                                                
                                                      
                                      
#define red     PIN_C0                                                       
#define yellow  PIN_C1               
#define green   PIN_C2           
                             
                                 
void main()                                        
{      
    unsigned char a=0, b=0, c=0, d=0; 
    setup_oscillator(osc_8MHz);
    disable_interrupts(GLOBAL);                                      
    setup_comparator(NC_NC_NC_NC);      
    setup_ADC(ADC_off);        
    setup_ADC_ports(no_analogs);    
    output_C(0x00);
    setup_SPI(SPI_DISABLED);     
    setup_timer_0(0|0|0);        
    setup_timer_1(T1_disabled);        
    setup_timer_2(T2_Disabled,1,1);   
    setup_ccp1(CCP_off);  
    set_timer2(0);                                                 
    set_timer1(0);  
    set_timer0(0); 
    delay_ms(500); 
    for(;;)
    {                      
            if(kbhit())       
            {                                     
                a=getc();
                b=getc(); 
                c=getc();                              
                d=getc();
            }                 
            else            
            {                              
                if(a==83 && b==83 && c==73 && d==82)
                {
                    output_toggle(red);    
                    a=b=c=d=0; 
                }   
                else if(a==73 && b==82 && c==83 && d==83)
                {  
                    output_toggle(yellow);     
                    a=b=c=d=0; 
                }      
                else if(a==82 && b==73 && c==83 && d==83)
                {                        
                    output_toggle(green); 
                    a=b=c=d=0; 
                }                          
                else if(a==84 && b==73 && c==83 && d==83)
                {
                    output_c(0x00);    
                    a=b=c=d=0; 
                }                       
            }                            
    }                            
}                             
