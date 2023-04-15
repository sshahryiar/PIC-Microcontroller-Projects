#include <16F887.h>


#device *= 16
#device ADC = 10


#fuses MCLR, NOWDT, PROTECT, CPD, NOWRT, INTRC_IO
#fuses BROWNOUT, IESO, FCMEN, NOLVP, BORV40, PUT 


#use delay (internal = 2MHz)


#include "lcd.c"


#define BT1             !input(pin_B0)
#define BT2             !input(pin_B1)
#define BT3             !input(pin_B2)
#define BT4             !input(pin_B3)

#define key_dly         200

#define no_of_samples   64

#define ADC_Max         1023
#define Fixed_R         470
#define error           5

#define Max_Entry       7
#define Min_Entry       0

#define R_Max           250
#define R_Min           0

#define P_Max           100
#define P_Min           0


const unsigned char symbols[0x08]= 
{
    0x00, 0x00, 0x0E, 0x11, 0x11, 0x0A, 0x11, 0x00
};     


unsigned char percentage_value[8] = {12, 24, 48, 60, 72, 84, 96, 100};
unsigned char resistance_value[8] = {10, 20, 54, 75, 102, 129, 140, 175};


void setup();
void lcd_symbols();
unsigned long ADC_avg();
void read_memory();
void write_memory();
void set_values();
signed long inc_dec(unsigned char x_pos, unsigned char y_pos, signed long value, signed long max_value, signed long min_value);
unsigned char calculate_R();
void swap_values(unsigned char *a, unsigned char *b);
void sort_arrays();
unsigned char get_percentage(unsigned char value);
unsigned char constrain(signed long value, signed long value_min, signed long value_max);
unsigned char map(signed long value, signed long x_min, signed long x_max, signed long y_min, signed long y_max);



void main()
{
   unsigned char v = 0;
   
   setup();   
   
   while(TRUE)
   {
       set_values();
       
       lcd_gotoxy(1, 1);
       lcd_putc("Resistance/ :");
       lcd_gotoxy(12, 1);
       lcd_putc(0);
       lcd_gotoxy(1, 2);
       lcd_putc("Percentage/%:");
       
       v = calculate_R();
       lcd_gotoxy(14, 1);
       printf(lcd_putc, "%3u", v);
       
       v = get_percentage(v);
       lcd_gotoxy(14, 2);
       printf(lcd_putc, "%3u", v);
       
       delay_ms(400);
   };
}


void setup()
{
    disable_interrupts(global);
    setup_oscillator(OSC_2MHZ);
    setup_WDT(WDT_off);
    setup_comparator(NC_NC_NC_NC);
    setup_SPI(SPI_disabled | SPI_SS_disabled);
    setup_CCP1(CCP_off);
    setup_CCP2(CCP_off);
    setup_timer_0(T0_internal | T0_8_bit);
    setup_timer_1(T1_disabled);
    setup_timer_2(T2_disabled, 255, 1);
    set_timer0(0);
    set_timer1(0);
    set_timer2(0);
    set_tris_B(0xFF);
    port_B_pullups(0x0F);
    setup_ADC(ADC_clock_div_32);
    setup_ADC_ports(sAN0);
    set_ADC_channel(0);
    lcd_init();
    lcd_putc("\f");
    lcd_symbols();
    delay_ms(100);
}


void lcd_symbols()  
{
   unsigned char i = 0;    
   
   lcd_send_byte(0, 0x40); 
   for(i = 0; i < 8; i++)
   {
        lcd_send_byte(1,symbols[i]);
   }
   lcd_send_byte(0, 0x80);
}  


unsigned long ADC_avg()
{
    unsigned char samples = no_of_samples;
    unsigned long avg = 0x0000;
    
    while(samples > 0)
    {
        read_ADC(ADC_start_only);
        while(ADC_done() == FALSE);
        avg += read_ADC(ADC_read_only);
        samples--;
    };
    
    avg >>= 0x06;
    
    return avg;
}


void read_memory()
{
    short data_error = FALSE;
    unsigned char i = 0;
    
    for(i = Min_Entry; i <= Max_Entry; i++)
    {
        resistance_value[i] = read_eeprom(i);
        delay_ms(40);
        
        if(resistance_value[i] > R_Max)
        {
            data_error = TRUE;
        }
    }
    
    for(i = Min_Entry; i <= Max_Entry; i++)
    {
        percentage_value[i] = read_eeprom(i + 8);
        delay_ms(40);
        
        if(percentage_value[i] > P_Max)
        {
            data_error = TRUE;
        }
    }
    
    if(data_error == TRUE)
    {
        memset(percentage_value, 0, sizeof(percentage_value));
        memset(resistance_value, 0, sizeof(resistance_value));
        write_memory();
    }
}


void write_memory()
{
    unsigned char i = 0;
    
    for(i = Min_Entry; i <= Max_Entry; i++)
    {
        if((resistance_value[i] >= R_Min) && (resistance_value[i] <= R_Max))
        {
            write_eeprom(i, resistance_value[i]);
            delay_ms(40);
        }
    }
    
    for(i = Min_Entry; i <= Max_Entry; i++)
    {
        if((percentage_value[i] >= P_Min) && (percentage_value[i] <= P_Max))
        {
            write_eeprom((i + 8), percentage_value[i]);
            delay_ms(40);
        }
    }
}


void set_values()
{    
    unsigned char i = 0;
    
    if(BT1)
    {
        while(BT1);
        delay_ms(600);
        lcd_putc("\f"); 
        memset(percentage_value, 0, sizeof(percentage_value));
        memset(resistance_value, 0, sizeof(resistance_value));
        
        while(TRUE)
        {
              read_memory();
              
              for(i = Min_Entry; i <= Max_Entry; i++)
              {
                  lcd_gotoxy(1, 1);
                  lcd_putc("R[ ]/ :");
                  lcd_gotoxy(6, 1);
                  lcd_putc(0);
                  lcd_gotoxy(1, 2);
                  lcd_putc("P[ ]/%:");
                  
                  lcd_gotoxy(3, 1);
                  printf(lcd_putc, "%1u", i);
                  lcd_gotoxy(3, 2);
                  printf(lcd_putc, "%1u", i);
                  resistance_value[i] = inc_dec(10, 1, resistance_value[i], R_Max, R_Min);
                  percentage_value[i] = inc_dec(10, 2, percentage_value[i], P_Max, P_Min);
                  
                  lcd_putc("\f");
              }
              
              sort_arrays();
              write_memory();
              break;
        };
    }
}


signed long inc_dec(unsigned char x_pos, unsigned char y_pos, signed long value, signed long max_value, signed long min_value)
{
     while(TRUE)
     {
         lcd_gotoxy(x_pos, y_pos);
         printf(lcd_putc, "%3lu  ", value);
         
         if(BT2)
         {
             delay_ms(key_dly);
             if(BT2)
             {
                 value += 10;
             }
             else
             {
                 value += 1;
             }
         }
         
         if(value > max_value)
         {
             value = min_value;
         }
         
         if(BT3)
         {
             delay_ms(key_dly);
             if(BT3)
             {
                 value -= 10;
             }
             else
             {
                 value -= 1;
             }
         }
         
         if(value < min_value)
         {
             value = max_value;
         }
         
         if(BT4)
         {
             delay_ms(key_dly);
             return value;
         }
     }
}


unsigned char calculate_R()
{
    float RV = 0;
    
    RV = ADC_avg();
    RV = ((ADC_max / RV) - 1);
    RV *= Fixed_R;
    
    return RV;
}


void swap_values(unsigned char *a, unsigned char *b)
{
    unsigned char c = 0;
    
    c = *a;
    *a = *b;
    *b = c;
}


void sort_arrays()
{
    unsigned char i = 0;
    unsigned char j = 0;
    
    for(j = Min_Entry; j <= Max_Entry; j++)
    {
          for(i = Min_Entry; i <= Max_Entry; i++)
          {
              if(resistance_value[i] >= resistance_value[i + 1])
              {
                  swap_values(&resistance_value[i], &resistance_value[i + 1]);
                  swap_values(&percentage_value[i], &percentage_value[i + 1]);
              }
          }
    }
}


unsigned char get_percentage(unsigned char value)
{
    signed char i = 0;
    unsigned char pc = 0;
    
    for(i = 0; i <= Max_Entry; i++)
    {
        if(value <= resistance_value[i])
        {           
            break;
        }
    }
    
    if(i >= Max_Entry)
    {
       i = Max_Entry;
    }
    if(i <= 0)
    {
       i = 0; 
    }
    
    if((i >= Min_Entry) && (i < Max_Entry))
    {
        pc = map(value, resistance_value[i], resistance_value[i + 1], percentage_value[i], percentage_value[i + 1]);
    }
    
    if(i >= Max_Entry)
    {
       pc = map(value, resistance_value[i - 1], resistance_value[i], percentage_value[i - 1], percentage_value[i]); 
    }
    
    pc = constrain(pc, 0, 100);
    
    return pc;
}


unsigned char constrain(signed long value, signed long value_min, signed long value_max)
{
      if(value >= value_max)
      {
           return value_max;
      }
      else if(value <= value_min)
      {
           return value_min;
      }
      else
      {
           return value;
      }
}


unsigned char map(signed long value, signed long x_min, signed long x_max, signed long y_min, signed long y_max)
{
    return (y_min + ((((float)(y_max - y_min))/((float)(x_max - x_min))) * ((float)(value - x_min))));
}



