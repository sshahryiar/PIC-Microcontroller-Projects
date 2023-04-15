#include <16F877A.h>
 

#device ADC=10


#fuses NOWDT, PROTECT, CPD, NOLVP, HS, NOBROWNOUT, NODEBUG, NOWRT, PUT  
                                                         
                                                                                                     
#use delay (clock = 10MHz) 

                                                           
#include "lcd.c"   


#define T_channel          0
#define RH_channel         1

#define Temp_Conv_Const    0.48876
#define VDD_Max_Count      1023.0
#define R_Ext              10000.0
#define t_step             5
#define min_humidity       20


#define t_entries          13
#define i_entries          15
                                

const unsigned char symbol[8] = {0x00, 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00};


const unsigned long long impedance_ohms_table[i_entries][t_entries] = 
{
   //Temperature 
   //00       05         10        15        20       25       30       35       40       45       50       55       60        Relative Humidity   /  RH Step
   
   //T Step
   // 0         1         2         3         4        5        6        7        8        9       10       11       12       

     17500000, 15000000, 12500000, 10000000, 6700000, 5000000, 3900000, 3000000, 2400000, 1750000, 1450000, 1150000, 970000,   //20%              /   0 
     13000000, 10000000,  7000000,  5000000, 3400000, 2600000, 1900000, 1500000, 1100000,  880000,  700000,  560000, 450000,   //25%              /   1
      6400000,  4600000,  3200000,  2300000, 1750000, 1300000,  970000,  740000,  570000,  420000,  340000,  270000, 215000,   //30%              /   2
      2900000,  2100000,  1500000,  1100000,  850000,  630000,  460000,  380000,  280000,  210000,  170000,  150000, 130000,   //35%              /   3
      1400000,  1000000,   750000,   540000,  420000,  310000,  235000,  190000,  140000,  110000,   88000,   70000,  57000,   //40%              /   4
       700000,   500000,   380000,   280000,  210000,  160000,  125000,  100000,   78000,   64000,   50000,   41000,  34000,   //45%              /   5
       370000,   260000,   200000,   150000,  115000,   87000,   69000,   56000,   45000,   38000,   31000,   25000,  21000,   //50%              /   6
       190000,   140000,   110000,    84000,   64000,   49000,   39000,   33000,   27000,   24000,   19500,   17000,  14000,   //55%              /   7
       105000,    80000,    62000,    50000,   39000,   31000,   25000,   20000,   17500,   15000,   13000,   11000,   9400,   //60%              /   8
        62000,    48000,    37000,    30000,   24000,   19500,   16000,   13000,   11500,   10000,    8600,    7600,   6800,   //65%              /   9
        38000,    30000,    24000,    19000,   15500,   13000,   10500,    9000,    8000,    7000,    6000,    5400,   4800,   //70%              /   10
        23000,    18000,    15000,    12000,   10000,    8400,    7200,    6200,    5600,    4900,    4200,    3800,   3400,   //75%              /   11
        15500,    12000,    10000,     8000,    7000,    5700,    5000,    4300,    3900,    3400,    3000,    2700,   2500,   //80%              /   12
        10500,     8200,     6800,     5500,    4800,    4000,    3500,    3100,    2800,    2400,    2100,    1900,   1800,   //85%              /   13
         7100,     5300,     4700,     4000,    3300,    2800,    2500,    2200,    2000,    1800,    1550,    1400,   1300    //90%              /   14
};


                          
void setup(void);             
void lcd_load_custom_symbol(void); 
unsigned long get_ADC_avg(unsigned char channel);
float get_temperature(void);
float get_impedance(void);
unsigned char binary_search_impedance_zone(unsigned long long value, unsigned char t_zone);
       
                                                                                                                    
                                                                                                                        
void main()                                                                    
{                                                         
   unsigned char a = 0;
   unsigned char b = 0;
   
   float t = 0;
   unsigned long long imp = 0;
   
   setup();
   
   lcd_gotoxy(1, 1);
   lcd_putc("HR202 R.H & Tmp Demo");
   lcd_gotoxy(1, 2);
   lcd_putc("Tc/ C:");  
   lcd_gotoxy(5, 2);
   lcd_putc(0);
   lcd_gotoxy(1, 3);
   lcd_putc("R.H/%:");
   lcd_gotoxy(1, 4);
   lcd_putc("Imp/R:");
                       
   while(TRUE)                               
   {                               
      t = get_temperature();
      imp = get_impedance();
      
      a = (t / t_step);
      b = binary_search_impedance_zone(imp, a);
      
      lcd_gotoxy(17, 2);
      printf(lcd_putc, "%2.1g", t);  
      lcd_gotoxy(19, 3);
      printf(lcd_putc, "%2u", ((b * t_step) + min_humidity));
      lcd_gotoxy(11, 4);
      printf(lcd_putc, "%9lu ", imp);
      
      delay_ms(400);
     
   };                                             
}                                                 
                                   
                                         
void setup(void)
{                                                                                               
   disable_interrupts(global);              
   setup_timer_0(T0_internal); 
   setup_timer_1(T1_disabled);
   setup_timer_2(T2_disabled, 255, 1);
   set_timer0(0);
   set_timer1(0);           
   set_timer2(0);                                                           
   setup_CCP1(CCP_off);
   setup_CCP2(CCP_off);
   setup_psp(PSP_disabled);                                
   setup_comparator(NC_NC_NC_NC);       
   setup_SPI(SPI_disabled);                   
   port_B_pullups(FALSE);  
   setup_adc_ports(RA0_RA1_RA3_ANALOG);
   setup_ADC(ADC_CLOCK_INTERNAL | ADC_CLOCK_DIV_16);                    
   disable_interrupts(global);  
   lcd_init();
   lcd_load_custom_symbol();     
   lcd_putc("\r");
}                                                            

                         
void lcd_load_custom_symbol(void) 
{
   unsigned char s = 0;                 
                                                        
   lcd_send_byte(0, 0x40); 
   
   for(s = 0; s < 8; s++)        
   {                           
        lcd_send_byte(1, symbol[s]);          
   }    
   
   lcd_send_byte(0, 0x80);   
} 


unsigned long get_ADC_avg(unsigned char channel)
{
   unsigned char samples = 16;
   unsigned long avg = 0;
   
   set_ADC_channel(channel);
   delay_ms(1);
   
   while(samples > 0)
   {
      read_ADC(ADC_start_only);
      while(!ADC_done());
      avg += read_ADC(ADC_read_only);
      samples--;
   };
   
   avg >>= 4;
   
   return avg;
}


float get_temperature(void)
{
   float temp = 0.0;
   
   temp = get_ADC_avg(T_channel);   
   temp *= Temp_Conv_Const;
   
   return temp;
}


float get_impedance(void)
{
   float impedance = 0.0;
   
   impedance = (float)get_ADC_avg(RH_channel);
   impedance = (R_Ext / ((VDD_Max_Count / impedance) - 1));
    
   return impedance;
}


unsigned char binary_search_impedance_zone(unsigned long long value, unsigned char t_zone)
{
    signed char h = (i_entries - 1);
    signed char l = 0;
    signed char m = (h / 2); 
    
    while(l <= h)
    {              
      if(value < (impedance_ohms_table[(unsigned char)m][t_zone]))
      {
         l = (m + 1);
      }
      
      else
      {
         h = (m - 1);
      }
      
      m = ((h + l) / 2);
        
    }
    
    return ((unsigned char)m);
}

            
                                                                                          

