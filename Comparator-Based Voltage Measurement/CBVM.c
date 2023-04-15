#include <16F628A.h>


#fuses INTRC_IO, PROTECT, NOWDT, PUT, NOMCLR, BROWNOUT, NOLVP


#use delay(internal = 4MHz)


#define data      pin_B0   
#define clock     pin_B1                           
#define strobe    pin_B2 

#define sw_pin    input(pin_B7)


//+in = vref, -in = sense


void setup();
void CD4094_SIPO(unsigned long value);


void main()
{
    signed char step = 0;
    
    const unsigned long disp1[16] = 
    {
    0x0001, 0x0002, 0x0004, 0x0008,
    0x0010, 0x0020, 0x0040, 0x0080,
    0x0100, 0x0200, 0x0400, 0x0800,
    0x1000, 0x2000, 0x4000, 0x8000
    };
    
    const unsigned long disp2[16] = 
    {
    0x0001, 0x0003, 0x0007, 0x000F,
    0x001F, 0x003F, 0x007F, 0x00FF,
    0x01FF, 0x03FF, 0x07FF, 0x0FFF,
    0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF
    };

    setup();
    
    while(TRUE)
    {
        step = 16;
        do
        {
            step--;
            setup_vref(VREF_LOW | step);
            delay_ms(1);
            
        }while(C1OUT && (step > 0));
             
        if(sw_pin)
        {
            CD4094_SIPO(disp1[step]);
        }
        else
        {
            CD4094_SIPO(disp2[step]);
        }
        
        delay_ms(200);
    }
}


void setup()
{
    disable_interrupts(GLOBAL);
    setup_oscillator(OSC_4MHz);
    set_tris_B(0xF8);
    port_B_pullups(FALSE);
    setup_CCP1(CCP_off);
    setup_timer_0(T0_internal);
    setup_timer_1(T1_disabled);
    setup_timer_2(T2_disabled, 255, 1);
    set_timer0(0);
    set_timer1(0);
    set_timer2(0);
    setup_vref(VREF_HIGH | 15);
    setup_comparator(A0_VR_A1_VR);
}


void CD4094_SIPO(unsigned long value)
{                      
    unsigned char clk = 16;
    
    output_low(strobe);                                 
    while(clk > 0) 
    {                                                   
        if((value & 0x8000) != 0)
        {
            output_high(data);
        }
        else
        {
            output_low(data);
        }
        
        output_high(clock);                     
        value <<= 1;         
        output_low(clock);
        clk--;
    }
    
    output_high(strobe);      
    delay_ms(1);                           
} 
