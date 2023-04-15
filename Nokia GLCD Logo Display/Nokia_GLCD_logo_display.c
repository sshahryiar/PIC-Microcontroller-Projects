#include <18F252.h> 


#device *= 16
#device ADC=10   


#fuses HS, NOWDT, PUT, PROTECT, NOSTVREN
#fuses NOCPB, NOWRT, NOWRTB NOEBTRB, NOEBTR
#fuses BROWNOUT, CPD, NOOSCSEN, BORV20
#fuses NOLVP, NODEBUG, NOWRTC, NOWRTD
                                       
                                         
#use delay(clock=10MHz)            
                                            
                                        
#byte lcdport         = 0x07   
#byte lcdport_tris    = 0x87   
    
                              
#include "3310_custom.c" 
#include "background_1.h"  
#include "background_2.h"
#include "background_3.h"      
#include "background_4.h"  
#include "background_5.h"            
#include "background_6.h"   
#include "background_7.h"    
#include "background_8.h"                  
#include "background_9.h"     
#include "background_10.h"     
#include "background_11.h"  
#include "background_12.h"
#include "background_13.h"      
#include "background_14.h"  
#include "background_15.h"
#include "background_16.h"   
#include "background_17.h"  
#include "background_18.h"                  
#include "background_19.h"     
#include "background_20.h"  
#include "background_21.h"  
#include "background_22.h"
#include "background_23.h"      
#include "background_24.h"  
#include "background_25.h"
#include "background_26.h"   
#include "background_27.h"  
#include "background_28.h"                  
#include "background_29.h"     
#include "background_30.h"  
                              

void setup();      
void draw_background(unsigned char x);
                                  

void main() 
{ 
   unsigned char s = 0;
   setup();              
   while(TRUE)
   {
        for(s = 0; s <= 30; s++)
        {           
                  draw_background(s);  
                  delay_ms(4000);     
        }                    
   }
}                                    


void setup() 
{
   disable_interrupts(global);
   setup_WDT(WDT_off);
   setup_adc_ports(no_analogs);
   setup_adc(ADC_off);
   setup_timer_0(T0_OFF);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,255,1);
   setup_timer_3(T3_DISABLED);
   setup_CCP1(CCP_off);
   setup_CCP2(CCP_off);
   setup_SPI(SPI_DISABLED);
   lcdport_tris = 0x00;      
   nokia_init();
   set_timer0(0);
   set_timer1(0);
   set_timer2(0);
   set_timer3(0);
   delay_ms(100);
}


void draw_background(unsigned char x)
{
   register unsigned int16 i = 0;
   nokia_clean_ddram();
   nokia_clear_screen(); 
   nokia_gotoxy(0, 0);   
   for(i = 0; i < 504; i++)  
   {
            switch(x)
            {
                  case 1:
                  {   
                           nokia_write_data(b1[i]);
                           break;
                  }  
                  case 2:                                               
                  {   
                           nokia_write_data(b2[i]);
                           break;
                  } 
                  case 3:
                  {   
                           nokia_write_data(b3[i]);
                           break;
                  } 
                  case 4:
                  {   
                           nokia_write_data(b4[i]);
                           break;
                  }  
                  case 5:                                               
                  {   
                           nokia_write_data(b5[i]);
                           break;
                  } 
                  case 6:
                  {   
                           nokia_write_data(b6[i]);
                           break;
                  } 
                  case 7:                                               
                  {   
                           nokia_write_data(b7[i]);
                           break;
                  } 
                  case 8:
                  {   
                           nokia_write_data(b8[i]);
                           break;
                  } 
                  case 9:                                               
                  {                           
                           nokia_write_data(b9[i]);
                           break;
                  } 
                  case 10:
                  {   
                           nokia_write_data(b10[i]);
                           break;
                  }  
                  case 11:
                  {   
                           nokia_write_data(b11[i]);
                           break;
                  }  
                  case 12:                                               
                  {   
                           nokia_write_data(b12[i]);
                           break;
                  } 
                  case 13:
                  {   
                           nokia_write_data(b13[i]);
                           break;
                  } 
                  case 14:
                  {   
                           nokia_write_data(b14[i]);
                           break;
                  }  
                  case 15:                                               
                  {   
                           nokia_write_data(b15[i]);
                           break;
                  } 
                  case 16:
                  {   
                           nokia_write_data(b16[i]);
                           break;
                  } 
                  case 17:                                               
                  {
                           nokia_write_data(b17[i]);
                           break;
                  } 
                  case 18:
                  {   
                           nokia_write_data(b18[i]);
                           break;
                  } 
                  case 19:                                               
                  {   
                           nokia_write_data(b19[i]);
                           break;
                  }                                  
                  case 20:
                  {   
                           nokia_write_data(b20[i]);
                           break;
                  } 
                  case 21:
                  {   
                           nokia_write_data(b21[i]);
                           break;
                  }  
                  case 22:                                               
                  {   
                           nokia_write_data(b22[i]);
                           break;
                  }        
                  case 23:
                  {   
                           nokia_write_data(b23[i]);
                           break;
                  } 
                  case 24:
                  {   
                           nokia_write_data(b24[i]);
                           break;
                  }  
                  case 25:                                               
                  {   
                           nokia_write_data(b25[i]);
                           break;
                  } 
                  case 26:
                  {   
                           nokia_write_data(b26[i]);
                           break;
                  } 
                  case 27:                                               
                  {
                           nokia_write_data(b27[i]);
                           break;
                  } 
                  case 28:
                  {   
                           nokia_write_data(b28[i]);
                           break;
                  } 
                  case 29:                                               
                  {   
                           nokia_write_data(b29[i]);
                           break;
                  } 
                  case 30:
                  {   
                           nokia_write_data(b30[i]);
                           break;
                  }  
                  default:  
                  {                                                    
                           break;        
                  }            
            } 
   }
}

                                             
