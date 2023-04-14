#include <16F887.h>


#device *= 16
                                                                       

#fuses HS, PUT, BROWNOUT, NOWDT, PROTECT, BORV40, CPD
#fuses NOMCLR, FCMEN, NOLVP, NODEBUG, NOIESO, NOWRT
                                                    
                                                               
#use delay (clock = 10MHz) 


#include "sound.c"


#define tmr0_load          0           
#define tmr1_load          32767                                   
                                                           
#define alt                input(pin_A0)  
#define inc                input(pin_A1)  
#define set                input(pin_A2) 
                                          
                                    
short toggle = 0;
 
unsigned char s = 0;
unsigned char min = 0;            
unsigned char hr = 0;            
unsigned char pos = 1;                       

const unsigned char num[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};  


void setup();
unsigned char get_buttons();     
unsigned char change_values(unsigned char value, unsigned char value_max); 
void settings();
                               


#int_RTCC 
          
void multiplex()
{     
         static unsigned char n = 0;   
         set_timer0(tmr0_load);
         switch(pos)          
         {
                  case 1:
                  {        
                           n = (hr / 10);
                           output_B(num[n]);
                           output_D(0x01);   
                           break;
                  }                    
                  case 2:
                  {        
                           n = (hr % 10);   
                           if(toggle)      
                           {
                                  output_B(num[n] | 0x80);
                           }
                           else
                           {
                                  output_B(num[n]);
                           }      
                           output_D(0x02);  
                           break;             
                  } 
                  case 3:
                  {
                           n = (min / 10);
                           output_B(num[n]);
                           output_D(0x04);    
                           break;
                  } 
                  case 4:
                  {
                           n = (min % 10);
                           if(toggle)      
                           {
                                  output_B(num[n] | 0x80);
                           }
                           else
                           {
                                  output_B(num[n]);
                           }   
                           output_D(0x08);    
                           break;                            
                  } 
                  case 5:
                  {                    
                           n = (s / 10);
                           output_B(num[n]);
                           output_D(0x10);    
                           break;
                  }                     
                  case 6:
                  {
                           n = (s % 10);
                           output_B(num[n]);
                           output_D(0x20);    
                           break;
                  } 
         } 
         pos++;
         if(pos > 6)
         {
                  pos = 1;
         }
}


#int_timer1

void RTC() 
{
         set_timer1(tmr1_load);  
         s += 1; 
         toggle = ~toggle;      
         if(s > 59)
         {                   
                  s = 0;
                  min += 1;
                  if(min > 59)
                  {                               
                           min = 0;
                           hr += 1;
                           if(hr > 23)
                           {
                                    hr = 0;
                           }
                  }
         }
         
}  
                                 

void main()
{                  
         setup();                    
         while(TRUE)
         {    
         		if(get_buttons() == 1)
         		{
         			settings();
         		}     
         };                          
}                                     

                                         
void setup()              
{                               
         disable_interrupts(global); 
         setup_comparator(NC_NC_NC_NC); 
         setup_ADC(ADC_off);
         setup_ADC_ports(no_analogs);                                                                           
			setup_SPI(SPI_disabled | SPI_SS_disabled);    
         setup_CCP1(CCP_off); 
         setup_CCP2(CCP_off);    
         setup_timer_0(T0_internal | T0_div_32 | T0_8_BIT);
         setup_timer_1(T1_EXTERNAL_SYNC |T1_DIV_BY_1 | T1_CLK_OUT); 
         setup_timer_2(T2_disabled, 255, 1); 
         set_timer0(tmr0_load);                 
         set_timer1(tmr1_load);             
         set_timer2(0);
         delay_ms(1000); 
         enable_interrupts(global); 
         enable_interrupts(int_timer0);  
         enable_interrupts(int_timer1);  
}                                                             


unsigned char get_buttons() 
{
		if(alt)
		{
			generate_tone(3000, 20);   
			return 1;        
		}       
		else if(inc)
		{
			generate_tone(2200, 20);    	   
			return 2;
		}              
		else if(set)
		{             
			generate_tone(3600, 20);    	                  
			return 3;
		}       
		else
		{
			return 0;
		}     
} 


unsigned char change_values(unsigned char value, unsigned char value_max)
{                        	   
			if(get_buttons() == 2)
			{         
				value += 1;
				delay_ms(60);               
			}       
			if(value > value_max) 
			{      
				value = 0; 
			}               
			return value;      
}


void settings()
{
		for(;;)
		{
			setup_timer_1(T1_disabled);
			toggle = 1;
			while(get_buttons() != 3)
			{
				hr = change_values(hr, 23);
			}
			delay_ms(200); 
			while(get_buttons() != 3)
			{
				min = change_values(min, 59);   
			}                            
			delay_ms(200); 
			while(get_buttons() != 3)
			{
				s = change_values(s, 59);
			}                     
			delay_ms(1000); 
			set_timer1(tmr1_load); 
			setup_timer_1(T1_EXTERNAL_SYNC |T1_DIV_BY_1 | T1_CLK_OUT);
			break;             
		}     
}
  

                                                                                                   
