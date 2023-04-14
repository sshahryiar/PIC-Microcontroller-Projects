#define data    		PIN_B4
#define clock   		PIN_B5
#define strobe  		PIN_B6   

#define LCD_DB4 			 5 
#define LCD_DB5 			 6 
#define LCD_DB6 			 7         
#define LCD_DB7 			 8         
                            
#define LCD_E   			 4
#define LCD_RS  			 3         

#define lcd_type			 2         
#define lcd_line_two	  0x40      

                                                                           
byte d0 = 0;
byte d1 = 0;
byte d2 = 0;
byte d3 = 0;
byte d4 = 0;
byte d5 = 0;
byte d6 = 0;
byte d7 = 0;
  
  
void set_shift(unsigned char value)
{
	unsigned char location = 0;
	unsigned char n = 0;
	location = 128;
	for(n = 1; n <= 8; n++)
	{    
	    if((value & location)>0) 
	    {
	    	output_bit(data,1); 
	    }
	    else 
	    {
	    	output_bit(data, 0);
	    }
	    output_bit(clock,1);
	    location >>= 1;
	    output_bit(clock,0);
	}
	output_bit(strobe, 1);
	output_bit(strobe, 0); 
}
                 

void f_output(byte value)
{
   set_shift(value);
}

void fatport()
{
	unsigned char value = 0;
	if(d0==1) 
	{        
		value += 1;
	}
	if(d1 == 1) 
	{
		value += 2;
	}           
	if(d2 == 1) 
	{
		value += 4;
	}    
	if(d3 == 1) 
	{
		value += 8;
	}
	if(d4 == 1) 
	{
		value += 16;
	}
	if(d5 == 1) 
	{
		value += 32;
	}    
	if(d6 == 1) 
	{
		value += 64;
	}           
	if(d7 == 1) 
	{
		value += 128;
	}
	f_output(value);  
}


void dg0(unsigned char value)
{
	d0 = value;
	fatport();  
}

void dg1(unsigned char value)
{
	d1 = value;
	fatport();
}

void dg2(unsigned char value)
{
	d2 = value;
	fatport();
}

void dg3(unsigned char value)
{
	d3 = value;
	fatport();
}

void dg4(unsigned char value)
{
	d4 = value;
	fatport(); 
}
         
void dg5(unsigned char value)
{
	d5 = value;
	fatport();
}

void dg6(unsigned char value)
{
	d6 = value;
	fatport();
}

void dg7(unsigned char value)
{
	d7 = value;
	fatport();
}

void f_output_bit(byte pin, value)
{
   switch(pin)
   {
      case 1:
      {
    		dg0(value);
            break;
      }
      case 2:
      {
            dg1(value);
            break;
      }
      case 3:
      {
    		dg2(value);
            break;
      }
      case 4:
      {
            dg3(value);
            break; 
      }
      case 5:
      {
    		dg4(value);
            break;
      }
      case 6:
      {
    		dg5(value);
            break;
      }
      case 7:
      {
    		dg6(value);
            break;
      }
      case 8:
      {
    		dg7(value);
            break; 
      }
   } 
}


void f_output_high(byte pin)
{
   switch(pin)
   {
      case 1:
      {
		dg0(1);
            break;
      }
      case 2:
      {
            dg1(1);
            break;
      }          
      case 3:
      {
		dg2(1);
            break;
      }
      case 4:
      {
            dg3(1);
            break; 
      }
      case 5:
      {
		dg4(1);
            break;
      }
      case 6:
      {
		dg5(1);
            break;
      }
      case 7:
      {
		dg6(1);
            break;
      }
      case 8:
      {
		dg7(1);
            break; 
      }
   }  
}


void f_output_low(byte pin)
{
   switch(pin)
   {
      case 1:
      {
		dg0(0);
            break;
      }
      case 2:
      {
            dg1(0);
            break;
      }
      case 3:
      {
		dg2(0);
            break;
      }
      case 4:
      {
            dg3(0);
            break; 
      }
      case 5:
      {
		dg4(0);
            break;
      }
      case 6:
      {
		dg5(0);
            break;
      }
      case 7:
      {
		dg6(0);
            break;
      }
      case 8:
      {
		dg7(0);
            break; 
      }
   }
}


const unsigned char LCD_INIT_STRING[4] = 
{                                        
	 0x20 | (lcd_type << 2),                  
	 0xc,                    
	 1,                           
	 6                       
}; 
                              

//------------------------------------- 
void lcd_send_nibble(int8 nibble) 
{ 
	 f_output_bit(LCD_DB4, !!(nibble & 1)); 
	 f_output_bit(LCD_DB5, !!(nibble & 2));  
	 f_output_bit(LCD_DB6, !!(nibble & 4));    
	 f_output_bit(LCD_DB7, !!(nibble & 8));    
	 delay_cycles(1); 
	 f_output_high(LCD_E); 
	 delay_us(2); 
	 f_output_low(LCD_E);   
} 


//---------------------------------------- 
// Send a byte to the LCD. 
void lcd_send_byte(unsigned char address, unsigned char n) 
{ 
	f_output_low(LCD_RS); 
	delay_us(60);  
	
	
	if(address)
	{
	   f_output_high(LCD_RS); 
	}	
	else
	{
	   f_output_low(LCD_RS); 
	}     
	delay_cycles(1); 
	
	f_output_low(LCD_E);	                           
	lcd_send_nibble(n >> 4);
	lcd_send_nibble(n & 0x0F);   
} 


void lcd_init() 
{ 
	unsigned char i = 0; 
	
	f_output_low(LCD_RS); 
	f_output_low(LCD_E); 
	delay_ms(15); 
	
	for(i = 0 ;i < 3; i++) 
	{ 
	    lcd_send_nibble(0x03); 
	    delay_ms(5); 
	} 
	
	lcd_send_nibble(0x02); 
	
	for(i=0; i < sizeof(LCD_INIT_STRING); i++) 
	{   
		lcd_send_byte(0, LCD_INIT_STRING[i]); 
	}     
	       
}         


void lcd_gotoxy(unsigned char x, unsigned char y) 
{ 
	unsigned char address = 0; 
	
	if(y != 1) 
	{
	   address = lcd_line_two; 
	}
	else 
	{
	   address=0; 
	}    
	address += x - 1; 
	lcd_send_byte(0, 0x80 | address);   
} 


void lcd_putc(char c) 
{ 
   switch(c) 
   { 
	    case '\f': 
	    {
	      lcd_send_byte(0, 1); 
	      delay_ms(2); 
	      break; 
	    }    
	    case '\n':
	    {
	       lcd_gotoxy(1, 2); 
	       break; 
	    }          
	    case '\b':
	    {                 
	       lcd_send_byte(0, 0x10); 
	       break; 
	    }    
	    default: 
	    {
	       lcd_send_byte(1, c); 
	       break; 
	    } 
   } 
}                      


