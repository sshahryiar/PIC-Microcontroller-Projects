#include <12F675.h>


#device *= 16              
#device ADC = 10


#fuses NOWDT, PROTECT, CPD, NOMCLR, PUT, BROWNOUT, INTRC_IO   

                            
#use delay(internal=4MHz)    
                                  

#define DIN             pin_A0 
#define CLK             pin_A1         
#define STB             pin_A2                                                   
#define MOD             input(pin_A3)         


const unsigned long disp1[17]={0x0000, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000};
const unsigned long disp2[17]={0x0000, 0x0001, 0x0003, 0x0007, 0x000F, 0x001F, 0x003F, 0x007F, 0x00FF, 0x01FF, 0x03FF, 0x07FF, 0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF};    
                                                  
  
void setup();
void send_data(unsigned long value);
void disp(unsigned char val);
                             

void main()                                
{  
    unsigned long pos = 0;
    setup();                           
    while(TRUE)                
    {                     
        read_adc(adc_start_only);
        while(!adc_done());                                      
        pos = read_adc(adc_read_only);
        pos /= 63;
        disp(pos); 
    }                         
}  
                     

void setup()
{         
    disable_interrupts(global);    
    setup_comparator(NC_NC_NC_NC);  
    setup_ADC(ADC_clock_internal);
    setup_ADC_ports(sAN3);
    set_ADC_channel(3);           
    setup_timer_0(0 | 0 | 0);    
    setup_timer_1(T1_disabled); 
    set_timer0(0);
    set_timer1(0);                                         
}                                           
                                                                                  

void send_data(unsigned long value)     
{
    unsigned char s = 0;
    output_low(STB); 
    for(s = 0; s < 16; s++)     
    {                                                     
        output_bit(DIN, (0x8000 & value));                                           
        output_high(CLK);                
        value = (value << 1);                        
        output_low(CLK); 
    }      
    output_high(STB);               
}  


void disp(unsigned char val) 
{
		static short i;
		if(MOD)
        {                                       
            delay_ms(20);
            while(MOD);
            i ^= 1;         
        }                                    
        switch(i)             
        {                                     
            case 1:
            {
                send_data(disp1[val]);
                break;
            } 
            default:
            {
                send_data(disp2[val]);
                break;
            } 
        }  
        delay_ms(60); 
}
