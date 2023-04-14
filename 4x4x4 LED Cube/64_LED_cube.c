#include <16F877A.h>


#device *=16


#fuses HS, NOWDT, CPD, NODEBUG, NOWRT
#fuses BROWNOUT, NOLVP, PROTECT, PUT


#use delay (clock = 10MHz)
#use fast_IO(B)                 
#use fast_IO(C)

                      
#BYTE PORTB = 0x006             
#BYTE PORTC = 0x007
#BYTE TRISB = 0x086
#BYTE TRISC = 0x087


#include <stdlib.h>
                           

#define reload      65438
#define cnt_max     0xFF
#define break_time  90

#define L0          pin_A0
#define L1          pin_A1
#define L2          pin_A2
#define L3          pin_A3


unsigned char cnt=0;
unsigned char L0_duty=0;
unsigned char L1_duty=0;
unsigned char L2_duty=0;
unsigned char L3_duty=0;
unsigned long ctrl_a=0;
unsigned long ctrl_b=0;



void setup();
void pattern_data
(
    unsigned char col_a,
    unsigned char col_b, 
    unsigned char row0, 
    unsigned char row1, 
    unsigned char row2, 
    unsigned char row3, 
    unsigned long speed
);
void all_off();
void water_drops();
void matrix();
void fly_away();
void rain_fall();
void random1();
void random2();
void random3();
void all_glowing();     
void shock();  
void level_move();  
void alternate();   
void rain();
                     

#int_timer1

void pwm_level_control()
{
    set_timer1(reload);  
    cnt++;
    if(cnt < cnt_max)
    {
        if(cnt == L0_duty)
        {
            output_low(L0);
        }
        if(cnt == L1_duty)
        {
            output_low(L1);
        }
        if(cnt == L2_duty)
        {
            output_low(L2);
        }
        if(cnt == L3_duty)
        {
            output_low(L3);
        }
    }
    else
    {
        cnt = 0x00;
        output_A(0x0F);
    }
}


void main()
{
    setup();
    all_off();
    while(TRUE)
    {
        water_drops();
        all_off(); 
        
        matrix();  
        all_off();
        
        fly_away();
        all_off();
        
        
        rain_fall();
        all_off();
        
        random1();
        all_off();
        
        random2();
        all_off();
        
        random3();               
        all_off();
        
        all_glowing();
        all_off();            
        
        shock();            
        all_off();
        
        level_move();
        all_off();      
        
        alternate();   
        all_off();
    }
}
                              

void setup()
{
    TRISB = 0x00;
    TRISC = 0x00;
    PORTB = 0x00;
    PORTC = 0x00;
    port_B_pullups(false);
    setup_ADC(ADC_off);
    setup_ADC_ports(no_analogs);
    setup_timer_1(T1_internal);
    set_timer1(reload);  
    enable_interrupts(global);
    enable_interrupts(int_timer1);
    delay_ms(2000);
}


void all_off()
{
    pattern_data(0, 0, 0, 0, 0, 0, break_time); 
}                                             


void pattern_data
(
    unsigned char col_a,
    unsigned char col_b, 
    unsigned char row0, 
    unsigned char row1, 
    unsigned char row2, 
    unsigned char row3, 
    unsigned long speed
)
{
    L0_duty = row0;
    L1_duty = row1;
    L2_duty = row2;
    L3_duty = row3;
    PORTB = col_a;
    PORTC = col_b;                        
    delay_ms(speed);       
}                                


void water_drops()         
{                                                       
    unsigned char s = 0;          
    ctrl_a = 1;              
    ctrl_b = 0;             
    setup_timer_1(T1_disabled);
    for(ctrl_a = 1; ctrl_a <= 128; ctrl_a <<= 1)
    {
            s = 8;
            PORTB = ctrl_a;
            PORTC = ctrl_b;
            output_A(0x00);
            while(s > 0)  
            {                                                
                output_A(s);       
                delay_ms(150);
                s>>=1;         
            } 
            
    }                                                                      
    
    ctrl_a = 0;
    ctrl_b = 1;
    for(ctrl_b = 1; ctrl_b <= 128; ctrl_b <<= 1)
    {  
            PORTB = ctrl_a;       
            PORTC = ctrl_b;
            output_A(0x00);
            for(s = 8; s > 1; s >>= 1)  
            {                    
                output_A(s);                 
                delay_ms(120);       
            }   
            output_A(1);                 
            delay_ms(120); 
    }       
                                               
    
    ctrl_a = 0;
    ctrl_b = 0; 
    output_A(0x00);
    setup_timer_1(T1_internal);       
}                          


void matrix()  
{      
    unsigned char s = 0;          
    unsigned char n = 0;             
    setup_timer_1(T1_disabled);  
    for(n = 1; n < 255; n++)                                   
    {
            PORTB = n;                
            PORTC = n;
            output_A(0x00);
            for(s = 8; s > 1; s >>= 1) 
            {                   
                output_A(s);       
                delay_ms(20);       
            }             
            output_A(1);                 
            delay_ms(20); 
    }                                                                            
           
    cnt = 0;                             
    setup_timer_1(T1_internal);
    set_timer1(reload);           
    pattern_data(0, 0, 0, 0, 0, 0, 10);               
}       
                                              

void fly_away()                     
{                    
    unsigned long duration = 0;    
    for(duration = 30; duration >= 10; duration -= 5)
    {                   
            ctrl_a = 1;
            ctrl_b = 0;
            for(ctrl_a = 1; ctrl_a <= 128; ctrl_a <<= 1)
            {
                pattern_data(ctrl_a, ctrl_b, 250, 0, 0, 0, duration);            
                pattern_data(ctrl_a, ctrl_b, 0, 250, 0, 0, duration); 
                pattern_data(ctrl_a, ctrl_b, 0, 0, 250, 0, duration);
                pattern_data(ctrl_a, ctrl_b, 0, 0, 0, 250, duration); 
            }
            
            ctrl_a = 0;
            ctrl_b = 1;
            for(ctrl_b = 1; ctrl_b <= 128; ctrl_b <<= 1)
            {
                pattern_data(ctrl_a, ctrl_b, 250, 0, 0, 0, duration);        
                pattern_data(ctrl_a, ctrl_b, 0, 250, 0, 0, duration);        
                pattern_data(ctrl_a, ctrl_b, 0, 0, 250, 0, duration);     
                pattern_data(ctrl_a, ctrl_b, 0, 0, 0, 250, duration);
            }
            
            ctrl_a = 0;
            ctrl_b = 0;
    }
}               

          
void rain_fall()
{                                                             
    unsigned char times = 0;                             
    while(times < 8)
    {                                                                       
        pattern_data(0xFF, 0xFF, 0, 0, 0, 240, 25);
        pattern_data(0, 0, 0, 0, 0, 0, 10);  
        pattern_data(0xFF, 0xFF, 0, 0, 240, 0, 25);  
        pattern_data(0, 0, 0, 0, 0, 0, 10);  
        pattern_data(0xFF, 0xFF, 0, 240, 0, 0, 25);  
        pattern_data(0, 0, 0, 0, 0, 0, 10);       
        pattern_data(0xFF, 0xFF, 240, 0, 0, 0, 25); 
        pattern_data(0, 0, 0, 0, 0, 0, 10);  
        times+=1;                                
    }                                  
}


void random1()                 
{
    unsigned char times = 0;
    unsigned char buffer1 = 0;
    unsigned char buffer2 = 0;
    unsigned char buffer3 = 0;
    unsigned char buffer4 = 0;
    unsigned char buffer5 = 0;
    unsigned char buffer6 = 0;
    unsigned char buffer7 = 0;
    while(times < 16)
    {                                 
        buffer1 = (rand()>>7);
        buffer2 = (rand()>>7);
        buffer3 = (rand()>>7);
        buffer4 = (rand()>>7);
        buffer5 = (rand()>>7);
        buffer6 = (rand()>>7);
        buffer7 = (rand()>>9);
        pattern_data(buffer1, buffer2, buffer3, buffer4, buffer5, buffer6, buffer7);
        ++times;
        delay_ms(100);
    }
}                        


void random2()
{
    unsigned char times = 0;
    unsigned char buffer1 = 0;
    unsigned char buffer2 = 0;           
    while(times < 16)
    {
        buffer1 = (rand()>>7);
        buffer2 = (rand()>>7);
        pattern_data(buffer1, buffer2, 180, 180, 180, 180, 25);
        ++times;        
    }
}


void random3()
{
    unsigned char pwm_duty = 0;
    while(pwm_duty <255)                                     
    {
        pattern_data((rand()>>7), (rand()>>7), pwm_duty, pwm_duty, pwm_duty, pwm_duty, 30);
        ++pwm_duty;  
    }
    while(pwm_duty > 0)
    {
        pattern_data((rand()>>7), (rand()>>7), pwm_duty, pwm_duty, pwm_duty, pwm_duty, 30);
        --pwm_duty;  
    }
}


void all_glowing()
{
    unsigned char pwm_duty = 0;
    while(pwm_duty < 255)
    {
        pattern_data(0xFF, 0xFF, pwm_duty, pwm_duty, pwm_duty, pwm_duty, 30);
        ++pwm_duty;  
    }
    while(pwm_duty > 0)
    {
        pattern_data(0xFF, 0xFF, pwm_duty, pwm_duty, pwm_duty, pwm_duty, 30);
        --pwm_duty;  
    }
}
                                                     

void shock() 
{                            
    unsigned char times=0;
    while(times<4)
    {
        pattern_data(0xFF, 0xFF, 250, 250, 250, 250, 150); 
        pattern_data(0x00, 0x00, 250, 250, 250, 250, 350); 
        ++times; 
    }                      
}                                  
                         

void level_move()                                   
{                                                                  
    unsigned char times=0;
    while(times<4)
    {                        
        pattern_data(0xFF, 0xFF, 240, 180, 120, 60, 100);      
        pattern_data(0xFF, 0xFF, 60, 240, 180, 120, 100);
        pattern_data(0xFF, 0xFF, 120, 60, 240, 180, 100);      
        pattern_data(0xFF, 0xFF, 180, 120, 60, 240, 100);          
                                                   
        pattern_data(0xFF, 0xFF, 60, 120, 180, 240, 100); 
        pattern_data(0xFF, 0xFF, 120, 180, 240, 60, 100);             
        pattern_data(0xFF, 0xFF, 180, 240, 60, 120, 100);      
        pattern_data(0xFF, 0xFF, 240, 60, 120, 180, 100);   
        times++;                                
    }                                           
}      


void alternate() 
{  
    unsigned char times=0;
    while(times<4)
    {                        
        pattern_data(0xAA, 0x55, 150, 0, 0, 0, 100);      
        pattern_data(0x55, 0xAA, 0, 150, 0, 0, 100);
        pattern_data(0xAA, 0x55, 0, 0, 150, 0, 100);      
        pattern_data(0x55, 0xAA, 0, 0, 0, 150, 100);      
                                                 
        pattern_data(0xAA, 0x55, 0, 0, 0, 150, 100); 
        pattern_data(0x55, 0xAA, 0, 0, 150, 0, 100);
        pattern_data(0xAA, 0x55, 0, 150, 0, 0, 100);      
        pattern_data(0x55, 0xAA, 150, 0, 0, 0, 100);   
        times++;                                
    }  
}  


void rain()
{
    unsigned char s = 0;          
    unsigned char n = 0;  
    setup_timer_1(T1_disabled);  
    for(n = 1; n < 25; n++)    
    {         
            PORTB = 0xAA;                
            PORTC = 0x55;   
            output_A(0x00);
            for(s = 8; s > 1; s >>= 1) 
            {                   
                output_A(s);                    
                delay_ms(20);                                                                         
            }             
            output_A(1);                 
            delay_ms(20); 
    }                                                                            
           
    cnt = 0;                             
    setup_timer_1(T1_internal);     
    set_timer1(reload);           
    pattern_data(0, 0, 0, 0, 0, 0, 10);  
}
                                                          
