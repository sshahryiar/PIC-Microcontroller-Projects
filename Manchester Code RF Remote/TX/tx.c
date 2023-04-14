#define BT1       PINB0_bit
#define BT2       PINB1_bit
#define BT3       PINB2_bit

#define LED       PORTB3_bit
#define TX        PORTB4_bit


void setup();
void send_one();
void send_zero();
void send_data(unsigned char value);


void main() 
{
     setup();
     
     while(1)
     {
             if(BT1)
             {
                 while(BT1);
                 send_data(4);
             }
             if(BT2)
             {
                 while(BT2);
                 send_data(9);
             }
             if(BT3)
             {
                 while(BT3);
                 send_data(16);
             }
     };
}


void setup()
{
    CLKPR = 0x80;
    CLKPR = 0x00;
    PORTB = 0x00;
    DDRB = 0x18;
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


void send_one()
{
    TX = 0;
    LED = 0;
    delay_ms(1);
    TX = 1;
    LED = 1;
    delay_ms(1);
}


void send_zero()
{
    TX = 1;
    LED = 1;
    delay_ms(1);
    TX = 0;
    LED = 0;
    delay_ms(1);
}


void send_data(unsigned char value)
{
     unsigned char s = 0;
     unsigned char t = 0;
     
     for(s = 0; s < 2; s++)
     {
           send_one();
     }
     send_zero();
     
     for(s = 0; s < 8; s++)
     {
           if((value & 0x80) != 0)
           {
               send_one();
           }
           else
           {
               send_zero();
           }
           
           value <<= 1;
     }
     
     TX = 0;
     LED = 0;
}