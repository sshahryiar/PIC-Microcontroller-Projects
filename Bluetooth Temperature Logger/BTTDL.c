#include <12F683.h>

                         
#device *= 16
#device ADC = 10


#fuses NOWDT, PROTECT, CPD, PUT, INTRC_IO, BROWNOUT, MCLR, FCMEN, NOIESO 
                                                                          

#use delay (internal = 8MHz)                       
#use RS232(baud = 9600, xmit = PIN_A4, rcv = PIN_A5, bits = 8, parity = N, stop = 1)     
                                                      
                                                                         
#define LED     pin_A1               
#define CMD     input(pin_A2)    
                            
#define load    63536                           
#define limit   10000                                    
                                                  
                     
short rec = 0;                                  
short command = 0;      
register unsigned long ms = 0;
register unsigned long t = 0;            
                        
                             
void setup(); 
unsigned long average_t();
                                           
#int_timer1    
               
void Timer_1_ISR()                                          
{                           
    set_timer1(load);        
    ms += 1;                     
    if(ms >= limit)                                                                                             
    {                                  
        output_toggle(LED);
        rec = 1;  
        t += 10;   
        ms = 0;     
    }
    
}


void main()                           
{                                
    register float temp = 0.0; 
    
    setup();      
    
    while(TRUE)
    {                            
         temp = average_t();
         temp *= 0.489;                 

         if(CMD)                                              
         {                        
                  delay_ms(60);     
                  output_high(LED);                     
                  while(CMD);
                  command ^= 1;                   
                  
                  if(command)                                                                 
                  {                        
                         puts("\n<<<< Bluetooth Temperature Logger >>>>\n\r");
                         puts("\n >>>> Started <<<< \n\r");
                         delay_ms(100);
                         setup_timer_1(T1_internal);                
                         set_timer1(load);                   
                         ms = 0;                                 
                         t = 0;          
                         rec = 0;
                         output_low(LED);     
                  }    
                                    
                  else                           
                  {            
                         puts("\n >>>> Stopped <<<< \n\r"); 
                         delay_ms(100);      
                         setup_timer_1(T1_disabled);
                         set_timer1(load);                         
                         ms = 0; 
                         t = 0; 
                         rec = 0;
                         output_low(LED); 
                  }      
         }                                        
         
         
                            
         if(rec)               
         {                                                                                 
            printf("\nTime: %Lu sec    Temperature: %3.2g'C\r", t, temp);    
            delay_ms(100);                   
            rec = 0; 
         }                                     
    }                                       
}                                                
                                  
                        
void setup()
{                             
    setup_oscillator(OSC_8MHz); 
    setup_WDT(WDT_off);                        
    setup_timer_0(0 | 0 | 0);
    setup_timer_1(T1_disabled);
    setup_timer_2(T2_disabled, 255, 1);  
    set_RTCC(0);
    set_timer1(load);
    set_timer2(0);
    setup_CCP1(CCP_off);                                                           
    setup_comparator(NC_NC_NC_NC);        
    setup_ADC(ADC_clock_div_16);
    setup_ADC_ports(sAN0);     
    set_ADC_channel(0);             
    output_high(LED);   
    delay_ms(600);                         
    output_low(LED);                        
    delay_ms(600);     
    enable_interrupts(global);
    enable_interrupts(int_timer1); 
}                     
                                                                            

unsigned long average_t()
{
    register unsigned long tmp = 0;
    unsigned char samples = 0;
    while(samples < 16)             
    {                                             
        read_ADC(ADC_start_only);                          
        while(!ADC_done());
        tmp += read_ADC(ADC_read_only);    
        samples += 1;                  
    }            
    tmp >>= 4;  
    return tmp;                        
}                                  
