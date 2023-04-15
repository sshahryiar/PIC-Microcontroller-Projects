#include <16F877A.h>


#device *=16
#device ADC=10
 

#fuses NOWDT, PROTECT, CPD, NOLVP, HS, NOBROWNOUT, NODEBUG, NOWRT, PUT  
                                                                                         
                                                                                                     
#use delay (clock = 20MHz) 


#include <math.h>
#include "lcd.c"  


#define center_value                521.05
#define volt_adj                        20

               
void setup();  
void get_parameters(float *vrms, unsigned int16 *freq);
     
                                                                                                                    
                                                                                                                        
void main()                                                                    
{                                                                                                              
   float vr = 0.0;
   unsigned int16 fi = 0;
   setup();
                       
   while(TRUE)                               
   {                                
      get_parameters(&vr, &fi);
      
      lcd_gotoxy(12, 1);
      printf(lcd_putc, "%2.1f" , vr);
      lcd_gotoxy(15, 2);
      printf(lcd_putc, "%2lu" , fi);
     
      delay_ms(100);
   };                                             
}                                                 
                                   
                                         
void setup()
{                                                                                               
   disable_interrupts(global); 
   set_tris_d(0x00);
   set_tris_e(0x00);
   setup_timer_0(T0_internal); 
   setup_timer_1(T1_disabled);
   setup_timer_2(T2_disabled, 255, 1);
   set_timer0(0);
   set_timer1(0);           
   set_timer2(0);                                                           
   setup_CCP1(CCP_off);
   setup_CCP2(CCP_off);
   setup_psp(PSP_disabled);                                
   setup_comparator(NC_NC_NC_NC);       
   setup_SPI(SPI_disabled);                   
   port_B_pullups(FALSE);  
   setup_ADC(ADC_CLOCK_DIV_64);                    
   set_analog_pins(AN0);
   set_ADC_channel(0);
   enable_interrupts(global);  
   enable_interrupts(int_rda);  
   lcd_init();
   lcd_putc("\f");
   
   lcd_gotoxy(1, 1);
   lcd_putc("Volt./V:");
   lcd_gotoxy(1, 2);
   lcd_putc("Freq/Hz:");
}       


void get_parameters(float *vrms, unsigned int16 *freq)
{
  
  unsigned int32 tmp1 = 0;

  float vtemp1 = 0.0;
  float vtemp2 = 0.0;

  unsigned int16 s = 512;

  unsigned int16 temp_f = 0;
  unsigned int16 temp_f1 = 0;
  unsigned int16 temp_f2 = 0;

  while(s > 0)
  {
    read_adc(ADC_START_ONLY);
    while(!adc_done());
        
    vtemp1 = read_adc(ADC_READ_ONLY);

    if((vtemp1 > center_value) && (vtemp2 < center_value))
    {
        temp_f1++;
    }

    if((vtemp2 > center_value) && (vtemp1 < center_value))
    {
        temp_f2++;
    }

    vtemp2 = vtemp1;

    vtemp1 -= center_value;
    tmp1 += ((vtemp1) * (vtemp1));
    
    s--;
  }


  temp_f = ((float)(((float)(temp_f1 + temp_f2)) / 2.0) * 4.15);
  tmp1 >>= 9;

  *vrms = (sqrt(tmp1) + volt_adj);

  *freq = (temp_f);
 }
