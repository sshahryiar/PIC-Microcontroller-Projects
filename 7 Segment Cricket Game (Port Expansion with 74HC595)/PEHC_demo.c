#include <10F200.h> 


#fuses NOWDT, NOMCLR, PROTECT                        

                                        
#use delay(clock = 4MHz) 


#define     button    input(pin_B3)

#define     SER       pin_B2   
#define     SRCK      pin_B1                           
#define     RCK       pin_B0  


// 1, 2, 3, 4, 6, n(no ball), c(caught out), L(LBW)
// S(stumped), r(runout), E(extra), .(no run), blank

const unsigned char score[13] =
{0x06,0x5B,0x4F,0x66,0x7D,0x54,0x61,0x38,0xED,0x50,0x79,0x80,0x00};

const unsigned char run[7] = 
{0x01,0x02,0x04,0x08,0x10,0x20,0x40};

                                           
void setup_mcu();              
void SIPO(unsigned int8 value);                        
      
                          
void main()    
{                                             
    unsigned int8 s=12;          
    unsigned int8 x=0;    
    setup_mcu(); 
    while(TRUE)
    {                
        if(button)
        {                               
           SIPO(run[6]);
           delay_ms(500);                                             
           while(button)
            {
                    x+=1;
                    if(x>5)
                    {
                        x=0;
                    }
                    SIPO(run[x]);
                    if(s>12)
                    {
                        s=0;
                    }
                    else
                    {
                        s++;
                    }
                    delay_ms(50);
            }
            SIPO(run[6]);
            delay_ms(500);
        }
        SIPO(score[s]);
    }                         
}                           
                             
                          
void setup_mcu()                     
{                        
    output_B(0);                 
    delay_ms(250);
}     


void SIPO(unsigned int8 value)
{                      
    unsigned int8 clk=0;                    
    output_low(RCK);                                 
    for(clk=0;clk<8;clk++) 
    {                                                   
        output_bit(SER,(0x80&value));                     
        output_high(SRCK);                                     
        delay_us(250);                        
        value<<=1;         
        output_low(SRCK);                         
        delay_us(250);                                 
    }                                     
    output_high(RCK);                               
}         
