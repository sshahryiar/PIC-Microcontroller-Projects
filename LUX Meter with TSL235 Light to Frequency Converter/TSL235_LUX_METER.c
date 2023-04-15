#include <16F873A.H>    


#device *=16


#fuses HS, NOWDT, NODEBUG, PROTECT, PUT 
#fuses NOLVP, CPD, NOBROWNOUT, NOWRT 
                                                 
                
#use DELAY (clock = 10MHz)        
#use FAST_IO(C) 
                                         
                                 
#include <lcd420.c>     
                                                     
                         
register unsigned long CNT=0;
register unsigned long long START_TIME=0;  
register unsigned long long END_TIME=0;
register unsigned long long PULSES_CNT=0;
register float F=0.0; 
register float lux=0.0; 


void setup_mcu();
void measure(); 


#int_CCP1
                 
void CCP_ISR()
{
   END_TIME=CCP_1;
   PULSES_CNT=((65536*CNT)-START_TIME+END_TIME);
   START_TIME=END_TIME;         
   CNT=0;
}    
               

#int_TIMER1                      

void TMR_ISR()                                                                                                                  
{
   CNT+=1;                           
}                                 

                                                                                                              
void main()                  
{  
   setup_mcu();
   lcd_gotoxy(1,1);           
   lcd_putc("Intensity/LUX:"); 
   while(TRUE)
   {                                                
        F=((40000.0/(PULSES_CNT)));
        if(f<3.0)
        {
            lux=0;
        }                           
        else
        {                     
            lux=((f/9.0)-(1.0/3.0));  
            lux*=1600.0;     
        }                              
        lcd_gotoxy(6,2);               
        printf(lcd_putc,"%f       ",lux);     
        delay_ms(1000);
   };                  
}
                      
             
void setup_mcu()               
{                                  
   disable_interrupts(global);
   setup_SPI(SPI_disabled); 
   setup_ADC(ADC_off); 
   setup_comparator(NC_NC_NC_NC);                                                                                                                                     
   setup_ADC_ports(no_analogs);
   set_RTCC(0);        
   set_timer1(0);
   set_timer2(0);    
   setup_timer_0(0|0|0);     
   setup_timer_1(T1_INTERNAL|T1_DIV_BY_1);   
   setup_timer_2(T2_disabled,255,1); 
   setup_ccp1(CCP_CAPTURE_FE|CCP_CAPTURE_DIV_16);  
   enable_interrupts(global); 
   enable_interrupts(int_TIMER1);             
   enable_interrupts(int_CCP1); 
   lcd_init();                  
}                               
