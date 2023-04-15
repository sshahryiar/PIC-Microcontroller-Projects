#include <16F887.h>


#device *= 16


#fuses NOWDT, PROTECT, HS, NOWRT, CPD, PUT, BORV40  
#fuses BROWNOUT, NOLVP, NODEBUG, NOIESO, FCMEN    
                         

#use delay(clock = 10MHz) 

                 
#define RXPIN     input(pin_C0)

#define LED       pin_C1     
#define RLA1      pin_C4     
#define RLA2      pin_C5     
#define RLA3      pin_C6                       
#define RLA4      pin_C7        


const unsigned char no_of_bits = 12;
const unsigned long bit_pos[no_of_bits] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048};     
                                     

void setup();
unsigned char rx_bit();
signed long rx_Data();


void main()
{     
     unsigned char data_rx = 0;    
     setup();
     while(TRUE)
     {                                        
             data_rx = rx_Data();
             if(data_rx != -1)
             {
                        if(data_rx == 9)
                        {
                            output_toggle(RLA1);  
                        }
                        if(data_rx == 18)
                        {
                            output_toggle(RLA2);  
                        }
                        if(data_rx == 27)
                        {
                            output_toggle(RLA3);  
                        }
                        if(data_rx == 36)
                        {
                            output_toggle(RLA4);  
                        }
                        delay_ms(600);  
                        output_low(LED);   
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


unsigned char rx_bit()
{
    unsigned char t = 0;     
    while(!RXPIN);
    while(RXPIN)
    { 
      delay_us(50); 
      t++;
    }      
    if((t > 35) && (t < 45))
    {
          return '1';
    }
    else if((t > 15) && (t < 25))
    {
          return '0';
    }
    else if((t > 5) && (t < 14))
    {
          return 'S';
    }
    else
    {
        return 'E';
    }    
}         


signed long rx_Data()  
{
    unsigned char D = 0; 
    unsigned char BIT = 0;
    unsigned char N_Bits = 0;
    unsigned long received_data = 0;
    while(rx_bit() != 'S');
    D = rx_bit();
    while(D == 'S')
    {
         D = rx_bit();
    }
    while(TRUE)
    {
      output_high(LED);  
      switch(D)
      {
        case '1':
        {
             received_data += bit_pos[BIT++];
             break;
        }
        case '0':
        {
             BIT++;
             break;
        }
        case 'E':
        case 'S':
        {
             return -1;
        }
      }
      if(BIT == no_of_bits)
      {
          for(BIT = 0; BIT < 8; BIT++)
          {
                  if(((received_data >> BIT) & 0x1))
                  {
                      N_Bits++;
                  }
          }  
          if((N_Bits == ((received_data >> 8) & 0xF)))
          {
              return (received_data & 0xFF);
          }
          else
          {
              return -1;
          }
      }
      D = rx_bit();  
    }               
}
