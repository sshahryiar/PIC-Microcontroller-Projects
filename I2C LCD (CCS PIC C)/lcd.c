#include "lcd.h" 
                              

void LCD_init()
{                                     
    unsigned char t = 0x1A;  

    data_value = 0x08;
    PCF8574_write(data_value);     
    while(t > 0x00)
    {   
            delay_ms(dly);
            t--;
    };      
    
    data_value = 0x30;
    PCF8574_write(data_value);  
                              
    data_value |= 0x08;
    PCF8574_write(data_value);     
    delay_ms(dly);
    data_value &= 0xF7;
    PCF8574_write(data_value);  
    delay_ms(dly); 
             
    data_value = 0x30;
    PCF8574_write(data_value);  
                    
    data_value |= 0x08;
    PCF8574_write(data_value);  
    delay_ms(dly);
    data_value &= 0xF7;
    PCF8574_write(data_value);  
    delay_ms(dly);

    data_value = 0x30;
    PCF8574_write(data_value);  

    data_value |= 0x08;
    PCF8574_write(data_value);  
    delay_ms(dly);
    data_value &= 0xF7;
    PCF8574_write(data_value);  
    delay_ms(dly);

    data_value = 0x20;
    PCF8574_write(data_value);  
                    
    data_value |= 0x08;
    PCF8574_write(data_value);  
    delay_ms(dly);
    data_value &= 0xF7;
    PCF8574_write(data_value);  
    delay_ms(dly);

    LCD_command(_4_pin_interface | _2_row_display | _5x7_dots);         
    LCD_command(display_on | cursor_off | blink_off);     
    LCD_command(clear_display);                           
    LCD_command(cursor_direction_inc | display_no_shift);        
}   


void LCD_command(unsigned char value)
{                                   
    data_value &= 0xFB;
    PCF8574_write(data_value);  
    LCD_4bit_send(value);           
}
   

void LCD_send_data(unsigned char value)
{                               
    data_value |= 0x04;
    PCF8574_write(data_value);  
    LCD_4bit_send(value);
}                                       
    

void LCD_4bit_send(unsigned char lcd_data)       
{
    unsigned char temp = 0x00;
      
    temp = (lcd_data & 0xF0);
    data_value &= 0x0F;   
    data_value |= temp;   
    PCF8574_write(data_value);  
                                          
    data_value |= 0x08;
    PCF8574_write(data_value);  
    delay_ms(dly); 
    data_value &= 0xF7;
    PCF8574_write(data_value);  
    delay_ms(dly);
    
    temp = (lcd_data & 0x0F);
    temp <<= 0x04;     
    data_value &= 0x0F;  
    data_value |= temp;                            
    PCF8574_write(data_value);  
               
    data_value |= 0x08;
    PCF8574_write(data_value);  
    delay_ms(dly);
    data_value &= 0xF7;
    PCF8574_write(data_value);  
    delay_ms(dly);
}  


void LCD_putstr(char *lcd_string)
{
    while (*lcd_string != '\0')   
    {
        LCD_send_data(*lcd_string);
        lcd_string++;
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
