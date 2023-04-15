#include <18F4520.h>


#device *= 16
#device ADC = 10
#device CONST = ROM
#device CONST = READ_ONLY


#fuses NOWDT, INTRC_IO, FCMEN, IESO, BROWNOUT, BORV27, NODEBUG, CPD, NOWRTB, NOWRTD, PUT
#fuses MCLR, NOPBADEN, NOLPT1OSC, NOLVP, STVREN, NOXINST, NOWRT, PROTECT, NOCPB, EBTR


#use delay (internal = 32MHz)
#use fast_io(A)
#use fast_io(C)
#use fast_io(D)


#include "XPT2046.c"
#include "BD663474.c"


void setup();
void test_colours();
void test_2D_graphics();
void draw_flag();
void test_RGB();
void print_test_data();


void main()
{
    unsigned char msg[11] = {"Draw Check"};
    
    setup();
    
    test_RGB();
    test_colours();
    test_2D_graphics();
    draw_flag();
    print_test_data();
    print_str(10, 10, 2, White, Black, msg);
    
    while(TRUE)
    {
        if(TP_INT == FALSE)
        {
            TP_read_coordinates(&tpx, &tpy);
            Draw_Circle(tpx, tpy, 6, YES, White);
        }
    };
}


void setup()
{
    disable_interrupts(GLOBAL);
    setup_WDT(WDT_off);
    setup_oscillator(OSC_32MHZ | OSC_PLL_ON);
    port_B_pullups(FALSE);
    setup_comparator(NC_NC_NC_NC);
    setup_ADC(ADC_off);
    setup_ADC_ports(no_analogs);
    setup_PSP(PSP_disabled);
    setup_SPI(SPI_disabled | SPI_SS_disabled);
    setup_SPI2(SPI_disabled | SPI_SS_disabled);
    setup_timer_0(T0_off | T0_8_BIT);
    set_timer0(0);
    setup_timer_1(T1_disabled);
    set_timer1(0); 
    setup_timer_2(T2_disabled, 255, 1);
    set_timer2(0); 
    setup_timer_3(T1_disabled);
    set_timer3(0); 
    setup_CCP1(CCP_off);
    setup_CCP2(CCP_off);
    TFT_init();
    tp_init();
    delay_ms(200);
}


void draw_flag()
{
    Draw_Rectangle(20, 20, 219, 299, YES, SQUARE, Green, Green);
    Draw_Circle(120, 160, 60, YES, Red);
    delay_ms(4000);
    TFT_fill(Black);
}


void test_RGB()
{
    TFT_fill(Red);
    delay_ms(600);
    TFT_fill(Green);
    delay_ms(600);
    TFT_fill(Blue);
    delay_ms(600);
    TFT_fill(Black);
}


void test_colours()
{
    Draw_Rectangle(0, 0, 23, 319, YES, SQUARE, Dark_Grey, Dark_Grey);
    Draw_Rectangle(24, 0, 47, 319, YES, SQUARE, Light_Grey, Light_Grey);
    Draw_Rectangle(48, 0, 71, 319, YES, SQUARE, White, White);
    Draw_Rectangle(72, 0, 95, 319, YES, SQUARE, Cyan, Cyan);
    Draw_Rectangle(96, 0, 119, 319, YES, SQUARE, Dark_Blue, Dark_Blue);
    Draw_Rectangle(120, 0, 143, 319, YES, SQUARE, Purple, Purple);
    Draw_Rectangle(144, 0, 167, 319, YES, SQUARE, Red, Red);
    Draw_Rectangle(168, 0, 191, 319, YES, SQUARE, Orange, Orange);
    Draw_Rectangle(192, 0, 215, 319, YES, SQUARE, Yellow, Yellow);
    Draw_Rectangle(216, 0, 239, 319, YES, SQUARE, Green, Green);
    
    delay_ms(2000);
    TFT_fill(Black);
}


void test_2D_graphics()
{
    signed long i = 0;

    TFT_fill(Black);

    for(i = 10; i <= 310; i += 10)
    {
        Draw_V_Line(i, 20, 300, White);
    }
    for(i = 20; i <= 300; i += 10)
    {
        Draw_H_Line(10, 230, i, White);
    }

    delay_ms(3000);

    TFT_fill(Black);
    for(i = 0; i < 240; i += 5)
    {
        Draw_Line(0, 0, i, 319, Light_Blue);
    }

    for(i = 239; i > -1; i -= 5)
    {
        Draw_Line(239, 319, i, 0, Blue);
    }

    delay_ms(3000);

    TFT_fill(Black);

    Draw_Rectangle(9, 9, 230, 310, NO, ROUND, Blue, Black);
    Draw_Rectangle(19, 19, 220, 300, YES, SQUARE, Cyan, Black);

    delay_ms(3000);

    TFT_fill(Black);
 
    Draw_Circle(119, 159, 80, NO, Red);
    Draw_Circle(119, 159, 40, YES, Yellow);


    delay_ms(3000);

    TFT_fill(Black);

    Draw_Triangle(10, 310, 120, 10, 230, 310, NO, Green);
    delay_ms(900);
    Draw_Triangle(10, 310, 120, 10, 230, 310, YES, Grey_Blue);

    delay_ms(3000);

    TFT_fill(Black);
}


void print_test_data()
{
    unsigned char msg1[11] = {"MicroArena"};
    unsigned char msg2[36] = {"https://www.facebook.com/MicroArena"};
    
    unsigned char s = 0;
    signed char c = -9;

    signed long i = -9;

    float f = -9.0;
    
    print_str(10, 10, 1, Red, Black, msg1);
    print_str(10, 30, 2, Green, Black, msg1);
    print_str(10, 60, 3, Blue, Black, msg1);
    
    print_str(10, 90, 3, White, Black, msg2);
    
    for(s = 0; s < 128; s++)
    {
        print_C(10, 160, 1, Red, Black, c);
        print_I(60, 160, 2, Green, Black, i);
        print_F(120, 160, 3, Blue, Black, f, 1);
        
        c++;
        i++;
        f += 0.1;
    }
    TFT_fill(Black);
}



