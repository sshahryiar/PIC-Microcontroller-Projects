#include <18F242.h> 


#device *= 16 
#device ADC = 10
   
                
#fuses HS, NOWDT, PUT, PROTECT, NOSTVREN
#fuses NOCPB, NOWRT, NOWRTB NOEBTRB, NOEBTR
#fuses BROWNOUT, CPD, NOOSCSEN, BORV20
#fuses NOLVP, NODEBUG, NOWRTC, NOWRTD
 
                             
#use delay(clock = 10MHz)             
                                       
                                                  
#byte lcdport         = 0x06   
#byte lcdport_tris    = 0x86    


#define calibrate		input(pin_C0) 
#define LED				pin_C7

#define CENTRE			0  
#define BACK			1
#define FRONT		    2
#define RIGHT			3
#define LEFT			4
#define CAL_DONE		5
                                                                      
                                                             
#include "3310.c" 
                                       

void setup();                                 
void draw_background();  
void adc_avg(unsigned long *x_axis, unsigned long *y_axis);
float map(float v, float x_min, float x_max, float y_min, float y_max);
float constrain(float value, float value_min, float value_max);


void main()                                       
{                              
   short cal = 0;
   
   unsigned char step = 0;
   unsigned char x_plot = 0;
   unsigned char y_plot = 0;
   
   unsigned long x_present = 0;
   unsigned long y_present = 0;  
   unsigned long x_previous = 0;
   unsigned long y_previous = 0; 
   unsigned long x_axis_max = 590; 
   unsigned long y_axis_max = 590;
   unsigned long x_axis_min = 370;    
   unsigned long y_axis_min = 370;  
   
   float x_dir = 0.0;
   float y_dir = 0.0; 
                                          
   setup();                                
                                
   while(TRUE)                         
   {                           
        adc_avg(&x_present, &y_present);  
        
        if(calibrate)
        {                   
            while(calibrate);
            cal = TRUE;  
        } 
        
        if(cal)
        {
            output_high(LED); 
            nokia_gotoxy(56, 0);
			printf(nokia_printchar,"CAL.");  
			switch(step)
			{
			    case CENTRE:
				{
					nokia_gotoxy(56, 1);          
					printf(nokia_printchar,"CTR"); 
					break;
				} 
				case BACK:
				{
					nokia_gotoxy(56, 1);          
					printf(nokia_printchar,"BACK"); 
					break;
				} 
				case FRONT:
				{
					nokia_gotoxy(56, 1);          
					printf(nokia_printchar,"FRNT");
					y_axis_max = y_present;
					break;                
				}  
				case RIGHT:
				{
					nokia_gotoxy(56, 1);          
					printf(nokia_printchar,"RGHT");
					y_axis_min = y_present;
					break; 
				}  
				case LEFT:
				{                    
					nokia_gotoxy(56, 1);          
					printf(nokia_printchar,"LEFT");
					x_axis_max = x_present;  
					break; 
				} 
				default:
				{
				    nokia_gotoxy(56, 1);          
					printf(nokia_printchar,"DONE");   
					x_axis_min = x_present; 
					step = 0;
					cal = FALSE; 
					output_low(LED); 
					nokia_clean_ddram();     
					nokia_clear_screen(); 
				}                                      
			}
			delay_ms(2600);                         
			step++;
        }                                         
        else             
        {       
        	nokia_gotoxy(56, 0);            
			printf(nokia_printchar,"RUN.");
			nokia_gotoxy(56, 1);            
			printf(nokia_printchar,"    ");                             
        }
                            
		if((x_present != x_previous) || (y_present != y_previous))
        {                                          
	        draw_background();
	        
	        x_present = constrain(x_present, x_axis_min, x_axis_max);
	        y_present = constrain(y_present, y_axis_min, y_axis_max);
	                                                                                           
	        x_plot = map(x_previous, x_axis_min, x_axis_max, 3, 46);
	        drawFilledRectangle(x_plot, 42, (x_plot + 1), 43, 0);     
	        x_plot = map(x_present, x_axis_min, x_axis_max, 3, 46);                        
	        drawFilledRectangle(x_plot, 42, (x_plot + 1), 43, 1);
	                                                                            
	        y_plot = map(y_previous, y_axis_min, y_axis_max, 35, 2);  
	        drawFilledRectangle(43, y_plot, 44, (y_plot + 1), 0);     
	        y_plot = map(y_present, y_axis_min, y_axis_max, 35, 2);     
	        drawFilledRectangle(43, y_plot, 44, (y_plot + 1), 1);  
	                                          
	        x_plot = map(x_previous, x_axis_min, x_axis_max, 5, 33);            
	        y_plot = map(y_previous, y_axis_min, y_axis_max, 33, 5);  
	        lcdPixel(x_plot, y_plot, 0);   
	        x_plot = map(x_present, x_axis_min, x_axis_max, 5, 33);
	        y_plot = map(y_present, y_axis_min, y_axis_max, 33, 5);  
	        lcdPixel(x_plot, y_plot, 1);    
	                                                                 
	        x_dir = map(x_present, x_axis_min, x_axis_max, -1.0, 1.0); 
	        y_dir = map(y_present, y_axis_min, y_axis_max, -1.0, 1.0);        
	        
	        nokia_gotoxy(56, 3);              
	    	printf(nokia_printchar,"%1.1g ", x_dir);  
	    	nokia_gotoxy(56, 5);                    
	    	printf(nokia_printchar,"%1.1g ", y_dir); 
	       	
	    	x_previous = x_present; 
	    	y_previous = y_present;
        }  
   }                                                               
}                                        
                                                         
                                        
void setup()                                 
{                             
   disable_interrupts(global);       
   setup_WDT(WDT_off);
   setup_adc_ports(AN0_AN1_AN3);
   setup_adc(ADC_clock_internal);
   setup_timer_0(T0_OFF);        
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,255,1);
   setup_timer_3(T3_DISABLED); 
   setup_CCP1(CCP_off);
   setup_CCP2(CCP_off);
   setup_SPI(SPI_DISABLED);
   lcdport_tris=0x00;      
   nokia_init();
   set_timer0(0);                                                                                       
   set_timer1(0);
   set_timer2(0);
   set_timer3(0); 
   nokia_clean_ddram();     
   nokia_clear_screen(); 
   lcd_update();           
   delay_ms(100);
}                                   


void draw_background()                        
{                                       
   drawCircle(19, 19, 16, 1); 
   drawCircle(19, 19, 15, 1);  
        
                      
   drawLine(4, 39, 46, 39, 1); 
   drawLine(4, 46, 46, 46, 1); 

   drawLine(3, 45, 3, 40, 1);        
   drawLine(3, 44, 3, 41, 0);       
   drawLine(2, 41, 2, 44, 1); 
   
   drawLine(47, 45, 47, 40, 1);        
   drawLine(47, 44, 47, 41, 0);       
   drawLine(48, 41, 48, 44, 1);       
                               
                               
   drawLine(40, 3, 40, 35, 1);  
   drawLine(47, 3, 47, 35, 1);  
                                                      
   drawLine(41, 2, 46, 2, 1);                
   drawLine(45, 2, 42, 2, 0);                           
   drawLine(45, 1, 42, 1, 1); 
                                         
   drawLine(41, 36, 46, 36, 1);                
   drawLine(45, 36, 42, 36, 0);                           
   drawLine(45, 37, 42, 37, 1);   
   
   nokia_gotoxy(56, 2);   
   printf(nokia_printchar,"X:");  
   nokia_gotoxy(56, 4);              
   printf(nokia_printchar,"Y:");       
}                 
                                                               
                           
void adc_avg(unsigned long *x_axis, unsigned long *y_axis)
{                                                  
    unsigned char s = 16;    
    register unsigned long tmp_x = 0;    
    register unsigned long tmp_y = 0; 
    
    while(s > 0)         
    {
        set_ADC_channel(0);
        delay_ms(1);
        read_adc(adc_start_only);
        while(!adc_done());
        tmp_x += read_adc(adc_read_only);  
        
        set_ADC_channel(1);
        delay_ms(1);
        read_adc(adc_start_only);
        while(!adc_done());
        tmp_y += read_adc(adc_read_only); 
        
        s--;  
    }     
    
    tmp_x >>= 4;  
    tmp_y >>= 4;
                     
    *x_axis = tmp_x;  
    *y_axis = tmp_y; 
}   


float map(float v, float x_min, float x_max, float y_min, float y_max) 
{
    register float m = 0.0;
    m = ((y_max - y_min)/(x_max - x_min));
    return (y_min + (m * (v - x_min)));
}


float constrain(float value, float value_min, float value_max) 
{                             
      if(value >= value_max) 
      { 
           return value_max; 
      } 

      else if(value <= value_min) 
      { 
           return value_min; 
      } 
      
      else 
      { 
           return value; 
      } 
} 
