#include <16F88.h>


#device ADC = 10 
#device *= 16

                                                 
#fuses INTRC
#fuses PUT           
#fuses NODEBUG   
#fuses PROTECT
#fuses CPD
#fuses FCMEN              
#fuses NOBROWNOUT
#fuses CCPB3
#fuses NOLVP                                           
#fuses NOWDT         
#fuses NOWRT
#fuses NOMCLR                          
#fuses NOIESO                            

                                
#use delay (internal = 8MHz)                       
                       

#include <lcd420.c>

                                           
void setup_mcu();          
float adc_avg(BYTE channel);   
float read_humidity();
float read_temperature(); 
void display();
                            
                                                          
void main()
{   
    setup_mcu();
    while(TRUE)    
    {                         
            display();  
    }                                        
}
                                                   
                                                             
void setup_mcu()                                       
{                            
    setup_oscillator(OSC_8MHZ);
    disable_interrupts(GLOBAL);          
    lcd_init();                    
    lcd_putc("\f");          
    setup_WDT(WDT_OFF);  
    setup_comparator(NC_NC_NC_NC);  
    setup_ADC(ADC_clock_div_16); 
    setup_ADC_ports(sAN0|sAN1|VSS_VDD); 
    set_Timer0(0);                    
    set_Timer1(0);          
    set_Timer2(0);                         
    setup_Timer_0(0|0|0); 
    setup_Timer_1(T1_DISABLED);
    setup_Timer_2(T2_DISABLED,0,1); 
    setup_SPI(SPI_SS_DISABLED);      
    setup_CCP1(CCP_OFF);              
    delay_ms(500);                                                    
}  

                    
float adc_avg(BYTE channel)                     
{                                           
    register unsigned int32 tmp = 0;    
    register unsigned long samples = 0;    
    register float avg = 0;             
    set_ADC_channel(channel);                        
    delay_ms(1);             
    for(samples = 0; samples < 512; samples++)
    {                    
        read_adc(ADC_START_ONLY);                    
        while(!adc_done());              
        tmp += read_adc(ADC_READ_ONLY); 
    }                       
    tmp >>= 9;        
    avg = (tmp * 0.004887585);        
    return avg;            
}        
                                 
                  
float read_humidity()
{
    register float HSM_20G = 0; 
    register float v = 0;       
    v = adc_avg(0);       
    HSM_20G = ((3.71 * v * v * v) - (20.65 * v * v) + (64.81 * v) - 27.44); 
    return HSM_20G;
}                             
                      

float read_temperature()                                  
{                             
    register float HSM_20G = 0;                                
    register float v = 0;       
    v = adc_avg(1);                 
    HSM_20G = ((5.26 * v * v * v) - (27.34 * v * v) + (68.87 * v) - 17.81); 
    return HSM_20G;   
}          
                                       
                                                             
void display()
{                            
    lcd_gotoxy(1,1); 
    printf(lcd_putc, "R.H: %02.2g ", read_humidity());   
    lcd_gotoxy(12,1);           
    lcd_putc("% ");                                   
    lcd_gotoxy(1,2);    
    printf(lcd_putc, "Tmp: %02.2g'C ", read_temperature());   
    delay_ms(100);
}                                                 
