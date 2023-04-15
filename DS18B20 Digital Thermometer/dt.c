#include <16F877A.h>


#device *= 16
#device ADC = 10

                                                                
#fuses XT, NOWDT, PROTECT, PUT, CPD, BROWNOUT, NOLVP, NODEBUG, NOWRT 
                                                             

#use delay (clock = 4MHz)   
#use fast_io(B)
#use fast_io(D)     


#include "DS18B20.c"   

                                         
#define chg_disp		!input(pin_C3)  


const unsigned char disp[0x0D] =                  
{0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x40, 0x39, 0x71};


unsigned char chr[4];

unsigned char n = 0;
                                                                                

void setup();  
                                                                                                               

#int_RTCC
                                                     
void timer_0_isr()
{   
	set_timer0(0x00); 
   	
	switch(n)
	{
	   case 0:
	   {
			output_D(chr[0]);
			output_B(0x00);
			break;      
	   } 
	   case 1:       
	   {
			output_D(chr[1]);                                      
			output_B(0x01);
			break;                                                                        
	   }                           
	   case 2:                                                                   
	   {
			output_D(chr[2]);
			output_B(0x02);
			break;                                                                              
	   }                                                                                      
	   case 3:
	   {                 
			output_D(chr[3]);
			output_B(0x03);
			break;
	   }                                      
	}   
	                                                                                             
	n++;
	if(n >= 4)
	{
	    n = 0; 
	} 
}                                
                                                                   
                                     
void main()
{                           
	short disp_type = FALSE;   	
	unsigned char num = 0;     
	float t = 0;     
	float t_previous = 25;
	signed long temp = 0;     

	setup();
	                 
	while(TRUE)                                                                  
	{   
		t = get_temperature();                                                        

		if((t >= (t_previous + 8)) || (t <= (t_previous - 8)))
		{                                         
			t = t_previous;    
		}                                       
                   
		if(t >= 100)    
		{                                                                          
		   t = 99.9; 
		}
		if(t <= -55)
		{                                                 
		   t = -55;	   	
		}  
	                                                                           
	    if(chg_disp)  
		{           
			while(chg_disp);                                                  
			disp_type ^= 1;
			                                    
		}   	   	
		                  
		if(disp_type)                                                   
		{    
			chr[3] = disp[0x0C];         
			temp = ((t * 18) + 320);    
		} 
		else               
		{                                                                                                
		    chr[3] = disp[0x0B];                                    
		    temp = (t * 10);                    
		} 
			                             
		if(t < 0)                                      
		{             
			chr[0] = disp[0x0A];
			num = (temp / 100);                                     
		    chr[1] = disp[num];
			num = ((temp / 10) % 10);    
			chr[2] = (disp[num] | 0x80);    
		}                            
		else
		{                                                                                         
		    num = (temp / 100);
		    chr[0] = disp[num];                                                      
			num = ((temp / 10) % 10);    
			chr[1] = (disp[num] | 0x80);                                                             
			num = (temp % 10);         
			chr[2] = disp[num];  
		}  
	   	
		t_previous = t;   
		delay_ms(100);
	};                         
}


void setup()
{                              
	disable_interrupts(GLOBAL); 
	set_TRIS_B(0xF8);   	
	set_TRIS_D(0x00);
	setup_ADC(ADC_off); 
	setup_CCP1(CCP_off); 
	setup_CCP2(CCP_off); 
	setup_ADC_ports(no_analogs);                                                  
	setup_comparator(NC_NC_NC_NC);              
	setup_SPI(SPI_disabled | SPI_SS_disabled); 
	setup_timer_0(T0_internal | T0_div_4 | T0_8_bit);  
	setup_timer_1(T1_disabled);             
	setup_timer_2(T2_disabled, 0xFF, 0x01);      
	set_timer0(0x00);                     
	set_timer1(0x0000);
	set_timer2(0x00); 
	enable_interrupts(GLOBAL);
	enable_interrupts(int_RTCC);
	init_DS18B20();
}                                                                                   
