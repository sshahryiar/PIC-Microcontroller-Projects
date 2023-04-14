#include <12F675.h>  


#device *= 16
#device ADC = 8
                        
                                                            
#fuses INTRC_IO, NOWDT, PROTECT, NOMCLR, BROWNOUT, PUT, CPD    
                                                                      

#use delay (internal = 4MHz)  
                                                           
                                                                                                    
#include "DHT11.c"                    
#include "TM1640.c"                      
                                              
                         
void setup();            
                                            
                                                                                                                                                                   
void main()                                                                                            
{                                          
   short state = 0;  
   unsigned char tmp = 0;
   unsigned char brightness = 0;
                          
   setup();     
   
   while(TRUE)                                                                         
   {                                         
	  state = get_data();
      
      read_adc(adc_start_only);
      while(!adc_done());
      brightness = read_adc(adc_read_only);
      brightness >>= 5;
      
      if(brightness > 7)
      {
         brightness = 7;
      } 
              
      TM1640_send_command(0x88 | brightness);
      
      switch(state)
      {
            case 1:                                                          
            {                  
                TM1640_send_data(5, (font[37]));
                TM1640_send_data(6, (font[82])); 
                TM1640_send_data(7, (font[82]));  
                TM1640_send_data(8, (font[79])); 
                TM1640_send_data(9, (font[82] | font[14]));        
                delay_ms(600);
                TM1640_clear_display();  
            	delay_ms(900);  
            	break;
            }                            
            default:                                     
            {                          
               TM1640_clear_display();  
               TM1640_send_data(0, (font[82] | font[14]));
			   TM1640_send_data(1, (font[72] | font[14]));      
			   TM1640_send_data(2, (font[0]));          
			                            
			   tmp = (values[0] / 100);   
			   if(tmp >= 1)  
			   {
					TM1640_send_data(3, (font[16 + tmp])); 
			   }
			   else
			   {    
					TM1640_send_data(3, (font[0]));   
			   }    
			   
			   tmp = ((values[0] / 10) % 10);      
			   if(tmp >= 1)  
			   {
					TM1640_send_data(4, (font[16 + tmp])); 
			   }
			   else
			   {                     
					TM1640_send_data(4, (font[0]));   
			   }   
			   
			   tmp = (values[0] % 10);   
			   TM1640_send_data(5, (font[16 + tmp] | font[14]));   
			   tmp = (values[1] / 100);              
			   TM1640_send_data(6, (font[16 + tmp]));     
			                                                           
			   TM1640_send_data(7, (font[0]));     
			   TM1640_send_data(8, (font[0]));     
			   TM1640_send_data(9, (font[0]));   
			   TM1640_send_data(10, (font[84] | font[14])); 
			   TM1640_send_data(11, (font[0]));                        
			   
			   tmp = (values[2] / 100);
			   if(tmp >= 1)  
			   {
					TM1640_send_data(12, (font[16 + tmp])); 
			   }
			   else
			   {    
					TM1640_send_data(12, (font[0]));   
			   }    
			                                                
			   tmp = ((values[2] / 10) % 10);  
			   if(tmp >= 1)  
			   {
					TM1640_send_data(13, (font[16 + tmp])); 
			   }
			   else
			   {    
					TM1640_send_data(13, (font[0]));   
			   }    
			   
			   tmp = (values[2] % 10);                                             
			   TM1640_send_data(14, (font[16 + tmp] | font[14]));   
			   tmp = (values[3] / 100);              
			   TM1640_send_data(15, (font[16 + tmp]));   
			   
               break;                    
            }                   
      }     
      delay_ms(1000); 
   }                                                         
}                  

                                                                                                            
void setup()
{                                     
   disable_interrupts(global);                   
   setup_comparator(NC_NC_NC_NC); 
   setup_ADC(ADC_clock_div_16);
   setup_ADC_ports(sAN0 | VSS_VDD);  
   set_ADC_channel(0);
   setup_timer_0(T0_internal | T0_8_bit);
   setup_timer_1(T1_disabled);      
   set_timer0(0);                                     
   set_timer1(0);   
   DHT11_init();   
   TM1640_init(brightness_100_pc);     
}
                            
