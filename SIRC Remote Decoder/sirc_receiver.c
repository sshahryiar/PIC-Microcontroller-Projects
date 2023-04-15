#include <16F877A.h>   
                
               
#device *=16


#fuses NOWRT, CPD, NOLVP, NODEBUG, PUT
#fuses BROWNOUT, NOWDT, PROTECT, HS 
                          

#use delay(clock=10MHz)
#use fast_io(B)                                


#define SIRC_1_MIN_TIME    4187               
#define SIRC_1_MAX_TIME    4812                 
#define SIRC_0_MIN_TIME    2687      
#define SIRC_0_MAX_TIME    3312          

#define LED              pin_C0  
                                      
#include <lcd.c>    
#include "sound.c"
                                                                                               
                                                         
unsigned long SIRC_frames[13];
unsigned char SIRC_cnt=0;
short received=0;


void setup();
void start_SIRC_detection();
short decode_SIRC(unsigned char &addr, unsigned char &cmd); 
                            

#int_ext                                    
void IR_detected() 
{                    
      if(received) 
      {
            return;
      }  

      SIRC_frames[SIRC_cnt++]=get_timer1();
                             
      if(SIRC_cnt>=13) 
      {
            received=1;                        
      }                   
                                                 
      set_timer1(0);   
      enable_interrupts(INT_TIMER1);    
}
                                   

#int_timer1                     
void pulse_time_exceeded() 
{                                      
      disable_interrupts(INT_TIMER1);
}                                 
                                                
                                                
void main() 
{
      unsigned char addr=0;
      unsigned char cmd=0;
      short frame_received=0;                           
      setup();     
      start_SIRC_detection(); 
      while(TRUE)                              
      {                                                   
            if(received) 
            {                                                      
                  frame_received=decode_SIRC(addr,cmd);      
                  lcd_gotoxy(1,2); 
                  printf(lcd_putc,"ADDR:%2u  CMD:%2u ",addr,cmd);    
                  if(frame_received)  
                  {    
                        output_high(LED); 
                  }                            
                  else
                  {                                           
                        lcd_gotoxy(1,2);   
                        lcd_putc("    Unknown!    ");
                  }                                     
                  
                  generate_tone(3000, 50);  
                  output_low(LED);      
                  start_SIRC_detection();             
            }  
      }                       
}


void setup()
{  
      setup_comparator(NC_NC_NC_NC);
      setup_ADC_ports(NO_ANALOGS);
      setup_ADC(ADC_OFF);
      set_tris_B(0x01);                                      
      setup_SPI(SPI_DISABLED);
      setup_PSP(PSP_DISABLED);
      setup_CCP1(CCP_off);                                                           
      setup_CCP2(CCP_off);
      output_low(LED);    
      setup_timer_0(0|0|0);                  
      setup_timer_1(T1_Internal);    
      setup_timer_2(T2_DISABLED,255,1);
      set_timer0(0);
      set_timer1(0); 
      set_timer2(0);
      ext_int_edge(L_TO_H);          
      enable_interrupts(INT_EXT);                    
      enable_interrupts(GLOBAL); 
      lcd_init();   
      lcd_putc("\f");
      lcd_gotoxy(3,1);             
      lcd_putc("SIRC Decoder");  
      delay_ms(250);     
}
                            

void start_SIRC_detection() 
{       
      memset(SIRC_frames, 0, 13);
      SIRC_cnt=0;                               
      received=0;               
}                           


short decode_SIRC(unsigned char &addr, unsigned char &cmd)  
{ 
      unsigned char s=0;   
      unsigned char bit_mask=0;   
      unsigned char bits[13];
    
      addr=0;
      cmd=0;                 
    
      for(s=1;s<=12;s+=1)                                             
      {                                    
            if ((SIRC_frames[s]>=SIRC_1_MIN_TIME)&&(SIRC_frames[s]<=SIRC_1_MAX_TIME))
            {                                                        
                bits[s]=1; 
            }                           
            else
            {
              if ((SIRC_frames[s]>=SIRC_0_MIN_TIME)&&(SIRC_frames[s]<=SIRC_0_MAX_TIME))
              {
                    bits[s]=0;
              }
              else        
              {
                    return 0;
              }
            }
      }    
                                          
      bit_mask=1;
      for(s=1;s<=7;s+=1)
      {                
            if(bits[s]) 
            {                       
                cmd=(cmd|bit_mask);
            }
            bit_mask<<=1;                       
      }
    
      bit_mask=1; 
      for(s=8;s<=12;s+=1) 
      {
            if(bits[s]) 
            {              
                addr=(addr|bit_mask);
            }
            bit_mask<<=1;                
      }
           
      return 1;   
} 
