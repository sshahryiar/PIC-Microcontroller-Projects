#include <16F886.h>


#device *= 16


#fuses NOWDT, PROTECT, HS, NOWRT, CPD, PUT, BORV40  
#fuses BROWNOUT, NOLVP, NODEBUG, NOIESO, FCMEN    
                         

#use delay(clock = 10MHz) 


#define txPin     pin_C0
#define LED       pin_C1

#define b1        input(pin_A0)   
#define b2        input(pin_A1)  
#define b3        input(pin_A2)  
#define b4        input(pin_A3)   


void setup();
void tx_data(unsigned char d);


void main()
{     
     setup();
     while(TRUE)
     {                                        
             if(b1)
             {
                        tx_data(9);
             }
             if(b2)
             {
                        tx_data(18);
             }
             if(b3)
             {
                        tx_data(27);
             }
             if(b4)
             {
                        tx_data(36);
             }     
     } 
}  


void setup()
{    
    disable_interrupts(global);
    setup_comparator(NC_NC_NC_NC);
    setup_ADC(ADC_off);
    setup_ADC_ports(no_analogs);
    set_TRIS_A(0x00);
    port_B_pullups(false);
    setup_WDT(WDT_OFF);
    setup_timer_1(T1_disabled);
    setup_timer_2(T2_disabled, 255, 1);
    setup_CCP1(CCP_off);
    setup_CCP2(CCP_off);
    setup_SPI(SPI_DISABLED | SPI_SS_DISABLED);
    set_timer0(0x00);
    set_timer1(0x0000);
    set_timer2(0x00);
    output_C(0);
    delay_ms(500);     
}    


void tx_data(unsigned char d)  
{
      unsigned char BITS = 0;
      unsigned char N_Bits = 0;
      delay_ms(40); 
      output_high(LED);        
      for(BITS = 0; BITS < 8; BITS++)
      {
          if(((d >> BITS) & 0x1))
          {
              N_Bits++;
          }
      }
      output_low(LED);  
      for(BITS = 0; BITS < 16; BITS++)
      {
          output_toggle(LED);  
          output_high(txPin);   
          delay_us(500);   
          output_low(txPin);   
          delay_us(500);   
      }
      for(BITS = 0; BITS < 8; BITS++)
      {
          output_toggle(LED);  
          output_high(txPin); 
          if(((d >> BITS) & 0x1))
          {
              delay_ms(2);
          }
          else
          {
              delay_ms(1);
          }
          output_low(txPin);   
          delay_ms(1);
      }
      for(BITS = 0; BITS < 4; BITS++)
      {
        output_toggle(LED);  
        output_high(txPin);  
        if(((N_Bits >> BITS) & 0x1))
        {
            delay_ms(2);
        }
        else
        {
            delay_ms(1);
        }        
        output_low(txPin); 
        delay_ms(1);
      }
      delay_ms(3);
      output_low(LED); 
}
