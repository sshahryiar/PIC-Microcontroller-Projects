#include <16F690.h>


#device *= 16


#fuses NOWDT, PROTECT, CPD, INTRC_IO, PUT
#fuses NOMCLR, FCMEN, NOIESO, NOBROWNOUT


#use delay (internal = 4MHz)
#use FAST_IO(C)
                                

#include "8255.c"


void setup();


void main()                            
{
    unsigned char data_value = 0x00;
    
    setup();
    
    while(TRUE)                                   
    {
        data_value = read_8255(PORT_A);
        write_8255(PORT_C, data_value);
    };
}


void setup()
{
    setup_oscillator(OSC_4MHz);
    setup_WDT(WDT_off);
    disable_interrupts(global);                                                   
    setup_SPI(SPI_disabled);
    setup_CCP1(CCP_off);
    setup_timer_0(0|0|0);
    setup_timer_1(T1_disabled);
    setup_timer_2(T2_disabled, 255, 1);
    set_RTCC(0);
    set_timer1(0);
    set_timer2(0);
    setup_comparator(NC_NC_NC_NC);
    setup_ADC(ADC_off);
    setup_ADC_ports(no_analogs);
    _8255_init();
    delay_ms(100);
}
                                                                        


