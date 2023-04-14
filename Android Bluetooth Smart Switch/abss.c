#include <12F629.h> 


#device *= 16   
                       

#fuses NOWDT, PROTECT, INTRC_IO, NOMCLR, PUT, BROWNOUT, CPD  
                                   

#define LED			PIN_A1   
#define LOAD		PIN_A2 
#define TX_pin		PIN_A4
#define RX_pin		PIN_A5         

                     
#use delay (internal = 4MHz)                                                            
#use RS232(BAUD = 9600, XMIT = TX_pin, RCV = RX_pin, STOP = 1, PARITY = N, BITS = 8)  


short on_flag = FALSE;  
short off_Flag = FALSE; 
unsigned char cmd = 0; 
unsigned long t = 0;
unsigned long long s_on = 0;      
unsigned long long s_off = 0;   

               
void setup(); 


#int_timer1

void timer_ISR()
{                          
	set_timer1(64536);   
	         
	t++;                        
	if(t > 999)                                 
	{                 
		if((on_flag == TRUE) || (off_flag == TRUE))
    	{
    	    if(on_flag == TRUE)  
    	    {                            
        	    if(s_on > 0)
        	    {                         
        	       s_on--;                
        	    }
        	    if(s_on == 0)
        	    {
        	    	cmd = 'H';                   
        	    }     
    	    }                       
    	    if(off_flag == TRUE) 
    	    {                        
        	    if(s_off > 0)
        	    {
        	       s_off--;     
        	    }
        	    if(s_off == 0)
        	    {                             
        	    	cmd = 'L';         
        	    } 
    	    }
    	    output_toggle(LED);  
    	}
    	t = 0;
	}
   	
	if(kbhit())
    {                                  
        cmd = getc();
    }  
}
                     
                
void main()
{                                      
	setup();
	                             
	while(TRUE)
	{           
         if(kbhit())
         {
        	cmd = getc();
         }   
                                       
         switch(cmd)
         {     
        	case 'h':                       
        	{                       
        	    output_high(LOAD);   
        	    output_high(LED);  
        	    cmd = 0; 
        	    break;
        	}    
        	case 'l':
        	{                                 
        	    output_low(LOAD);   
        	    output_low(LED); 
        	    cmd = 0; 
        	    break;           
        	}  
        	case 'H':
        	{                       
        	    output_high(LOAD);      
        	    output_high(LED);  
        	    on_flag = FALSE;  
        	    s_on = 0;                        
        	    s_off = 0; 
        	    cmd = 0; 
        	    break;
        	}               
        	case 'L':                            
        	{                                 
        	    output_low(LOAD);  
        	    output_low(LED);
        	    off_flag = FALSE;  
        	    s_on = 0;         
        	    s_off = 0;          
        	    break;
        	}  
        	case 'm':
        	{                  
        	    output_high(LOAD); 
        	    s_off += 1800;      
        	    off_flag = TRUE;   
        	    cmd = 0; 
        	    break;                      
        	}                         
        	case 's':
        	{   
        	    output_high(LOAD);   
        	    s_off += 30;     
        	    off_flag = TRUE;
        	    cmd = 0; 
        	    break;
        	}               
        	case 'M':
        	{                      
        	    output_low(LOAD);                             
        	    s_on += 1800; 
        	    on_flag = TRUE;  
        	    cmd = 0; 
        	    break;           
        	}  
        	case 'S':            
        	{                                             
        	    output_low(LOAD);      
        	    s_on += 30;                           
        	    on_flag = TRUE;  
        	    cmd = 0;             
        	    break;
        	}                         
        	default:
        	{
        	    break;
        	}
         }    
	};       
}
                                                                                                       

void setup()
{
	disable_interrupts(global); 
	setup_comparator(NC_NC_NC_NC);  
	setup_timer_0(T0_internal);       
	setup_timer_1(T1_internal);           
	set_timer0(0);  
	set_timer1(64536); 
	enable_interrupts(global);          
	enable_interrupts(int_timer1);    
	output_low(LED); 
	output_low(LOAD);
	delay_ms(100);
} 
          
