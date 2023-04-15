#include "lcd.h"


void SIPO(void)
{
    unsigned char clk = 8;
    unsigned char temp = 0;
    
    temp = data_value;
    
    CSN_low;
    
    while(clk > 0)                                                                      
    {   
        if((temp & 0x80) != 0x00)
        {                
            SER_high;
        }
        
        else
        {
            SER_low;
        }     
        
        SCK_high;
        temp <<= 1;                                                                                        
        clk--;
        SCK_low;    
    }      
                                     
    CSN_high;
}
                              

void LCD_init(void)
{                                     
    unsigned char t = 0x0A;
    
    set_tris_C(0xF8);
    output_C(0xF8);   
    
    data_value = 0x08;
    SIPO();
    while(t > 0x00)
    {   
        delay_ms(dly);
        t--;    
    };      
    
    data_value = 0x30;
    SIPO();
                              
    data_value |= 0x08;
    SIPO();
    delay_ms(dly);
    data_value &= 0xF7;
    SIPO();
    delay_ms(dly); 
             
    data_value = 0x30;
    SIPO();
                    
    data_value |= 0x08;
    SIPO();
    delay_ms(dly);
    data_value &= 0xF7;
    SIPO();
    delay_ms(dly);

    data_value = 0x30;
    SIPO();

    data_value |= 0x08;
    SIPO();
    delay_ms(dly);
    data_value &= 0xF7;
    SIPO();
    delay_ms(dly);

    data_value = 0x20;
    SIPO();
                    
    data_value |= 0x08;
    SIPO();
    delay_ms(dly);
    data_value &= 0xF7;
    SIPO();
    delay_ms(dly);

    LCD_command(_4_pin_interface | _2_row_display | _5x7_dots);         
    LCD_command(display_on | cursor_off | blink_off);     
    LCD_command(clear_display);         
    LCD_command(cursor_direction_inc | display_no_shift);        
}   


void LCD_command(unsigned char value)
{                                   
    data_value &= 0xFB;
    SIPO();
    LCD_4bit_send(value);           
}
   

void LCD_send_data(unsigned char value)
{                               
    data_value |= 0x04;
    SIPO();
    LCD_4bit_send(value);
}  
    

void LCD_4bit_send(unsigned char lcd_data)       
{
    unsigned char temp = 0x00;
    
    temp = (lcd_data & 0xF0);
    data_value &= 0x0F;
    data_value |= temp;
    SIPO();
                                          
    data_value |= 0x08;
    SIPO();
    delay_ms(dly);
    data_value &= 0xF7;
    SIPO();
    delay_ms(dly);

    temp = (lcd_data & 0x0F);
    temp <<= 0x04;
    data_value &= 0x0F;
    data_value |= temp;
    SIPO();
               
    data_value |= 0x08;
    SIPO();
    delay_ms(dly);
    data_value &= 0xF7;
    SIPO();
    delay_ms(dly);
}  


void LCD_putstr(char *lcd_string)
{
    while (*lcd_string != '\0')   
    {                          
        LCD_send_data(*lcd_string++);
    };
}


void LCD_putchar(char char_data)
{
    LCD_send_data(char_data);
}


void LCD_clear_home()
{
    LCD_command(clear_display);
    LCD_command(goto_home);
}


void LCD_goto(unsigned char x_pos,unsigned char y_pos)
{                                                   
    if(y_pos == 0)    
    {                              
        LCD_command(0x80 | x_pos);
    }
    else 
    {                                              
        LCD_command(0x80 | 0x40 | x_pos); 
    }
}
