#include <16F628A.h>


#device *= 16
                      

#fuses INTRC, PROTECT, NOMCLR, BROWNOUT, NOLVP, CPD, PUT, NOWDT           
                            

#use delay (internal = 4MHz)                                                     
#use I2C(MASTER, SDA = PIN_A0, SCL = PIN_A1)   
                      

#include <lcd420.c>            


const unsigned char symbol[8] = {0x0E, 0x0A, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00};


void setup();
void lcd_symbols();
float read_thermo(unsigned char address);
                                             
                              
void main()
{                                                
   float tempc = 0.0;                     
   float tempa = 0.0;  
                                          
   setup();        
   
   while (TRUE)                              
   {                       
      tempa = read_thermo(0x06);   
      tempc = read_thermo(0x07);  
      
      lcd_gotoxy(8, 1);                        
      printf(lcd_putc, "%3.2g  ",tempc);   
      lcd_gotoxy(8, 2);                        
      printf(lcd_putc, "%3.2g  ",tempa); 
      
      delay_ms(490);      
   }                                                 
} 
                                                                        

void setup()
{                                        
    disable_interrupts(global); 
    setup_oscillator(OSC_4MHz);
    setup_comparator(NC_NC_NC_NC);
    setup_timer_0(RTCC_internal);
    setup_timer_1(T1_disabled); 
    setup_timer_2(T2_disabled, 255, 1); 
    set_RTCC(0);  
    set_timer1(0);  
    set_timer2(0);            
    setup_CCP1(CCP_off);
    lcd_init();           
    lcd_putc("\f");
    lcd_symbols();            
    lcd_gotoxy(1, 1);
    lcd_putc("To/");  
    lcd_gotoxy(1, 2); 
    lcd_putc("Ta/"); 
    lcd_gotoxy(4, 1);
    lcd_putc(0);  
    lcd_gotoxy(4, 2);              
    lcd_putc(0);         
    lcd_gotoxy(5, 1);                                                                       
    lcd_putc("C:");  
    lcd_gotoxy(5, 2);  
    lcd_putc("C:"); 
}   


void lcd_symbols() 
{
   unsigned char s = 0;  
   
   lcd_send_byte(0, 0x40); 
   while(s < 8)
   {
        lcd_send_byte(1,symbol[s]); 
        s++;               
   }
   lcd_send_byte(0, 0x80);
}


float read_thermo(unsigned char address)
{     
	  unsigned char CRC = 0;     
	  unsigned char tmp1 = 0;
	  unsigned long tmp2 = 0;             
	  unsigned long long tempk = 0; 
	  float t = 0;
	  
	  i2c_start();
	  if(i2c_write(0xB4) == 0)
      {                                      
          i2c_write(address);                                                                       
          i2c_start();        
          i2c_write(0xB5);    
          
          tmp1 = i2c_read(1);                           
          tmp2 = i2c_read(1);      
          CRC = i2c_read(0); 
                                                           
      }                             
      i2c_stop(); 
      
      tempk = ((tmp2 << 8) + tmp1);  
                                     
      if(tempk <= 32650)                                                              
	  {        
    	t = ((tempk * 0.02) - 273.0);
	  }  
	  
	  return t;
}         
