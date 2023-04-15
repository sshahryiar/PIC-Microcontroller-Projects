#include <12F675.h>


#device *= 16


#fuses NOWDT, INTRC_IO, PROTECT, CPD, NOMCLR, PUT, BROWNOUT


#use delay(internal = 4MHz)


#include <stdlib.h>


#define OUT_A        pin_A0
#define OUT_B        pin_A1
#define OUT_C        pin_A4
#define OUT_D        pin_A5
#define OUT_TGL      pin_A2

#define BUTTON       !input(pin_A3)


void setup();
void welcome();
void display_output(unsigned char value, short toggler_state);


void main()
{
    unsigned char cnt = 0x00;
    
    short toggler = FALSE;
    
    setup();
    welcome();
    
    while(TRUE)
    {
        if(BUTTON)
        {
            while(BUTTON)
            {
                display_output(0, TRUE);
                delay_ms(45);
                display_output(0, FALSE);
                delay_ms(45);
            };
            
            cnt = get_timer0();
            srand(cnt);
            cnt = (rand() & 0x07);
            set_timer0(cnt);
            toggler = ~toggler;
            display_output(cnt, toggler);
            delay_ms(90);
        }
    };
}


void setup()
{
    disable_interrupts(global);
    setup_comparator(NC_NC_NC_NC);
    setup_ADC(ADC_off);
    setup_ADC_ports(no_analogs);
    setup_timer_0(T0_internal);
    setup_timer_1(T1_disabled);
    set_timer0(0);
    set_timer1(0);
    delay_ms(100);
}


void welcome()
{
    unsigned char i = 0;
    
    for(i = 0; i <= 6; i++)
    {
        display_output(i, TRUE);   
        delay_ms(600);
    }
    
    display_output(7, TRUE); 
    
    for(i = 0; i <= 6; i++)
    {
        display_output(i, FALSE);   
        delay_ms(600);
    }
    
    display_output(7, FALSE);   
}


void display_output(unsigned char value, short toggler_state)
{
   short mask = FALSE;
   
   switch(toggler_state)
   {
      case TRUE:
      {
          output_high(OUT_TGL);
          mask = TRUE;
          break;
      }
      default:
      {
          output_low(OUT_TGL);
          mask = FALSE;
          break;
      }
   }
   
   switch(value)
   {
       case 0:
       {
           set_tris_a(0xFF);
           break;
       }       
       case 1:
       {
           output_float(OUT_A);
           output_float(OUT_B);
           output_float(OUT_C);          
           output_bit(OUT_D, ~mask);
           break;
       }
       case 2:
       {
           output_bit(OUT_A, ~mask);          
           output_float(OUT_B);
           output_float(OUT_C);
           output_float(OUT_D);  
           break;
       }
       case 3:
       {
           output_bit(OUT_B, ~mask);
           output_bit(OUT_D, ~mask);
           output_float(OUT_A);
           output_float(OUT_C);  
           break;
       }
       case 4:
       {
           output_bit(OUT_A, ~mask);
           output_bit(OUT_B, ~mask);
           output_float(OUT_C);
           output_float(OUT_D);  
           break;
       }
       case 5:
       {
           output_bit(OUT_A, ~mask);
           output_bit(OUT_B, ~mask);
           output_bit(OUT_D, ~mask);
           output_float(OUT_C);
           break;
       }
       case 6:
       {
           output_bit(OUT_A, ~mask);
           output_bit(OUT_B, ~mask);
           output_bit(OUT_C, ~mask);
           output_float(OUT_D);
           break;
       }
       default:
       {
           output_bit(OUT_A, mask);
           output_bit(OUT_B, mask);
           output_bit(OUT_C, mask);
           output_bit(OUT_D, mask);
           break;
       }
   }
}
