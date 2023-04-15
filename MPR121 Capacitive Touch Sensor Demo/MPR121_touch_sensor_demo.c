#include <16F630.h> 


#device *= 16


#fuses INTRC_IO, PUT, PROTECT, NOWDT, NOMCLR, BROWNOUT, CPD


#use delay (clock = 4MHz)

                          
#include "MPR121.c"
                     

void setup();


void main()
{  
	unsigned char states = 0;   	
   	
	setup();
   	
	while(TRUE)
	{       
	    states = MPR121_read(ELE_L);
	    states &= 0x0F;
	    output_C(states);
	};                                
}                     
                                      
                                                                                                                            
void setup()
{    
	disable_interrupts(global);   
	setup_comparator(NC_NC_NC_NC);  
	set_TRIS_C(0xF0);
	setup_timer_0(T0_INTERNAL | T0_8_BIT); 
	setup_timer_1(T1_DISABLED); 
	set_timer0(0); 
	set_timer1(0);   
	MPR121_init();
}
                         
