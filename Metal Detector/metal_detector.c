#include <12F675.h>


#device *=16
#device ADC=10 


#fuses NOMCLR, WDT, PROTECT
#fuses CPD, PUT, BROWNOUT, LP  
                         
                                   
#use delay(clock=4MHz) 
                             

#BYTE OPTION_REG = 0x081                 


#define MAXTRY  10    


unsigned int8   ctr=0;          
unsigned int8   previous=0;      
unsigned int8   calibr=0;        
unsigned int8   restarts=0;    
int1 en=0;               



void main(void)
{
    disable_interrupts(global);
    setup_ADC(ADC_off);
    setup_ADC_ports(no_analogs);
    setup_comparator(NC_NC_NC_NC);
                                
    if(restart_cause()==NORMAL_POWER_UP)
    {
                restarts=0;
                calibr=1;
    }                                   
                          
    if(restarts<255) 
    {
        restarts+=1;
    }   
    
    if((previous^ctr)>calibr)          
    {
        output_bit(pin_A0,en); 
                                             
        if(restart_cause()==WDT_TIMEOUT)
         {                
                if(restarts<MAXTRY)
                {
                    calibr<<=1;
                    delay_ms(4);
                }
         }                
         else
         {
            output_bit(pin_A0,0);                                          
         }                 
     }
     
     previous=ctr;      
     
     if(restarts>MAXTRY)
     {
        output_bit(pin_A1,0);
        en=1;
     }     
     
     else                                       
     {
        output_bit(pin_A1,1);
        en=0 ;             
     }     
                                
     setup_WDT(WDT_18MS); 
     //OPTION_REG=0xF9;
     ctr=0; 
     
     while(TRUE)
     {
            ctr+=1;
     }
}                                       
