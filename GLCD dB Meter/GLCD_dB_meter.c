#include <18F242.h>
                
                
#device *= 16 
#device ADC=10


#fuses HS, PUT, NOWDT, PROTECT, CPD, BROWNOUT, BORV45
#fuses NOSTVREN, NOLVP, NODEBUG, NOCPB, NOWRT, NOWRTC
#fuses NOWRTB,  NOWRTD, NOEBTR, NOEBTRB, NOOSCSEN       


#use delay (clock = 10MHz)      
                      

#define LARGE_LCD        
                     
#define ref_SPL			 94 
#define sensitivity    3.16
                    
                                                        
#include <math.h>
#include "HDM64GS192.c"             
#include "graphics.c"       
#include "background_art.c"           
                                             
                                                                     
unsigned char txt_dB_msg[30] = {"SPL Meter"};
unsigned char txt_dB_current[30];
unsigned char txt_dB_max[30];   
unsigned char txt_dB_min[30];    
unsigned char txt_dB_avg[30];    
                                
unsigned char x = 17;             

float dB_max = 0.0;
float dB_min = 100.0;
float dB_avg = 0.0;             
float dB_current = 0.0;        
                              
                                                                                                 
void setup();
void draw_background();                                                        
float adc_rms();    
void read_SPL();
void display_redings();            
void plot_data(); 
float map(float value, float x_min, float x_max, float y_min, float y_max);
                             

void main()                                                                                
{                        
    setup(); 
    draw_background();
    
    while(TRUE)            
    {
        read_SPL();
        display_redings();
        plot_data();     
        delay_ms(400);
    }                             
}                                                                                
                                                         
                                                  
void setup()                                                                   
{                                           
       disable_interrupts(global);  
       setup_WDT(WDT_off);
       setup_spi(spi_ss_disabled|spi_disabled);
       setup_timer_0(T0_internal);                              
       setup_timer_1(T1_disabled);                                                                         
       setup_timer_2(T2_disabled, 255, 1);     
       set_timer0(0);
       set_timer1(0);     
       set_timer2(0);
       setup_ccp1(ccp_off);       
       setup_ccp2(ccp_off);
       setup_ADC_ports(AN0);                        
       setup_ADC(ADC_clock_div_32);  
       set_ADC_channel(0);
       glcd_init(on);   
       glcd_fillscreen(0);            
       memset(txt_dB_current, 0, 30);     
       memset(txt_dB_max, 0, 30); 
       memset(txt_dB_min, 0, 30);    
       memset(txt_dB_avg, 0, 30);    
                                                     
       glcd_text57(130, 4, txt_dB_msg, 1, ON); 
}                               


void draw_background() 
{                            
   unsigned long n = 0;                      
   unsigned char i = 0;
   unsigned char j = 0;
   unsigned char cs = 0;        
   
   for(i = 0; i < 8; ++i)       
   {       
          output_low(GLCD_DI);                      
          glcd_writeByte(GLCD_LEFT, 0x40);   
          glcd_writeByte(GLCD_RIGHT, 0x40);       
          glcd_writeByte(GLCD_MID, 0x40);                          
          glcd_writeByte(GLCD_LEFT,(i | 0xB8));
          glcd_writeByte(GLCD_RIGHT,(i | 0xB8)); 
          glcd_writeByte(GLCD_MID,(i | 0xB8));     
          output_high(GLCD_DI);                         
          for(j = 0; j < 128; ++j)
          {
                 if((j >= 0) && (j < 64))                  
                 {                                     
                    cs=GLCD_LEFT;                   
                 }               
                 else                                
                 {                          
                    cs=GLCD_MID;
                 }                                                 
                 glcd_writeByte(cs,background[n]);      
                 n++;     
          } 
   }                         
}   
    

float adc_rms()
{                         
    unsigned char samples = 16;
    register unsigned long long tmp = 0;
    register float rms = 0.0;          
                      
    while(samples > 0)
    {
        read_adc(adc_start_only);
        while(!adc_done());
        rms = read_adc(adc_read_only);  
        tmp += (rms * rms);      
        samples--;      
    }                     
    tmp >>= 4;                           
    rms = (sqrt(tmp));     
    rms *= 0.004875; 
    
    if(rms <= 0)
    {
    	rms = 0.004875;
    }
    
    return rms;                    
}                                         


void read_SPL() 
{
   dB_current = adc_rms();
   db_current = (ref_SPL + 20 * log10(db_current / sensitivity)); 
   
   if(db_current <= 40)
   {
       dB_current = 40;
   }
   if(dB_current >= 99)
   {
       dB_current = 99;
   }
                                                 
   if(x > 125) 
   {
		db_max = 0.0;
		db_min = 100.0;    
		x = 17;            
   }
   
   if(dB_current > dB_max)      
   {     
		db_max = dB_current;  
   }              
   
   if(dB_current < dB_min)      
   {                    
		db_min = dB_current;
   }                   
   
   dB_avg = ((db_max + dB_min) * 0.5);   
}                                                                 
                     

void display_redings() 
{                                                          
    glcd_text57(130, 20, txt_dB_current, 1, OFF);  
    sprintf(txt_dB_current, "Cr:%2.1g dB", db_current);    
    glcd_text57(130, 20 , txt_dB_current, 1, ON);     
                                          
    glcd_text57(130, 30, txt_dB_max, 1, OFF);         
    sprintf(txt_dB_max, "Mx:%2.1g dB", dB_max);
    glcd_text57(130, 30 , txt_dB_max, 1, ON);     
    
    glcd_text57(130, 40, txt_dB_min, 1, OFF);         
    sprintf(txt_dB_min, "Mn:%2.1g dB", dB_min);
    glcd_text57(130, 40 , txt_dB_min, 1, ON); 
                      
    glcd_text57(130, 50, txt_dB_avg, 1, OFF);         
    sprintf(txt_dB_avg, "Av:%2.1g dB", dB_avg);
    glcd_text57(130, 50, txt_dB_avg, 1, ON); 
}                                
                            
                                                    
void plot_data()                                
{                                    
   unsigned char l = 0;
                           
   l = map(dB_current, 40, 99, 61, 2); 
   
   glcd_line(x, 2, x, 61, YES);   
   glcd_line(x, l, x, 61, NO); 
   
   x += 2;
}     

                                  
float map(float value, float x_min, float x_max, float y_min, float y_max)   
{                         
    return (y_min + (((y_max - y_min) / (x_max - x_min)) * (value - x_min)));
}           
