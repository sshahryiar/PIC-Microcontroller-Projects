#include <16F88.h>


#device *= 16;
#device ADC = 10


#fuses NOMCLR, INTRC_IO, PROTECT, CPD, NOWDT
#fuses BROWNOUT, NOWRT, PUT, NOIESO, FCMEN


#use delay (internal = 4MHz)


#include <lcd420.c>


#define step         10.0
#define min         -50.0       
#define res          4700
#define ADC_max      1023
#define entries		   21


const unsigned long R[entries] =                 
{
1030, 1135, 1247, 1367, 1495, 1630, 1772, 1922, 2080, 2245, 2417, 2597, 2785, 2980, 3182, 3392, 3607, 3817, 4008, 4166, 4280
//-50   -40   -30   -20   -10     0    10    20    30    40    50    60    70    80    90   100   110   120   130   140   150    
// 0     1     2     3     4      5     6     7     8     9    10    11    12    13    14    15    16    17    18    19    20      
};

const unsigned char symbols[16]= 
{        
    0x07, 0x05, 0x05, 0x07, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x0E, 0x11, 0x11, 0x0A, 0x11, 0x00
};


void setup();
void lcd_symbol();
unsigned long adc_avg();        
unsigned char binary_search(unsigned long value);

                                               
void main()
{
    float t = 0;
    float RT = 0;
    unsigned char tmp = 0;
    setup();
    
    while(TRUE)
    {
        RT = adc_avg();
        RT = ((ADC_max  / RT) - 1);
        RT *= res;
        
        tmp = binary_search(RT);
                              
        t = ((step * tmp) + min);
        t += (step * ((RT - R[tmp]) / (R[tmp + 1] - R[tmp])));
                                                  
        lcd_gotoxy(1, 1);
        lcd_putc("T/ C:");
        lcd_gotoxy(3, 1);
        lcd_putc(0);
        lcd_gotoxy(7, 1);
        printf(lcd_putc, "%3.2g ", t);
        lcd_gotoxy(1, 2);
        lcd_putc("R/k :");
        lcd_gotoxy(4, 2);               
        lcd_putc(1);             
        lcd_gotoxy(7, 2);
        printf(lcd_putc, "%4.2g ", RT);
        delay_ms(100);
    };
}


void setup()
{                                                 
    disable_interrupts(global);
    setup_oscillator(OSC_4MHz);
    setup_WDT(WDT_off);
    setup_timer_0(RTCC_INTERNAL | RTCC_8_BIT);
    setup_timer_1(T1_disabled);
    setup_timer_2(T2_disabled, 255, 1);
    set_timer0(0);
    set_timer1(0);
    set_timer2(0);
    setup_CCP1(CCP_off);
    setup_SPI(SPI_disabled | SPI_SS_disabled);
    setup_comparator(NC_NC_NC_NC);
    setup_ADC(ADC_clock_internal);
    setup_ADC_ports(sAN0, VSS_VDD);
    set_ADC_channel(0);
    lcd_init();
    lcd_symbol();
 }
 
 
void lcd_symbol()  
{
   unsigned char n = 0;  
 
   lcd_send_byte(0, 0x40); 
   for(n = 0; n < 16; n += 1)                                       
   {
        lcd_send_byte(1, symbols[n]);
   }
   lcd_send_byte(0, 0x80);                      
} 
 
 
unsigned long adc_avg()
{                                          
    unsigned char s = 16;
    register unsigned long avg = 0;
    
    while(s > 0)                       
    {
        read_ADC(ADC_start_only);
        while(!ADC_done());
        avg += read_ADC(ADC_read_only);
        s -= 1;
    }
    avg >>= 4;         
    
    return avg;
 }
 
 
unsigned char binary_search(unsigned long value)
{
    signed char h = 20;
    signed char l = 0;
    signed char m = 10;
    
    while ((l <= h) && (l < entries) && (h > 0))
    {                                    
        if(value < (R[m]))
        {
            h = (m - 1);
        }
        else
        {
            l = (m + 1);
        }
        m = (h + l);
        m >>= 1;
    }
    
    return ((unsigned char)m);
}
