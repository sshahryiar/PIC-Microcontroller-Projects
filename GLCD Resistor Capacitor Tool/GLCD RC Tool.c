#include <18F242.h>
               

#device *= 16                                                                                                      


#fuses HS, PUT, NOWDT, PROTECT, CPD, BROWNOUT, NOEBTR 
#fuses NOLVP, NODEBUG, NOWRT, BORV42, NOEBTRB, NOWRTB 
#fuses NOOSCSEN, NOSTVREN, NOCPB, NOWRT, NOWRTC, NOWRTD
                                                                  
#use delay (clock = 10MHz)
                                                                                                                                                             
 
#include "HDM64GS12.c"
#include "graphics.c" 
#include "sound.c"
#include "background_art.c"      
                                        
                                                  
#define opt     input(pin_A0)   
#define inc     input(pin_A1)
#define dec     input(pin_A2)
                                                                                                                                        
                              
void setup();                              
void load_background(unsigned char type);                   
unsigned char scan_keys();
unsigned char inc_dec(signed char value, signed char max, signed char min, unsigned char x, unsigned char y, int1 type);
void clear_screen1();
void clear_screen2();
void capacitor_tool();
void resistor_tool();
                
                  
void main()
{
    setup();
    while(TRUE)
    {      
        while(TRUE)
        {
            load_background(0);
            while((scan_keys()!=2) && (scan_keys()!=3));
            if((scan_keys()==2))
            {
                capacitor_tool();
            }
            if((scan_keys()==3))
            {
                resistor_tool();
            }
        }
    };                            
}                                
                                    

void setup()                    
{                              
   disable_interrupts(global); 
   glcd_init(OFF); 
   setup_spi(spi_ss_disabled);
   setup_timer_1(t1_disabled);  
   setup_timer_2(t2_disabled,0,1); 
   setup_adc(adc_off);     
   setup_adc_ports(no_analogs);
   setup_ccp1(ccp_off);
   setup_ccp2(ccp_off);
   set_RTCC(0);
   set_timer1(0);
   set_timer2(0);
   delay_us(20);
   glcd_init(ON);  
   enable_interrupts(global);
   enable_interrupts(int_timer1);  
   delay_ms(100);  
}


unsigned char scan_keys()
{
    if(opt)
    {
        generate_tone(300,25);
        delay_ms(50);
        return 1;
    }
    else if(inc)
    {
        generate_tone(900,25);
        delay_ms(50);       
        return 2;              
    }
    else if(dec)
    {
        generate_tone(600,25);
        delay_ms(50);
        return 3;
    }
    else
    {
        return 0;
    }
}


void load_background(unsigned char type)
{                            
   unsigned int16 n=0;
   unsigned int8 i=0;
   unsigned int8 j=0;
   int1 cs=0;
   for(i = 0; i < 8; ++i)       
   {
      output_low(GLCD_DI);                      
      glcd_writeByte(GLCD_LEFT, 0b01000000);   
      glcd_writeByte(GLCD_RIGHT, 0b01000000);
      glcd_writeByte(GLCD_LEFT, i | 0b10111000);
      glcd_writeByte(GLCD_RIGHT, i | 0b10111000);
      output_high(GLCD_DI);         
      for(j = 0; j < 128; ++j)
      {
         if(j<64) 
         {
            cs=GLCD_LEFT;
         }
         else 
         {
            cs=GLCD_RIGHT;
         }
         if(type==1)
         {
            glcd_writeByte(cs, cap[n]);  
         }
         else if(type==2)
         {
            glcd_writeByte(cs, res[n]);
         }
         else
         {
            glcd_writeByte(cs, menu[n]);
         }
         n++;
      }
   }
}


unsigned char inc_dec(signed char value, signed char max, signed char min, unsigned char x, unsigned char y, int1 type)
{
    unsigned char text[15];
    unsigned char text_BL[8]= {"Black  "};
    unsigned char text_BR[8]= {"Brown  "};
    unsigned char text_R[8]=  {"Red    "};
    unsigned char text_O[8]=  {"Orange "};
    unsigned char text_Y[8]=  {"Yellow "};
    unsigned char text_G[8]=  {"Green  "};
    unsigned char text_B[8]=  {"Blue   "};
    unsigned char text_V[8]=  {"Violet "};
    unsigned char text_GR[8]= {"Grey   "};
    unsigned char text_W[8]=  {"White  "};
    unsigned char text_GD[8]= {"Gold   "};            
    unsigned char text_SL[8]= {"Silver "};
    unsigned char text_N[8]=  {"None   "};
    
    while(TRUE)
    {
        if(type==1)
        {
            switch(value)
            {
                case 0:
                {
                    glcd_text35(x, y , text_BL, OFF);
                    delay_ms(20); 
                    glcd_text35(x, y , text_BL, ON); 
                    break;
                }                     
                case 1:
                {
                    glcd_text35(x, y , text_BR, OFF);
                    delay_ms(20);
                    glcd_text35(x, y , text_BR, ON);
                    break;
                }
                case 2:
                {
                    glcd_text35(x, y , text_R, OFF);
                    delay_ms(20); 
                    glcd_text35(x, y , text_R, ON);
                    break;                          
                }
                case 3:
                {
                    glcd_text35(x, y , text_O, OFF);
                    delay_ms(20);
                    glcd_text35(x, y , text_O, ON);     
                    break;
                }
                case 4:
                {
                    glcd_text35(x, y , text_Y, OFF);
                    delay_ms(20);
                    glcd_text35(x, y , text_Y, ON);    
                    break;
                }
                case 5:
                {
                    glcd_text35(x, y , text_G, OFF);
                    delay_ms(20); 
                    glcd_text35(x, y , text_G, ON);          
                    break;
                }
                case 6:                             
                {
                    glcd_text35(x, y , text_B, OFF);
                    delay_ms(20);
                    glcd_text35(x, y , text_B, ON);          
                    break;    
                }
                case 7:
                {
                     glcd_text35(x, y , text_V, OFF);
                    delay_ms(20);
                    glcd_text35(x, y , text_V, ON);      
                    break;
                }
                case 8:
                {
                    glcd_text35(x, y , text_GR, OFF);
                    delay_ms(20);                     
                    glcd_text35(x, y , text_GR, ON);   
                    break;
                }
                case 9:
                {
                    glcd_text35(x, y , text_W, OFF);
                    delay_ms(20);
                    glcd_text35(x, y , text_W, ON);     
                    break;
                }
                case 10:
                {
                    glcd_text35(x, y , text_GD, OFF);
                    delay_ms(20);
                    glcd_text35(x, y , text_GD, ON); 
                    break;
                }
                case 11:
                {
                     glcd_text35(x, y , text_SL, OFF);
                    delay_ms(20);
                    glcd_text35(x, y , text_SL, ON);    
                    break;
                }
                case 12:
                {  
                     glcd_text35(x, y , text_N, OFF);
                    delay_ms(20);
                    glcd_text35(x, y , text_N, ON);    
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        else
        {
            sprintf(text, "%1u", value);            
            glcd_text35(x, y , text, OFF); 
            delay_ms(20);
            glcd_text35(x, y , text, ON); 
        }
        
        if(scan_keys()==2)
        {
            value++;
        }
        if(value>max)
        {
            value=min;
        }
        if(scan_keys()==3)
        {
            value--;
        }
        if(value<min)
        {
            value=max;
        }
        if(scan_keys()==1)
        {   
            if(type==1)
            {
                
                switch(value)
                {
                    case 0:
                    {
                        glcd_text35(x, y , text_BL, OFF);
                        break;
                    }
                    case 1:
                    {
                        glcd_text35(x, y , text_BR, OFF);
                        break;
                    }
                    case 2:
                    {
                        glcd_text35(x, y , text_R, OFF);
                        break;
                    }
                    case 3:
                    {
                        glcd_text35(x, y , text_O, OFF);
                        break;
                    }
                    case 4:
                    {
                        glcd_text35(x, y , text_Y, OFF);
                        break;
                    }
                    case 5:
                    {
                        glcd_text35(x, y , text_G, OFF);
                        break;
                    }
                    case 6:
                    {
                        glcd_text35(x, y , text_B, OFF);
                        break;
                    }
                    case 7:
                    {
                        glcd_text35(x, y , text_V, OFF);
                        break;
                    }
                    case 8:
                    {
                        glcd_text35(x, y , text_GR, OFF);
                        break;
                    }
                    case 9:
                    {
                        glcd_text35(x, y , text_W, OFF);
                        break;
                    }
                    case 10:
                    {
                        glcd_text35(x, y , text_GD, OFF);
                        break;
                    }
                    case 11:
                    {
                        glcd_text35(x, y , text_SL, OFF);
                        break;
                    }
                    case 12:
                    {          
                        glcd_text35(x, y , text_N, OFF);
                        break;
                    }
                    default:
                    {
                        break;
                    }
                }
            }
            else
            {
                glcd_text35(x, y , text, OFF); 
            }
            return value;
        }
    }
}


void clear_screen1()
{
    unsigned char s=0; 
    for(s=0;s<64;s++)
    {
         glcd_line((64+s), 0, (64+s), 63, NO); 
         glcd_line((63-s), 0, (63-s), 63, NO); 
         delay_ms(5);
    }        
    glcd_fillScreen(0);
}


void clear_screen2()
{                           
    unsigned char s=0;
    for(s=0;s<32;s++)
    {
         glcd_line(0, (32+s), 127, (32+s), NO); 
         glcd_line(0, (31-s), 127, (31-s), NO); 
         delay_ms(5);
    } 
    glcd_fillScreen(0);
}


void capacitor_tool()
{
     int1 chk=0;
     unsigned char text_x[3]={"X:"};
     unsigned char text_y[3]={"Y:"};
     unsigned char text_z[3]={"Z:"};
     unsigned char text_cap_val[15];
     unsigned char x_val=0;     
     unsigned char y_val=0;
     unsigned char z_val=0;                  
     float cap_val=0;  
     float temp=0;
     clear_screen1();
     while(TRUE) 
     {
        if(chk==0)
        {
            glcd_fillScreen(0);
            load_background(1);
            glcd_text35(40, 33 , text_x, OFF);  
            glcd_text35(40, 39 , text_y, OFF);
            glcd_text35(40, 45 , text_z, OFF);  
            x_val=inc_dec(x_val,9,0,47,33,0);
            y_val=inc_dec(y_val,9,0,47,39,0);
            z_val=inc_dec(z_val,9,0,47,45,0); 
            generate_tone(2000,50);   
            cap_val = ((x_val*10.0)+(y_val*1.0));
            while(z_val>0)
            {
                cap_val *= 10;                                 
                z_val--;                               
            }                
            if(cap_val>=1000)                   
            {
                temp=cap_val;
                temp=(temp/1000.0);    
                if(temp<1000)
                {
                    sprintf(text_cap_val, "C/nF: %f", temp); 
                }     
                else
                {    
                    temp/=1000.0;  
                    sprintf(text_cap_val, "C/uF: %f", temp);
                }
            }                                             
            if(cap_val<1000)
            {         
                sprintf(text_cap_val, "C/pF: %f", cap_val);
            }            
            glcd_text35(40, 55 , text_cap_val, OFF);  
            chk=1;      
        }
                           
        if(scan_keys()==1)
        {
            delay_ms(500);
            if(scan_keys()==1)
            {                       
                clear_screen2(); 
                break;
            }  
            else
            {         
                x_val=12;     
                y_val=12;
                z_val=12;                  
                cap_val=0;  
                temp=0;
                chk=0;
            }
        }   
     }
     clear_screen2();
     glcd_fillScreen(0);
}


void resistor_tool()
{
    int1 chk=0;  
    float temp=0;
    float res_val=0;
    unsigned char text_col1[7]={"Col 1:"};
    unsigned char text_col2[7]={"Col 2:"};
    unsigned char text_col3[7]={"Col 3:"};
    unsigned char text_col4[7]={"Col 4:"};
    unsigned char text_col5[7]={"Col 5:"};
    unsigned char text_res_val[15];
    unsigned char tol=0;
    unsigned char col1=0;
    unsigned char col2=0;
    unsigned char col3=0;
    unsigned char col4=0;
    unsigned char col5=0; 
    clear_screen1();             
    while(TRUE)                                  
    {                                              
        if(chk==0)
        {
            glcd_fillScreen(0);
            load_background(2);
            glcd_text35(2, 30 , text_col1, OFF);  
            glcd_text35(2, 36 , text_col2, OFF);
            glcd_text35(2, 42 , text_col3, OFF);
            glcd_text35(2, 48 , text_col4, OFF);
            glcd_text35(2, 54 , text_col5, OFF); 
            col1=inc_dec(col1,12,0,27,30,1);
            col2=inc_dec(col2,12,0,27,36,1);
            col3=inc_dec(col3,12,0,27,42,1);
            col4=inc_dec(col4,12,0,27,48,1); 
            col5=inc_dec(col5,12,0,27,54,1);  
            generate_tone(2000,50);    
            if(col5==12)                   
            {                    
                res_val=((float)(10*col1));  
                res_val+=((float)col2);
                while(col3>0)   
                {                    
                    res_val = res_val * 10;
                    col3--;
                }
                tol=col4;                         
            }                    
            else
            {
                res_val=((float)(100*col1));     
                res_val+=((float)(10*col2));          
                res_val+=((float)col3);                                             
                while(col4>0)                                      
                {                                      
                    res_val *= 10;
                    col4--;
                }
                tol=col5;   
            }                
                                           
            if(res_val>999)        
            {                           
                temp=res_val;
                temp=(temp/1000);     
                if(temp<1000)
                {
                    sprintf(text_res_val, "R/k Ohms: %f", temp);
                }
                else
                {
                    temp/=1000;
                    sprintf(text_res_val, "R/M Ohms: %f", temp);
                }                                       
            }             
            if(res_val<1000)
            {                                                                   
                sprintf(text_res_val, "R/Ohms: %f", res_val);   
            }           
            glcd_text35(55, 40 , text_res_val, OFF);  
                            
            if(tol==12)                         
            {
                tol=20;
            }                                  
            else if(tol==11)
            {                                      
                tol=10;
            }
            else if(tol==10)
            {                              
                tol=5;
            }  
            else 
            {
                tol=tol;
            }
            
            sprintf(text_res_val, "Tol/pc: %2u", tol);
            glcd_text35(55, 50 , text_res_val, OFF);   
            
            chk=1;    
        }                                     
        if(scan_keys()==1)           
        {
            delay_ms(500);
            if(scan_keys()==1)
            {                       
                clear_screen2(); 
                break;
            }  
            else                 
            {   
                tol=0;
                col1=12;
                col2=12;
                col3=12;
                col4=12;
                col5=12;
                temp=0;
                res_val=0;
                chk=0; 
            }
        }                              
                                    
    }
}
