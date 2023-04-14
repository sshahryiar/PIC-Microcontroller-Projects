/*
        Author: Shawon M. Shahryiar
        Micro: PIC12F683
        Project: Univeral Logic Probe
        Date:   04.02.2012
        Compiler: MikroC Pro for PIC v.5.3.0
*/

//Definition of shift register pin names//

#define     data_pin        GP5_bit
#define     clock_pin       GP4_bit
#define     strobe_pin      GP2_bit


//Definition of global variables//

float high_level = 0.0;           //minimum logic high voltage value storage
float low_level = 0.0;            //maximum logic low voltage value storage
const unsigned short disp [4] =
{0x38, 0x3E, 0x76, 0x00};         //Symbols for display - L, U, H and blank


//Function prototypes//

static void setup(void);
static void test_display(void);
static void test_supply(void);
void display(unsigned char val);
float adc_avg(unsigned char ch);
void check_logic(void);


void main(void)
{
    setup();           //Setup the micro for operation
    test_supply();     //Check supply voltage and choose convention
    test_display();    //Check display

    for(;;)             //Infinite loop
    {
            check_logic();   //Test logic level at logic input terminal
    }
}


static void setup(void)
{
     //Set directions and initial states of I/O pins//
     TRISIO5_bit = 0;
     TRISIO4_bit = 0;
     TRISIO3_bit = 1;
     TRISIO2_bit = 0;
     TRISIO1_bit= 1;
     TRISIO0_bit = 1;
     data_pin = 0;
     strobe_pin = 0;
     clock_pin = 0;
     adc_init();          //Initiate internal ADC module
     delay_ms(100);       //Wait of some time
}


static void test_display(void)
{
     unsigned short test = 0;
     while(test<3)                       //Test seven segment display
     {
                  display(disp[test]);   //Put called symbol on display
                  delay_ms(400);         //Wait for some time
                  test++;                //Continue test
     }
     display(disp[3]);                   //Turn off display
     delay_ms(300);                      // Wait for some time
}


static void test_supply(void)
{
     register float supply = 0;
     supply = adc_avg(1);                //Take the average of supply voltage
     delay_ms(150);                      //Wait for some time
     if(((supply >= 4.75) && (supply <= 5.25)))   //TTL convention test
     {
                     high_level = 2.2;
                     low_level = 0.8;
     }                                            //CMOS convention test
     else
     {
                     high_level = (supply*0.67);
                     low_level = (supply*0.33);
     }                                            //Wait for a while
     delay_ms(50);
}


void display(unsigned char val)
{
    unsigned short clk = 0;
    unsigned short d = 0;
    strobe_pin = 0;                               //Hold strobe pin low
    for(clk = 0; clk < 8; clk++)                  //Do following job eight times
    {
        d = (0x80 & val);                         //Read the MSB of val
        if(d == 0)                                //If the MSB is zero
        {
             data_pin = 0;                        //then send 0
        }
        else                                      //otherwise send 1
        {
             data_pin = 1;
        }
        clock_pin = 1;                            //Pull clock pin high
        delay_ms(1);                              //Wait for a while
        val = val << 1;                           //Left shift val
        clock_pin = 0;                            //Hold clock pin low
        delay_ms(1);                              //Wait for a while
    }
    strobe_pin = 1;                               //Pull strobe pin high
    delay_ms(1);                                  //Wait for a while
}


float adc_avg(unsigned char ch)
{
         register unsigned int temp = 0;
         unsigned short samples = 0;
         register float avg = 0;
         delay_ms(1);
         for(samples = 1; samples <= 16; samples += 1)   //Take 16 ADC samples
         {
                         temp = adc_read(ch);            //Read assigned ADC
                         temp += temp;                   //channel data and
                         delay_us(25);                   //store temporarily
         }
         avg = (temp * 5.0);                            //Convert to readable
         avg /= 1023.0;                                 //value other than count
         avg *= 10.0;                                   //Take voltage divider
         avg /= 16.0;                                   //and number of samples
         return avg;                                    //into account
}


void check_logic(void)
{
        register float vin = 0;
        vin = adc_avg(0);                   //Read logic input channel
        delay_ms(20);                       //Wait for some time
        if(vin >= high_level)               //Check if it is high
        {
            display(disp[2]);               //Send 'H' to display
        }
        else if(vin <= low_level)           //Check if it is low
        {
            display(disp[0]);               //Send 'L' to display
        }
        else                                //Otherwise it is undefined
        {
            display(disp[1]);               //Toggle and send 'U' to display
            delay_ms(400);
            display(disp[3]);
            delay_ms(400);
        }
}