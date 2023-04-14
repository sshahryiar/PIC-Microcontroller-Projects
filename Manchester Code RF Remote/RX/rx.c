#include <16F684.h> 


#device *= 16


#fuses NOWDT, INTRC_IO, PROTECT, CPD, NOMCLR, FCMEN


#use delay (internal = 4MHz)  


#define LED1        pin_C0
#define LED2        pin_C1
#define LED3        pin_C2    
#define LED         pin_C3
                                     
#define RX          input(pin_C4) 

#define sample_time	900 
                                    
                                   
void setup();               


void main()
{                
    short received = FALSE; 
    short rx_data[22];
    unsigned char b = 0;
    unsigned long dat = 0;  
    
    setup();                         
                                          
    while(TRUE)                 
    {                                
    	if(RX)
    	{       
    		 rx_data[0] = 0x00;   
    		 for(b = 1; b < 22; b++) 
    		 {               
    		     rx_data[b] = RX;                 
    		     output_bit(LED, rx_data[b]);             
    		     delay_us(sample_time);  
    		 }   
    		 
    		 received = TRUE;          
    	}      
    	                                   
    	if(received == TRUE)
    	{      
    		for(b = 1; b < 22; b += 2)
    		{                             
            	dat <<= 1;
            	              
            	if((rx_data[b - 1] == 0) && (rx_data[b] == 1))
            	{
            		dat |= 1;
            	}                                             
            	if((rx_data[b - 1] == 1) && (rx_data[b] == 0)) 
            	{
            		dat |= 0;
            	}                         
    		}                       
    	   	
    		memset(rx_data, 0, sizeof(rx_data));
    		dat &= 0x00FF;    
    		output_low(LED);  
    		received = FALSE; 
    	}      
    	if(dat > 0)
    	{
	    	if(dat == 4)         
	    	{ 
	    		output_toggle(LED1);
	    	}
	    	if(dat == 9)
	    	{
	    		output_toggle(LED2);
	    	}                       
	    	if(dat == 16)
	    	{               
	    		output_toggle(LED3);
	    	} 
	       	
	    	dat = 0;
    	}              
    };                                
}                             

                                 
void setup()                       
{                                
    disable_interrupts(GLOBAL);
    setup_WDT(WDT_off); 
    setup_oscillator(OSC_4MHz);                                                                           
    setup_comparator(NC_NC_NC_NC);
    setup_ADC(ADC_off);
    setup_ADC_ports(no_analogs);
    setup_CCP1(CCP_off);
    setup_timer_0(T0_internal);
    setup_timer_1(T1_internal); 
    setup_timer_2(T2_disabled, 255, 1);
    set_timer0(0); 
    set_timer1(0);                 
    set_timer2(0);   
}                     

