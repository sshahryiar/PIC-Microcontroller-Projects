#include <10F200.h>                                     
                                                  
                                                  
#fuses PROTECT, NOWDT, NOMCLR         
                                                  
                                                  
#use delay (clock = 4MHz)                                                 
 
                                                       
                                                   
#include "MCP23S17.c" 
                                                                                     
                                                  
void setup();                                                  
                                                                              
                                                  
void main()                                       
{                                                  
    unsigned char in = 0;
    setup();
           
    while(TRUE)                                   
    {                 
		in = MCP23S17_read(GPIOB); 
		MCP23S17_write(OLATA, in);
    }                                                                    
}                                                 
                                                  
                                                  
void setup()                                      
{                                                                           
    setup_MCP23S17();                             
}                                                 
                                                  
                                                  
                                                
                                            
                         
                      











