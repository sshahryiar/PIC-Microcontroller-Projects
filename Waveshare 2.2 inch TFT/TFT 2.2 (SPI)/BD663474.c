#include "BD663474.h"
#include "font.c"


void TFT_IO_init()
{
    set_TRIS_A(0xE9);
    set_TRIS_C(0xD7);
    set_TRIS_D(0xFE);
    output_high(TFT_CS);
    setup_SPI(SPI_MASTER | SPI_XMIT_L_TO_H | SPI_SCK_IDLE_HIGH | SPI_SAMPLE_AT_MIDDLE);
}


void TFT_reset() 
{
   output_low(TFT_RST);
   delay_us(3);
   output_high(TFT_RST);
   delay_us(3); 
}


void TFT_write_word(unsigned long value)
{   
   SPI_write(value >> 8);
   SPI_write(value);
}


void TFT_write(unsigned long value, short mode)
{ 
   output_bit(TFT_RS, mode);
   output_low(TFT_CS);
   TFT_write_word(value);
   output_high(TFT_CS);
}


void TFT_write_RAM_prepare()
{
   TFT_write(0x0202, CMD);
}


void TFT_write_reg(unsigned long cmd_value, unsigned long data_value)
{
   TFT_write(cmd_value, CMD);
   TFT_write(data_value, DAT);
}

void TFT_init()
{
   TFT_IO_init();
   TFT_reset();

   TFT_write_reg(0x000, 0x0001); 
   delay_ms(10);
   
   TFT_write_reg(0x100, 0x0000);    
   TFT_write_reg(0x101, 0x0000); 
   TFT_write_reg(0x102, 0x3110); 
   TFT_write_reg(0x103, 0xE200); 
   TFT_write_reg(0x110, 0x009D); 
   TFT_write_reg(0x111, 0x0022); 
   TFT_write_reg(0x100, 0x0120); 
   delay_ms(20);

   TFT_write_reg(0x100, 0x3120);
   delay_ms(80);
   
   TFT_write_reg(0x001, 0x0100);
   TFT_write_reg(0x002, 0x0000);
   TFT_write_reg(0x003, 0x1238);//
   TFT_write_reg(0x006, 0x0000);
   TFT_write_reg(0x007, 0x0101);
   TFT_write_reg(0x008, 0x0808);
   TFT_write_reg(0x009, 0x0000);
   TFT_write_reg(0x00B, 0x0000);
   TFT_write_reg(0x00C, 0x0000);
   TFT_write_reg(0x00D, 0x0018);
 
   TFT_write_reg(0x012, 0x0000);
   TFT_write_reg(0x013, 0x0000);
   TFT_write_reg(0x018, 0x0000);
   TFT_write_reg(0x019, 0x0000);

   TFT_write_reg(0x203, 0x0000);
   TFT_write_reg(0x204, 0x0000);

   TFT_write_reg(0x210, 0x0000);
   TFT_write_reg(0x211, 0x00EF);
   TFT_write_reg(0x212, 0x0000);
   TFT_write_reg(0x213, 0x013F);
   TFT_write_reg(0x214, 0x0000);
   TFT_write_reg(0x215, 0x0000);
   TFT_write_reg(0x216, 0x0000);
   TFT_write_reg(0x217, 0x0000);

   TFT_write_reg(0x300, 0x5343);
   TFT_write_reg(0x301, 0x1021);
   TFT_write_reg(0x302, 0x0003);
   TFT_write_reg(0x303, 0x0011);
   TFT_write_reg(0x304, 0x050A);
   TFT_write_reg(0x305, 0x4342);
   TFT_write_reg(0x306, 0x1100);
   TFT_write_reg(0x307, 0x0003);
   TFT_write_reg(0x308, 0x1201);
   TFT_write_reg(0x309, 0x050A);

   TFT_write_reg(0x400, 0x4027);
   TFT_write_reg(0x401, 0x0000);
   TFT_write_reg(0x402, 0x0000);      
   TFT_write_reg(0x403, 0x013F);        
   TFT_write_reg(0x404, 0x0000);

   TFT_write_reg(0x200, 0x0000);
   TFT_write_reg(0x201, 0x0000);
   
   TFT_write_reg(0x100, 0x7120);
   TFT_write_reg(0x007, 0x0103);
   delay_ms(10);
   TFT_write_reg(0x007, 0x0113);
}


void TFT_set_display_address_window(signed long xs, signed long ys, signed long xe, signed long ye)
{
   TFT_write_reg(0x210, xs);
   TFT_write_reg(0x211, xe);
   TFT_write_reg(0x212, ys);
   TFT_write_reg(0x213, ye);
}


void TFT_set_cursor(signed long x_pos, signed long y_pos)
{
   TFT_write_reg(0x200, x_pos);
   TFT_write_reg(0x201, y_pos);
   
   TFT_write_RAM_prepare();
}


void TFT_fill(unsigned long colour)
{
   unsigned long long index = (MAX_X * MAX_Y);

   TFT_set_display_address_window(0, 0, 239, 319);
   TFT_set_cursor(0, 0);
   
   while(index)
   {
      TFT_write(colour, DAT);
      index--;
   };
}


unsigned long TFT_BGR2RGB(unsigned long colour)
{
   unsigned long r = 0x0000;
   unsigned long g = 0x0000;
   unsigned long b = 0x0000;
   unsigned long rgb_colour = 0x0000;
   
   b = ((colour >> 0)  & 0x1F);
   g = ((colour >> 5)  & 0x3F);
   r = ((colour >> 11) & 0x1F);
   
   rgb_colour = ((b << 11) + (g << 5) + r);
   
   return rgb_colour;
}


unsigned long RGB565_converter(unsigned char r, unsigned char g, unsigned char b)
{
   return (((((unsigned long)r) >> 3) << 11) | ((((unsigned long)g) >> 2) << 5) | (((unsigned long)b) >> 3));
}


void swap_values(signed long *a, signed long *b)
{
    signed long temp = 0x0000;

    temp = *b;
    *b = *a;
    *a = temp;
}


void Draw_Pixel(signed long x_pos, signed long y_pos, unsigned long colour)
{
    unsigned char i = 2;
    
    if((x_pos >= MAX_X) || (y_pos >= MAX_Y) || (x_pos < 0) || (y_pos < 0))
    {
        return;
    }
    
    TFT_set_display_address_window(x_pos, y_pos, (x_pos + 1), (y_pos + 1));
    TFT_set_cursor(x_pos, y_pos);
    while(i)
    {
        TFT_write(colour, DAT);
        i--;
    }
}


void Draw_Line(signed long x1, signed long y1, signed long x2, signed long y2, unsigned long colour)
{
    signed long dx = 0x0000;
    signed long dy = 0x0000;
    signed long stepx = 0x0000;
    signed long stepy = 0x0000;
    signed long fraction = 0x0000;

    dy = (y2 - y1);
    dx = (x2 - x1);

    if(dy < 0)
    {
        dy = -dy;
        stepy = -1;
    }
    else
    {
        stepy = 1;
    }

    if(dx < 0)
    {
        dx = -dx;
        stepx = -1;
    }
    else
    {
        stepx = 1;
    }

    dx <<= 0x01;
    dy <<= 0x01;

    Draw_Pixel(x1, y1, colour);

    if(dx > dy)
    {
        fraction = (dy - (dx >> 1));
        while(x1 != x2)
        {
            if(fraction >= 0)
            {
                y1 += stepy;
                fraction -= dx;
            }
            x1 += stepx;
            fraction += dy;

            Draw_Pixel(x1, y1, colour);
        }
    }
    else
    {
        fraction = (dx - (dy >> 1));

        while(y1 != y2)
        {
            if (fraction >= 0)
            {
                x1 += stepx;
                fraction -= dy;
            }
            y1 += stepy;
            fraction += dx;
            Draw_Pixel(x1, y1, colour);
        }
    }
}


void Draw_V_Line(signed long x1, signed long y1, signed long y2, unsigned colour)
{   
    if(y1 > y2)
    {
       swap_values(&y1, &y2);
    }
    
    while(y2 > (y1 - 1))
    {
        Draw_Pixel(x1, y2, colour);
        y2--;
    }
}


void Draw_H_Line(signed long x1, signed long x2, signed long y1, unsigned colour)
{
    if(x1 > x2)
    {
       swap_values(&x1, &x2);
    }

    while(x2 > (x1 - 1))
    {
        Draw_Pixel(x2, y1, colour);
        x2--;
    }
}


void Draw_Triangle(signed long x1, signed long y1, signed long x2, signed long y2, signed long x3, signed long y3, short fill, unsigned long colour)
{
    signed long a = 0;
    signed long b = 0;
    signed long sa = 0;
    signed long sb = 0;
    signed long yp = 0;
    signed long last = 0;
    signed long dx12 = 0;
    signed long dx23 = 0;
    signed long dx13 = 0;
    signed long dy12 = 0;
    signed long dy23 = 0;
    signed long dy13 = 0;

    switch(fill)
    {
        case YES:
        {
            if(y1 > y2)
            {
                swap_values(&y1, &y2);
                swap_values(&x1, &x2);
            }
            if(y2 > y3)
            {
                swap_values(&y3, &y2);
                swap_values(&x3, &x2);
            }
            if(y1 > y2)
            {
                swap_values(&y1, &y2);
                swap_values(&x1, &x2);
            }

            if(y1 == y3)
            {
                a = b = x1;

                if(x2 < a)
                {
                    a = x2;
                }
                else if(x2 > b)
                {
                    b = x2;
                }
                if(x2 < a)
                {
                    a = x3;
                }
                else if(x3 > b)
                {
                    b = x3;
                }

                Draw_H_Line(a, (a + (b - (a + 1))), y1, colour);
                return;
            }

            dx12 = (x2 - x1);
            dy12 = (y2 - y1);
            dx13 = (x3 - x1);
            dy13 = (y3 - y1);
            dx23 = (x3 - x2);
            dy23 = (y3 - y2);
            sa = 0,
            sb = 0;

            if(y2 == y3)
            {
                last = y2;
            }
            else
            {
                last = (y2 - 1);
            }

            for(yp = y1; yp <= last; yp++)
            {
                a = (x1 + (sa / dy12));
                b = (x1 + (sb / dy13));
                sa += dx12;
                sb += dx13;
                if(a > b)
                {
                    swap_values(&a, &b);
                }
                Draw_H_Line(a, (a + (b - (a + 1))), yp, colour);
            }

            sa = (dx23 * (yp - y2));
            sb = (dx13 * (yp - y1));
            for(; yp <= y3; yp++)
            {
                a = (x2 + (sa / dy23));
                b = (x1 + (sb / dy13));
                sa += dx23;
                sb += dx13;

                if(a > b)
                {
                    swap_values(&a, &b);
                }
                Draw_H_Line(a, (a + (b - (a + 1))), yp, colour);
            }
            
            
            break;
        }
        default:
        {
            Draw_Line(x1, y1, x2, y2, colour);
            Draw_Line(x2, y2, x3, y3, colour);
            Draw_Line(x3, y3, x1, y1, colour);
            break;
        }
    }
}


void Draw_Rectangle(signed long x1, signed long y1, signed long x2, signed long y2, short fill, unsigned char type, unsigned long colour, unsigned long back_colour)
{
     signed long i = 0x0000;
     signed long xmin = 0x0000;
     signed long xmax = 0x0000;
     signed long ymin = 0x0000;
     signed long ymax = 0x0000;
     
     switch(fill)
     {
         case YES:
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

             for(; xmin <= xmax; ++xmin)
             {
                 for(i = ymin; i <= ymax; ++i)
                 {
                     Draw_Pixel(xmin, i, colour);
                 }
             }
             
             break;
         }
         default:
         {
             Draw_V_Line(x1, y1, y2, colour);
             Draw_V_Line(x2, y1, y2, colour);
             Draw_H_Line(x1, x2, y1, colour);
             Draw_H_Line(x1, x2, y2, colour);
             break;
         }
     }

     if(type != SQUARE)
     {
         Draw_Pixel(x1, y1, back_colour);
         Draw_Pixel(x1, y2, back_colour);
         Draw_Pixel(x2, y1, back_colour);
         Draw_Pixel(x2, y2, back_colour);
     }
}


void Draw_H_Bar(signed long x1, signed long x2, signed long y1, signed long bar_width, signed long bar_value, unsigned long border_colour, unsigned long bar_colour, unsigned long back_colour, unsigned char border)
{
    switch(border)
    {
        case YES:
        {
            Draw_Rectangle((x1 + 1), (y1 + 1), (x1 + bar_value), (y1 + bar_width - 1), YES, SQUARE, bar_colour, back_colour);
            Draw_Rectangle((x2 - 1), (y1 + 1), (x1 + bar_value + 1), (y1 + bar_width - 1), YES, SQUARE, back_colour, back_colour);
            Draw_Rectangle(x1, y1, x2, (y1 + bar_width), NO, SQUARE, border_colour, back_colour);
            break;
        }
        default:
        {
            Draw_Rectangle(x1, y1, (x1 + bar_value), (y1 + bar_width), YES, SQUARE, bar_colour, back_colour);
            Draw_Rectangle(x2, y1, (x1 + bar_value), (y1 + bar_width), YES, SQUARE, back_colour, back_colour);
            break;
        }
    }
}


void Draw_V_Bar(signed long x1, signed long y1, signed long y2, signed long bar_width, signed long bar_value, unsigned long border_colour, unsigned long bar_colour, unsigned long back_colour, unsigned char border)
{
    switch(border)
    {
        case YES:
        {
            Draw_Rectangle((x1 + 1), (y2 - 1), (x1 + bar_width - 1), (y2 - bar_value), YES, SQUARE, bar_colour, back_colour);
            Draw_Rectangle((x1 + 1), (y2 - bar_value - 1), (x1 + bar_width - 1), (y1 + 1), YES, SQUARE, back_colour, back_colour);
            Draw_Rectangle(x1, y1, (x1 + bar_width), y2, NO, SQUARE, border_colour, back_colour);
            break;
        }
        default:
        {
            Draw_Rectangle(x1, y2, (x1 + bar_width), (y2 - bar_value), YES, SQUARE, bar_colour, back_colour);
            Draw_Rectangle(x1, (y2 - bar_value), (x1 + bar_width), y1, YES, SQUARE, back_colour, back_colour);
            break;
        }
    }
}


void Draw_Circle(signed long xc, signed long yc, signed long radius, short fill, unsigned long colour)
{
     signed long a = 0x0000;
     signed long b = 0x0000;
     signed long p = 0x0000;

     b = radius;
     p = (1 - b);

     do
     {
          switch(fill)
          {
              case YES:
              {
                  Draw_Line((xc - a), (yc + b), (xc + a), (yc + b), colour);
                  Draw_Line((xc - a), (yc - b), (xc + a), (yc - b), colour);
                  Draw_Line((xc - b), (yc + a), (xc + b), (yc + a), colour);
                  Draw_Line((xc - b), (yc - a), (xc + b), (yc - a), colour);
                  break;
              }
              default:
              {
                  Draw_Pixel((xc + a), (yc + b), colour);
                  Draw_Pixel((xc + b), (yc + a), colour);
                  Draw_Pixel((xc - a), (yc + b), colour);
                  Draw_Pixel((xc - b), (yc + a), colour);
                  Draw_Pixel((xc + b), (yc - a), colour);
                  Draw_Pixel((xc + a), (yc - b), colour);
                  Draw_Pixel((xc - a), (yc - b), colour);
                  Draw_Pixel((xc - b), (yc - a), colour);
                  break;
              }
          }

          if(p < 0)
          {
              p += (0x03 + (0x02 * a++));
          }
          else
          {
              p += (0x05 + (0x02 * ((a++) - (b--))));
          }
    }while(a <= b);
}


void Draw_Font_Pixel(unsigned long x_pos, unsigned long y_pos, unsigned long colour, unsigned char pixel_size)
{
     unsigned long i = 0x0000;

     TFT_set_cursor(x_pos, y_pos);
     
     for(i = 0x0000; i < (pixel_size * pixel_size); i++)
     {
         TFT_write(colour, DAT);
     }
}


void print_char(unsigned long x_pos, unsigned long y_pos, unsigned char font_size, unsigned long colour, unsigned long back_colour, char ch)
{
     unsigned char i = 0x00;
     unsigned char j = 0x00;

     unsigned long value = 0x0000;

     if(font_size <= 0)
     {
         font_size = 1;
     }

     if(x_pos < font_size)
     {
         x_pos = font_size;
     }

     for(i = 0x00; i < 0x05; i++)
     {
         for(j = 0x00; j < 0x08; j++)
         {
             value = 0x0000;
             value = ((font[((unsigned char)ch) - 0x20][i]));

             if(((value >> j) & 0x01) != 0x00)
             {
                 Draw_Font_Pixel(x_pos, y_pos, colour, font_size);
             }
             else
             {
                 Draw_Font_Pixel(x_pos, y_pos, back_colour, font_size);
             }

             y_pos += font_size;
          }
          y_pos -= (font_size << 0x03);
          x_pos++;
      }
}


void print_str(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned long colour, unsigned long back_colour, char *ch)
{
     do
     {
         print_char(x_pos, y_pos, font_size, colour, back_colour, *ch++);
         x_pos += 0x06;
     }while((*ch >= 0x20) && (*ch <= 0x7F));
     print_char(x_pos, y_pos, font_size, colour, back_colour, 0x20);
}


void print_C(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned long colour, unsigned long back_colour, signed long value)
{
     unsigned char ch[6] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20};

     if(value < 0x00)
     {
        ch[0] = 0x2D;
        value = -value;
     }
     else
     {
        ch[0] = 0x20;
     }

     if((value > 99) && (value <= 999))
     {
         ch[1] = ((value / 100) + 0x30);
         ch[2] = (((value % 100) / 10) + 0x30);
         ch[3] = ((value % 10) + 0x30);
     }
     else if((value > 9) && (value <= 99))
     {
         ch[1] = (((value % 100) / 10) + 0x30);
         ch[2] = ((value % 10) + 0x30);
         ch[3] = 0x20;
     }
     else if((value >= 0) && (value <= 9))
     {
         ch[1] = ((value % 10) + 0x30);
         ch[2] = 0x20;
         ch[3] = 0x20;
     }

     print_str(x_pos, y_pos, font_size, colour, back_colour, ch);
}


void print_I(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned long colour, unsigned long back_colour, signed long value)
{
    unsigned char ch[6] = {0x20, 0x20, 0x20, 0x20, 0x20, 0x20};

    if(value < 0)
    {
        ch[0] = 0x2D;
        value = -value;
    }
    else
    {
        ch[0] = 0x20;
    }

    if(value > 9999)
    {
        ch[1] = ((value / 10000) + 0x30);
        ch[2] = (((value % 10000)/ 1000) + 0x30);
        ch[3] = (((value % 1000) / 100) + 0x30);
        ch[4] = (((value % 100) / 10) + 0x30);
        ch[5] = ((value % 10) + 0x30);
    }

    else if((value > 999) && (value <= 9999))
    {
        ch[1] = (((value % 10000)/ 1000) + 0x30);
        ch[2] = (((value % 1000) / 100) + 0x30);
        ch[3] = (((value % 100) / 10) + 0x30);
        ch[4] = ((value % 10) + 0x30);
        ch[5] = 0x20;
    }
    else if((value > 99) && (value <= 999))
    {
        ch[1] = (((value % 1000) / 100) + 0x30);
        ch[2] = (((value % 100) / 10) + 0x30);
        ch[3] = ((value % 10) + 0x30);
        ch[4] = 0x20;
        ch[5] = 0x20;
    }
    else if((value > 9) && (value <= 99))
    {
        ch[1] = (((value % 100) / 10) + 0x30);
        ch[2] = ((value % 10) + 0x30);
        ch[3] = 0x20;
        ch[4] = 0x20;
        ch[5] = 0x20;
    }
    else
    {
        ch[1] = ((value % 10) + 0x30);
        ch[2] = 0x20;
        ch[3] = 0x20;
        ch[4] = 0x20;
        ch[5] = 0x20;
    }

    print_str(x_pos, y_pos, font_size, colour, back_colour, ch);
}


void print_D(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned long colour, unsigned long back_colour, unsigned long value, unsigned char points)
{
    unsigned char ch[6] = {0x2E, 0x20, 0x20, 0x20, 0x20, 0x20};

    ch[1] = ((value / 1000) + 0x30);

    if(points > 1)
    {
        ch[2] = (((value % 1000) / 100) + 0x30);

        if(points > 2)
        {
            ch[3] = (((value % 100) / 10) + 0x30);

            if(points > 3)
            {
                ch[4] = ((value % 10) + 0x30);
            }
        }
    }

    print_str(x_pos, y_pos, font_size, colour, back_colour, ch);
}


void print_F(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned long colour, unsigned long back_colour, float value, unsigned char points)
{
    signed long tmp = 0x0000;

    tmp = value;
    print_I(x_pos, y_pos, font_size, colour, back_colour, tmp);
    tmp = ((value - tmp) * 10000);

    if(tmp < 0)
    {
       tmp = -tmp;
    }

    if((value >= 10000) && (value < 100000))
    {
        print_D((x_pos + 36), y_pos, font_size, colour, back_colour, tmp, points);
    }
    else if((value >= 1000) && (value < 10000))
    {
        print_D((x_pos + 30), y_pos, font_size, colour, back_colour, tmp, points);
    }
    else if((value >= 100) && (value < 1000))
    {
        print_D((x_pos + 24), y_pos, font_size, colour, back_colour, tmp, points);
    }
    else if((value >= 10) && (value < 100))
    {
        print_D((x_pos + 18), y_pos, font_size, colour, back_colour, tmp, points);
    }
    else if(value < 10)
    {
        print_D((x_pos + 12), y_pos, font_size, colour, back_colour, tmp, points);

        if((value) < 0)
        {
            print_char(x_pos, y_pos, font_size, colour, back_colour, 0x2D);
        }
        else
        {
            print_char(x_pos, y_pos, font_size, colour, back_colour, 0x20);
        }
    }
}

void Draw_BMP(unsigned long x_pos, unsigned long y_pos, unsigned long H, unsigned long W, unsigned char *bitmap)
{
     unsigned long index = 0;
     unsigned long size = (W * H);
     unsigned long *bitmap_ptr = (unsigned long*)bitmap;

     TFT_set_display_address_window(x_pos, y_pos, (W - 1), (H - 1));
     TFT_write_RAM_prepare();

     for(index = 0; index < size; index++)
     {
         TFT_write(*bitmap_ptr++, DAT);
     }

     TFT_set_display_address_window(0, 0, 239, 319);
}


