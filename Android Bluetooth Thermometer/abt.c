#include <12F615.h> 


#device *= 16
#device ADC = 10
                       

#fuses NOWDT, PROTECT, INTRC_IO, NOMCLR, PUT, BROWNOUT   
                                   

#define LED			PIN_A1
#define TX_pin		PIN_A4
#define RX_pin		PIN_A5


#use delay (internal = 4MHz) 
#use RS232(BAUD = 9600, XMIT = TX_pin, RCV = RX_pin, STOP = 1, PARITY = N, BITS = 8)

               
void setup();
void adc_avg(unsigned long *value);
                     

void main()
{
	unsigned long tmp = 0;
	float t = 0;                     
	                                                     
	setup();
	                             
	while(TRUE)
	{           
		adc_avg(&tmp);
		t = tmp * 0.489;           
		printf(" ");     
		printf("%2.2f\n\r", t);  
		output_toggle(LED);              
		delay_ms(490);
	};       
}
                                                                                                       

void setup()
{
	disable_interrupts(global);
	setup_comparator(NC_NC_NC_NC);
	setup_ADC(ADC_clock_internal);
	setup_ADC_ports(sAN0);
	set_ADC_channel(0);
	setup_CCP1(CCP_PWM);
	setup_timer_0(T0_internal);  
	setup_timer_1(T1_disabled);   
	setup_timer_2(T2_disabled, 255, 1);
	set_timer0(0);  
	set_timer1(0); 
	set_timer2(0); 
	delay_ms(100);
} 
                               

void adc_avg(unsigned long *value)
{
	unsigned char s = 64;
	unsigned long avg = 0;
   	
	while(s > 0)
	{
		read_adc(adc_start_only);
		while(!adc_done());
		avg += read_adc(adc_read_only); 
		s--;
	}  
   	
	*value = (avg >> 6);
}
