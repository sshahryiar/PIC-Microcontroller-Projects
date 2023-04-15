#define B1      PINB0_bit
#define B2      PINB1_bit
#define B3      PINB2_bit

#define LED     PORTB3_bit
#define TX      PORTB4_bit


void setup();
void send_preamble_and_sync();
void data_TX(unsigned char value);


void main() 
{
     setup();
     while(1)
     {
             if(B1)
             {
                   while(B1);
                   data_TX(9);
             }
             if(B2)
             {
                   while(B2);
                   data_TX(119);
             }
             if(B3)
             {
                   while(B3);
                   data_TX(127);
             }
     };
}


void setup()
{
    CLKPR = 0x80;
    CLKPR = 0x00;
    PORTB = 0x00;
    DDRB = 0x1F;
    TCCR0A = 0x00;
    TCCR0B = 0x00;
    TCNT0 = 0x00;
    OCR0A = 0x00;
    OCR0B = 0x00;
    GIMSK = 0x00;
    MCUCR = 0x00;
    TIMSK0 = 0x00;
    ACSR = 0x80;
    ADCSRB = 0x00;
    DIDR0 = 0x00;
    ADCSRA = 0x00;
}


void send_preamble_and_sync()
{
     unsigned char tx = 0;
     
     for(tx = 0; tx < 16; tx++)
     {
             delay_us(300);
             TX = 1;
             LED = 1;
             delay_us(300);
             TX = 0;
             LED = 0;
     }

     delay_us(170);
     TX = 1;
     LED = 1;
     delay_us(80);
     TX = 0;
     LED = 0;
     delay_us(100);
}


void data_TX(unsigned char value)
{
     unsigned char s = 0;
     send_preamble_and_sync();
     for(s = 0; s < 8; s++)
     {
             delay_us(20);
             TX = 1;
             LED = 1;
             if((value & 0x80) != 0)
             {
                 delay_us(50);
             }
             delay_us(80);
             TX = 0;
             LED = 0;
             value <<= 1;
     }
}