#include "SSD1306.h"


void OLED_init()
{
    OLED_clear_buffer();
    
    OLED_reset_sequence();
                                                      
    OLED_write((Set_Display_ON_or_OFF_CMD + Display_OFF) , CMD);;
                                                                                         
    OLED_write(Set_Display_Clock_CMD, CMD);
    OLED_write(0x80, CMD);
    
    OLED_write(Set_Multiplex_Ratio_CMD, CMD);
    OLED_write(0x3F, CMD);                        
    
    OLED_write(Set_Display_Offset_CMD, CMD);
    OLED_write(0x00, CMD);
                                                                     
    OLED_write((Set_Display_Start_Line_CMD | 0x00), CMD);                                                             
    
    OLED_write(Set_Charge_Pump_CMD, CMD);
    OLED_write((Set_Higher_Column_Start_Address_CMD | 0x04), CMD);
                                                                         
    OLED_write(Set_Memory_Addressing_Mode_CMD, CMD);
    OLED_write(Page_Addressing_Mode, CMD);
                                                                    
    OLED_write((Set_Segment_Remap_CMD | Column_Address_0_Mapped_to_SEG127), CMD);
    
    OLED_write((Set_COM_Output_Scan_Direction_CMD | Scan_from_COM63_to_0), CMD);
    
    OLED_write(Set_Common_HW_Config_CMD, CMD);
    OLED_write(0x12, CMD);

    OLED_write(Set_Contrast_Control_CMD, CMD);
    OLED_write(0xCF, CMD);
    
    OLED_write(Set_Pre_charge_Period_CMD, CMD);
    OLED_write(0xF1, CMD);
    
    OLED_write(Set_VCOMH_Level_CMD, CMD);
    OLED_write(0x40, CMD);

    OLED_write((Set_Entire_Display_ON_CMD | Normal_Display), CMD);

    OLED_write((Set_Normal_or_Inverse_Display_CMD | Non_Inverted_Display), CMD);
    
    OLED_write((Set_Display_ON_or_OFF_CMD + Display_ON) , CMD);
    
    OLED_gotoxy(0, 0);
    
    OLED_clear_screen();
}


void OLED_reset_sequence()
{                
    output_high(SCK_pin);                 
    output_low(RST_pin);
    delay_ms(40);
    output_high(RST_pin); 
    output_low(SCK_pin);   
    delay_ms(40);
}


void OLED_write(unsigned char value, unsigned char type)
{
    unsigned char s = 0x08;          

    output_low(CS_pin); 
    output_bit(DC_pin, type);

    while(s > 0)
    {
        if((value & 0x80) != 0)
        {
            output_high(SDA_pin);
        }
        else
        {
            output_low(SDA_pin);
        }

        output_high(SCK_pin); 
        output_low(SCK_pin);

        value <<= 1;
        s--;
    }
    
    output_high(CS_pin);
}  


void OLED_gotoxy(unsigned char x_pos, unsigned char y_pos)
{                                    
    OLED_write((Set_Page_Start_Address_CMD + y_pos), CMD);
    OLED_write(((x_pos & 0x0F) | Set_Lower_Column_Start_Address_CMD), CMD);
    OLED_write((((x_pos & 0xF0) >> 0x04) | Set_Higher_Column_Start_Address_CMD), CMD);
}


void OLED_fill(unsigned char bmp_data) 
{                                                     
    unsigned char x_pos = 0x00;
    unsigned char page = 0x00;

    for(page = y_min; page < y_max; page++)
    {
        OLED_write((Set_Page_Start_Address_CMD + page), CMD);
        OLED_write(Set_Lower_Column_Start_Address_CMD, CMD);
        OLED_write(Set_Higher_Column_Start_Address_CMD, CMD);

        for(x_pos = x_min; x_pos < x_max; x_pos++)
        {
            OLED_write(bmp_data, DAT);
        }                                                                                  
    }   
} 


void OLED_clear_screen()
{ 
    OLED_fill(0x00);
}


void OLED_clear_buffer()
{
     unsigned long s = 0x00;

     for(s = 0; s < buffer_size; s++)
     {
          buffer[s] = 0x00;
     }
}


void OLED_cursor(unsigned char x_pos, unsigned char y_pos)
{         
    unsigned char i = 0x00;

    if(y_pos != 0x00)
    {
        if(x_pos == 1)
        {
            OLED_gotoxy(0x00, (y_pos + 0x02));
        }
        else
        {
            OLED_gotoxy((0x50 + ((x_pos - 0x02) * 0x06)), (y_pos + 0x02));
        }

        for(i = 0; i < 6; i++)
        {
            OLED_write(0xFF, DAT);
        }
    }
}


void OLED_draw_bitmap(unsigned char xb, unsigned char yb, unsigned char xe, unsigned char ye, unsigned char bmp_img[])
{
    unsigned long s = 0x0000;
    unsigned char x_pos = 0x00;
    unsigned char y_pos = 0x00;

    for(y_pos = yb; y_pos <= ye; y_pos++)
    {
        OLED_gotoxy(xb, y_pos);
        for(x_pos = xb; x_pos < xe; x_pos++)
        {
            OLED_write(bmp_img[s++], DAT);
        }
    }
}
       
                    
void OLED_print_char(unsigned char x_pos, unsigned char y_pos, unsigned char ch)
{
    unsigned char chr = 0x00;
    unsigned char s = 0x00;

    chr = (ch - 32);

    if(x_pos > (x_max - 6))
    {
        x_pos = 0;
        y_pos++;
    }
    OLED_gotoxy(x_pos, y_pos);

    for(s = 0x00; s < 0x06; s++)
    {
        OLED_write(font_regular[chr][s], DAT);
    }
}
  

void OLED_print_string(unsigned char x_pos, unsigned char y_pos, unsigned char *ch)
{
    unsigned char chr = 0x00;
    unsigned char i = 0x00;
    unsigned char j = 0x00;

    while(ch[j] != '\0')
    {
        chr = (ch[j] - 32);

        if(x_pos > (x_max - 0x06))
        {
            x_pos = 0x00;
            y_pos++;
        }
        OLED_gotoxy(x_pos, y_pos);

        for(i = 0x00; i < 0x06; i++)
        {
            OLED_write(font_regular[chr][i], DAT);
        }

        j++;
        x_pos += 6;
     }
}    
                                             
                      
void OLED_print_chr(unsigned char x_pos, unsigned char y_pos, signed long value)
{                                             
    unsigned char ch = 0x00;

    if(value < 0x00)
    {
        OLED_print_char(x_pos, y_pos, '-');
        value = -value;
    }
    else
    {
        OLED_print_char(x_pos, y_pos,' ');
    }

     if((value > 99) && (value <= 999))
     {
         ch = (value / 100);
         OLED_print_char((x_pos + 6), y_pos , (48 + ch));
         ch = ((value % 100) / 10);
         OLED_print_char((x_pos + 12), y_pos , (48 + ch));
         ch = (value % 10);
         OLED_print_char((x_pos + 18), y_pos , (48 + ch));
     }
     else if((value > 9) && (value <= 99))
     {
         ch = ((value % 100) / 10);
         OLED_print_char((x_pos + 6), y_pos , (48 + ch));
         ch = (value % 10);
         OLED_print_char((x_pos + 12), y_pos , (48 + ch));
         OLED_print_char((x_pos + 18), y_pos , 32);
     }
     else if((value >= 0) && (value <= 9))
     {
         ch = (value % 10);
         OLED_print_char((x_pos + 6), y_pos , (48 + ch));
         OLED_print_char((x_pos + 12), y_pos , 32);
         OLED_print_char((x_pos + 18), y_pos , 32);
     }
}


void OLED_print_int(unsigned char x_pos, unsigned char y_pos, signed long long value)
{ 
    unsigned char ch = 0x00;

    if(value < 0)
    {
        OLED_print_char(x_pos, y_pos, '-');
        value = -value;
    }
    else
    {
        OLED_print_char(x_pos, y_pos,' ');
    }

    if(value > 9999)
    {
        ch = (value / 10000);
        OLED_print_char((x_pos + 6), y_pos , (48 + ch));

        ch = ((value % 10000)/ 1000);
        OLED_print_char((x_pos + 12), y_pos , (48 + ch));

        ch = ((value % 1000) / 100);
        OLED_print_char((x_pos + 18), y_pos , (48 + ch));

        ch = ((value % 100) / 10);
        OLED_print_char((x_pos + 24), y_pos , (48 + ch));

        ch = (value % 10);
        OLED_print_char((x_pos + 30), y_pos , (48 + ch));
    }

    else if((value > 999) && (value <= 9999))
    {
        ch = ((value % 10000)/ 1000);
        OLED_print_char((x_pos + 6), y_pos , (48 + ch));

        ch = ((value % 1000) / 100);
        OLED_print_char((x_pos + 12), y_pos , (48 + ch));

        ch = ((value % 100) / 10);
        OLED_print_char((x_pos + 18), y_pos , (48 + ch));

        ch = (value % 10);
        OLED_print_char((x_pos + 24), y_pos , (48 + ch));
        OLED_print_char((x_pos + 30), y_pos , 32);
    }
    else if((value > 99) && (value <= 999))
    {
        ch = ((value % 1000) / 100);
        OLED_print_char((x_pos + 6), y_pos , (48 + ch));

        ch = ((value % 100) / 10);
        OLED_print_char((x_pos + 12), y_pos , (48 + ch));

        ch = (value % 10);
        OLED_print_char((x_pos + 18), y_pos , (48 + ch));
        OLED_print_char((x_pos + 24), y_pos , 32);
        OLED_print_char((x_pos + 30), y_pos , 32);
    }
    else if((value > 9) && (value <= 99))
    {
        ch = ((value % 100) / 10);
        OLED_print_char((x_pos + 6), y_pos , (48 + ch));

        ch = (value % 10);
        OLED_print_char((x_pos + 12), y_pos , (48 + ch));
        
        OLED_print_char((x_pos + 18), y_pos , 32);
        OLED_print_char((x_pos + 24), y_pos , 32);
        OLED_print_char((x_pos + 30), y_pos , 32);
    }
    else
    {
        ch = (value % 10);
        OLED_print_char((x_pos + 6), y_pos , (48 + ch));
        OLED_print_char((x_pos + 12), y_pos , 32);
        OLED_print_char((x_pos + 18), y_pos , 32);
        OLED_print_char((x_pos + 24), y_pos , 32);
        OLED_print_char((x_pos + 30), y_pos , 32);
    }
}                                                      


void OLED_print_decimal(unsigned char x_pos, unsigned char y_pos, unsigned long value, unsigned char points)
{
    unsigned char ch = 0x00;

    OLED_print_char(x_pos, y_pos, '.');

    ch = (value / 1000);
    OLED_print_char((x_pos + 6), y_pos , (48 + ch));

    if(points > 1)
    {
        ch = ((value % 1000) / 100);
        OLED_print_char((x_pos + 12), y_pos , (48 + ch));


        if(points > 2)
        {
            ch = ((value % 100) / 10);
            OLED_print_char((x_pos + 18), y_pos , (48 + ch));

            if(points > 3)
            {
                ch = (value % 10);
                OLED_print_char((x_pos + 24), y_pos , (48 + ch));
            }
        }
    }
}
 
                             
void OLED_print_float(unsigned char x_pos, unsigned char y_pos, float value, unsigned char points) 
{
    signed long long tmp = 0x00;

    tmp = value;
    OLED_print_int(x_pos, y_pos, tmp);
    tmp = ((value - tmp) * 10000);
    
    if(tmp < 0)
    {
       tmp = -tmp;
    }

    if((value >= 9999) && (value < 99999))
    {
        OLED_print_decimal((x_pos + 36), y_pos, tmp, points);
    }
    else if((value >= 999) && (value < 9999))
    {
        OLED_print_decimal((x_pos + 30), y_pos, tmp, points);
    }
    else if((value >= 99) && (value < 999))
    {
        OLED_print_decimal((x_pos + 24), y_pos, tmp, points);
    }
    else if((value >= 9) && (value < 99))
    {
        OLED_print_decimal((x_pos + 18), y_pos, tmp, points);
    }
    else if(value < 9)
    {
        OLED_print_decimal((x_pos + 12), y_pos, tmp, points);
        if((value) < 0)
        {
            OLED_print_char(x_pos, y_pos, '-');
        }
        else
        {
            OLED_print_char(x_pos, y_pos, ' ');
        }
    }
}


void Draw_Pixel(unsigned char x_pos, unsigned char y_pos, short colour)
{
    unsigned char value = 0x00;
    unsigned char page = 0x00;
    unsigned char bit_pos = 0x00;
    
    page = (y_pos / y_max);
    bit_pos = (y_pos - (page * y_max));
    value = buffer[((page * x_max) + x_pos)];
    
    if((colour & 0x01) != 0)
    {
        value |= (1 << bit_pos);
    }
    else
    {
        value &= (~(1 << bit_pos));
    }
                                 
    buffer[((page * x_max) + x_pos)] = value;
    OLED_gotoxy(x_pos, page);
    OLED_write(value, DAT);
}


void Draw_Line(signed long x1, signed long y1, signed long x2, signed long y2, short colour)
{          
    signed long dx = 0x0000;
    signed long dy = 0x0000;
    signed long stepx = 0x0000;
    signed long stepy = 0x0000;
    signed long fraction = 0x0000;

    dy = (y2 - y1);
    dx = (x2 - x1); 
                              
    if (dy < 0)
    {
        dy = -dy;
        stepy--;
    }
    else
    {
        stepy = 1;
    }

    if (dx < 0)
    {
        dx = -dx;
        stepx--;
    }
    else
    {                             
        stepx = 1;
    }                       

    dx <<= 1;
    dy <<= 1;  

    Draw_Pixel(((unsigned char)x1), ((unsigned char)y1), colour); 
       
    if(dx > dy)
    {
        fraction = (dy - (dx >> 1));
        while (x1 != x2)                                      
        {
            if(fraction >= 0)
            {                                                                     
                y1 += stepy;
                fraction -= dx;
            }
            
            x1 += stepx;
            fraction += dy;

            Draw_Pixel(((unsigned char)x1), ((unsigned char)y1), colour); 
        }
    }
    else
    {
        fraction = (dx - (dy >> 1));
        while (y1 != y2)
        {
            if (fraction >= 0)
            {
                x1 += stepx;
                fraction -= dy;
            }
            
            y1 += stepy;
            fraction += dx;
            
            Draw_Pixel(((unsigned char)x1), ((unsigned char)y1), colour); 
        }
    }
}


void Draw_Rectangle(signed long x1, signed long y1, signed long x2, signed long y2, short fill, short colour, short type)
{
     unsigned char i = 0x00;
     unsigned char xmin = 0x00;
     unsigned char xmax = 0x00;
     unsigned char ymin = 0x00;
     unsigned char ymax = 0x00;

     if(fill == ON)
     {
        if(x1 < x2)
        {
           xmin = x1;
           xmax = x2;
        }
        else
        {
           xmin = x2;
           xmax = x1;
        }

        if(y1 < y2)
        {
           ymin = y1;
           ymax = y2;
        }
        else
        {
           ymin = y2;                                   
           ymax = y1;                                             
        }
        
        for(i = ymin; i <= ymax; ++i) 
        {  
            Draw_Line(xmin, i, xmax, i, colour);   
        }  
               
     }

     else
     {        
        Draw_Line(x1, y1, x2, y1, colour);  
        Draw_Line(x1, y2, x2, y2, colour);
        Draw_Line(x1, y1, x1, y2, colour);
        Draw_Line(x2, y1, x2, y2, colour);
     }
                                                                                                              
     if(type != SQUARE)                                  
     {
         Draw_Pixel(((unsigned char)x1), ((unsigned char)y1), ~colour);                         
         Draw_Pixel(((unsigned char)x1), ((unsigned char)y2), ~colour); 
         Draw_Pixel(((unsigned char)x2), ((unsigned char)y1), ~colour);                                             
         Draw_Pixel(((unsigned char)x2), ((unsigned char)y2), ~colour); 
     }
}                                                   
                                                             

void Draw_Circle(signed long xc, signed long yc, signed long radius, short fill, short colour) 
{
   signed long a = 0x0000;
   signed long b = 0x0000;
   signed long P = 0x0000;

   b = radius;
   P = (1 - b);

   do
   {
        if(fill == ON)
        {
           Draw_Line((xc - a), (yc + b), (xc + a), (yc + b), colour);
           Draw_Line((xc - a), (yc - b), (xc + a), (yc - b), colour);
           Draw_Line((xc - b), (yc + a), (xc + b), (yc + a), colour);
           Draw_Line((xc - b), (yc - a), (xc + b), (yc - a), colour);
        }
        else
        {
           Draw_Pixel((xc + a), (yc + b), colour);
           Draw_Pixel((xc + b), (yc + a), colour);
           Draw_Pixel((xc - a), (yc + b), colour);
           Draw_Pixel((xc - b), (yc + a), colour);
           Draw_Pixel((xc + b), (yc - a), colour);                
           Draw_Pixel((xc + a), (yc - b), colour);
           Draw_Pixel((xc - a), (yc - b), colour);
           Draw_Pixel((xc - b), (yc - a), colour);
        }

        if(P < 0)
        {
           P += (3 + (2 * a++));
        }
        else
        {
           P += (5 + (2 * ((a++) - (b--))));
        }
    }while(a <= b);
}
