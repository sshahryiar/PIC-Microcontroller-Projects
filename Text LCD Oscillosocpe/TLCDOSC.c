#include <16F88.h>


#device *=16
#device ADC=8


#fuses NOWDT, PROTECT, CPD, BROWNOUT, PUT, INTRC_IO, NOIESO, FCMEN, NOMCLR


#use delay (internal=4MHz)


#include <lcd420.c>
#include <math.h>


#define T_inc       input(pin_A1)                       
#define T_dec       input(pin_A2)
#define V_inc       input(pin_A6)
#define V_dec       input(pin_A7)   


unsigned char signal[16];
const unsigned char step[5]={1,2,4,8,16};


void setup();                               
void load_graphics();

void main()
{
    unsigned char x=0;
    unsigned char vm=1;
    unsigned long adc=0;
    unsigned int32 tmp=0;
    unsigned long t=10;
    float mx=0, mn=0, v=0;
    setup();
    while(TRUE)
    {
        if(V_inc)
        {                
            delay_ms(25);
            vm+=0.5;
        }
        if(V_dec)
        {
            delay_ms(25);
            vm-=0.5;
        }                                           
        
        if(vm>4)
        {
            vm=4;
        }
        if(vm<1)
        {
            vm=1;
        }      

        if(T_inc)
        {
            delay_ms(25);
            t*=10;
        }
        if(T_dec)
        {
            delay_ms(25);
            t/=10;
        }
        
        if(t>1000)
        {
            t=1000;
        }
        if(t<1)
        {
            t=1;
        }
        
        mn=5.0;
        mx=0.0;
        for(x=0;x<16;x++)
        {
            read_adc(adc_start_only);
            while(!adc_done());
            tmp=read_adc(adc_read_only);
            v=(tmp-127.0);         
            v=(v*v);
            v=sqrt(v);  
            v*=10.0;
            v/=255.0;             
            if(v>mx)
            {
                mx=v;
            }
            if(v<mn)  
            {
                mn=v;                        
            }
            adc=tmp;
            adc*=vm;
            adc/=32.0;   
            signal[x]=step[adc]; 
            delay_us(t);               
        }                     
        load_graphics();
        lcd_gotoxy(1,1);
        lcd_putc(0);
        lcd_gotoxy(1,2);
        lcd_putc(1);                          
        lcd_gotoxy(3,1);
        printf(lcd_putc,"MX:%1.2fV   T ", mx);
        lcd_gotoxy(3,2);
        printf(lcd_putc,"MN:%1.2fV %4lu ", mn,t);
    };
}                                            


void setup()
{
    disable_interrupts(GLOBAL); 
    setup_WDT(WDT_OFF);
    setup_oscillator(OSC_4MHz);
    setup_SPI(SPI_SS_disabled);       
    setup_timer_0(0|0|0);                       
    setup_timer_1(T1_disabled);
    setup_timer_2(T2_disabled,255,1);
    set_RTCC(0);
    set_timer1(0);
    set_timer2(0);
    setup_CCP1(CCP_OFF);
    setup_comparator(NC_NC_NC_NC);
    setup_ADC(ADC_clock_internal);
    setup_ADC_ports(sAN0);
    set_ADC_channel(0);
    lcd_init();
    lcd_putc("\f");
}

                                       
void load_graphics()
{
   unsigned int8 s=0;
   lcd_send_byte(0,0x40); 
   for(s=0;s<16;s++)
   {
        lcd_send_byte(1,signal[s]);
   }
   lcd_send_byte(0,0x80);
} 


