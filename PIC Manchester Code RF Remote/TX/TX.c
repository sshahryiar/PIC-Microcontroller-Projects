#include <12F629.h> 


#device *= 16


#fuses NOWDT, PROTECT, CPD, INTRC_IO, PUT, BROWNOUT, NOMCLR


#use delay (internal = 4MHz)
#use RS232 (xmit = pin_A5, rcv = pin_A4, baud = 1200, stop = 1, parity = N)  
                                                     

#define BT1		input(pin_A0)     
#define BT2		input(pin_A1)
#define BT3		input(pin_A2)    
#define BT4		input(pin_A3)


void setup();
void send_data(unsigned char value);
              

void main()
{
	setup();
   	
	while(TRUE)
	{         
		if(BT1)
		{   
			while(BT1);
			send_data(0x04);    
		}           
		if(BT2)
		{   
			while(BT2);       
			send_data(0x06);   
		}           
		if(BT3)
		{   
			while(BT3);
			send_data(0x09);
		}         
		if(BT4)
		{   
			while(BT4);
			send_data(0x11);            
		}
	};    
}


void setup()
{        
	disable_interrupts(global);
	setup_comparator(NC_NC_NC_NC);
	setup_timer_0(T0_internal);   
	setup_timer_1(T1_disabled);  
	set_timer0(0);      
	set_timer1(0);    
} 


void send_data(unsigned char value)
{
	unsigned char s = 0;   	
	unsigned char hb = 0;
	unsigned char lb = 0;  
	unsigned long temp = 0;                      
   	
	for(s = 0; s < 8; s++)
	{
		temp <<= 2;
		if((value & 0x80) != 0)
		{          
		    temp |= 0x1;
		}  
		else
		{
			temp |= 0x2;
		}
		value <<= 1;
	}              
	                                                            
	hb = ((temp & 0xFF00) >> 8); 
	lb = (temp & 0x00FF);
   	
	for(s = 0; s < 2; s++)
	{    
		putc(0x00);
		putc(0x55);                    
		putc(hb);                                                  
		putc(lb); 
		putc(hb & lb);                                                                                                              
		putc(0xAA);     
		delay_ms(40);                 
	} 
}                                                                                      
