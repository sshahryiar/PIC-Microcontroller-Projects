#include <18F242.h> 


#device *= 16 
#device ADC=10
   
                
#fuses HS, NOWDT, PUT, PROTECT, NOSTVREN
#fuses NOCPB, NOWRT, NOWRTB NOEBTRB, NOEBTR
#fuses BROWNOUT, CPD, NOOSCSEN, BORV20
#fuses NOLVP, NODEBUG, NOWRTC, NOWRTD
 
                             
#use delay(clock = 10MHz)             

                                                  
#byte lcdport         = 0x07   
#byte lcdport_tris    = 0x87         
                 
                                  
#include "3310_custom.c" 
#include "background_art.c"   


#define toggle             input(pin_A0)           
#define move_left          input(pin_A1) 
#define move_right         input(pin_A2) 
#define move_on            input(pin_A3)  

#define dly 			   40   


short dir = 1;
short tgl = 0;          
short start = 0;
signed char pos = 0;
unsigned char ps = 0;
unsigned char num = 0; 
unsigned char tmp = 0;  
unsigned char b[8];    
                              

void setup();
void draw_background();  
unsigned char get_input();
void make_number();
unsigned char pow(unsigned char val); 
void redraw_background();


void main()                                       
{                                    
   setup();
   draw_background();
   while(TRUE)       
   {
		make_number();           
   }
}   
                               
                                  
void setup() 
{
   disable_interrupts(global);
   setup_WDT(WDT_off);
   setup_adc(ADC_off);  
   setup_adc_ports(no_analogs);
   setup_timer_0(T0_OFF);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,255,1);
   setup_timer_3(T3_DISABLED);
   setup_CCP1(CCP_off);
   setup_CCP2(CCP_off);
   setup_SPI(SPI_DISABLED);                        
   lcdport_tris = 0x00;      
   nokia_init();
   set_timer0(0);                                                                                       
   set_timer1(0);
   set_timer2(0);
   set_timer3(0); 
   lcd_update();   
   memset(b, 0x00, sizeof(b)); 
   delay_ms(100);
}                                   
                                                      
                      
void draw_background()
{
   register unsigned long i = 0;
   nokia_clean_ddram();      
   nokia_clear_screen();
   nokia_gotoxy(0,0); 
   for(i = 0; i < 504; i += 1)
   {
        nokia_write_data(background[i]);
   }
   delay_ms(10);
}    


unsigned char get_input()
{
         if(toggle)                   
         {
		       delay_ms(dly);  
		       return 1;                    
         }
         else if(move_left)
         {
               delay_ms(dly);
               return 2;      
         }                        
         else if(move_right)
         {
               delay_ms(dly);
               return 3;          
         }
         else if(move_on)
         {
               delay_ms(dly);
               return 4;  
         }
         else
         {
             return 0;    
         }            
}


void make_number()
{                                                  
     nokia_gotoxy(15, 2);
     nokia_printchar("Bin");  
     nokia_gotoxy(15, 3);     
     printf(nokia_printchar,"HEX: %02x ", num);   
     nokia_gotoxy(15, 4);
     printf(nokia_printchar,"DEC: %03u ", num);    
     nokia_gotoxy(15, 5);
     printf(nokia_printchar,"CHR: %c ", num);      
                             
     if(get_input() == 1)  
     {
    	while(get_input() == 1); 
    	start = 1;
     }                                
     
     while(start)
     {
           if(get_input() == 2)
           {                      
        		pos += 1;
	            dir = 1;
           }
           if(get_input() == 3)
           {
                pos -= 1;
				dir = 0;       
           }                   
           if(pos > 7)          
           {
            	pos = 0;
           }
           if(pos < 0)
           {
        		pos = 7;
           }
           if(get_input() == 1)
           {                  
                 num = 0;  
                 b[pos] = (b[pos] & 0x01);
                 b[pos] ^= 1;    
                 drawRectangle(tmp, 10, (tmp + 1), 11, (b[pos])); 
                 for(ps = 0; ps < 9; ps += 1)
                 {             
                     
                     b[ps] = (b[ps] & 0x01);
                     num += (pow(ps) * (b[ps]));
                     tmp = (16 + (7 * (7 - ps)));   
                 }                        
                           
                 if(dir)
                 {                                        
                        pos += 1;
                 }                   
                 else                                      
                 {                             
                        pos -= 1;
                 }        
                 
                 nokia_gotoxy(15, 3);
				 printf(nokia_printchar,"HEX: %02x ", num);   
				 nokia_gotoxy(15, 4);                                                                 
				 printf(nokia_printchar,"DEC: %03u ", num);    
				 nokia_gotoxy(15, 5);
    				printf(nokia_printchar,"CHR: %c ", num);      
				 drawRectangle(tmp, 10, (tmp + 1), 11, (b[pos]));				 
           }                    
                                            
           tmp = (16 + (7 * (7 - pos)));           
           drawRectangle(tmp, 10, (tmp + 1), 11, tgl);    
           tgl = ~tgl;                           
    	   delay_ms(100);  
    	   drawRectangle(tmp, 10, (tmp + 1), 11, (b[pos]));
    	   redraw_background();
                                                   
           if(get_input() == 4)              
           {                                           
	           redraw_background();
	           start = 0;     
           }  
     }        
}
                                            
                                               
unsigned char pow(unsigned char val)
{                              
     unsigned char cnt = 0;                        
     unsigned long temp = 1;
     for(cnt = 0; cnt < val; cnt += 1)
     {
         temp <<= 1;
     }
     return temp;   
}       


void redraw_background()
{
	unsigned char l = 0; 
	drawLine(13, 8, 69, 8, 1);
    drawLine(13, 13, 69, 13, 1); 
    drawRectangle(9, 10, 10, 11, 0); 
    for(l = 13; l < 70; l += 7)      
	{
		drawLine(l, 8, l, 13, 1); 
	}  
} 

