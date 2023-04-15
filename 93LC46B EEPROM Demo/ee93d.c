#include <16F877A.h>


#device *= 16


#fuses NOWDT, PROTECT, CPD, NOLVP, NODEBUG, XT, BROWNOUT, NOWRT, PUT


#use delay (clock = 4MHz)


#define t_min_ROM_location          0
#define t_max_ROM_location          1

#define reset_readings              !input(pin_B0)

#define LED                         pin_B4

#include "lcd.c"
#include "DS18B20.c"
#include "93LC46B.c"


const unsigned char symbols[8]= 
{
    0x00, 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00
};           


void setup();
void lcd_symbol_load();


void main()
{
    float t_c = 0.0;
    float t_max = -880;
    float t_min = 2000;
    signed long temp = 0;
    
    setup();
    
    temp = EE_93LC46B_read(t_min_ROM_location);
    
    if((temp < -880) && (temp > 2000))
    {
        EE_93LC46B_erase(t_min_ROM_location);
        t_min = 2000;
    }
    else
    {
        t_min = temp;
    }
    
    temp = EE_93LC46B_read(t_max_ROM_location);
    
    if((temp < -880) && (temp > 2000))
    {
        EE_93LC46B_erase(t_max_ROM_location);
        t_max = -880;
    }
    else
    {
        t_max = temp;
    }
    
    lcd_gotoxy(1, 1);
    lcd_putc("T/ C  Tmax  Tmin");
    lcd_gotoxy(3, 1);
    lcd_putc(0);
    
    while(TRUE)
    {
        t_c = get_temperature();
        
        if(reset_readings)
        {
            output_high(LED);
            
            EE_93LC46B_erase(t_min_ROM_location);
            t_min = 2000;
            
            EE_93LC46B_erase(t_max_ROM_location);
            t_max = -880;
        }
        
        if(t_c > t_max)
        {
             output_high(LED);
             t_max = t_c;
             EE_93LC46B_write(t_max_ROM_location, t_max);
        }
        
        if(t_c < t_min)
        {
             output_high(LED);
             t_min = t_c;
             EE_93LC46B_write(t_min_ROM_location, t_min);
        }
        
        lcd_gotoxy(1, 2);
        printf(lcd_putc, "%2.1g  %2.1g  %2.1g", (t_c * t_scalar), (t_max * t_scalar), (t_min * t_scalar));
        output_low(LED);
    };
}


void setup()
{
    disable_interrupts(GLOBAL);
    setup_comparator(NC_NC_NC_NC);
    setup_ADC(ADC_off);
    setup_ADC_ports(no_analogs);
    setup_SPI(SPI_disabled | SPI_SS_disabled);
    setup_timer_0(T0_internal | T0_8_bit);
    set_timer0(0);
    setup_timer_1(T1_disabled);
    set_timer1(0);
    setup_timer_2(T2_disabled, 255, 1);
    set_timer2(0);
    setup_CCP1(CCP_off);
    setup_CCP2(CCP_off);
    port_B_pullups(TRUE);
    DS18B20_init();
    EE_93LC46B_init();
    lcd_init();
    lcd_symbol_load();
    lcd_putc("\f");
}


void lcd_symbol_load()  
{
   unsigned char i = 0x00;
   
   lcd_send_byte(0, 0x40); 
   
   for(i = 0x00; i < 0x08; i++)
   {
        lcd_send_byte(1, symbols[i]);
   }
   lcd_send_byte(0, 0x80);
}   
