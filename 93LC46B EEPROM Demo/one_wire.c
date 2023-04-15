#define ONE_WIRE_PIN   PIN_A0          
                            
                     
short onewire_reset(); 
void onewire_write_bit(short bit_value);
short onewire_read_bit();
void onewire_write(unsigned char value);    
unsigned char onewire_read();
    
                                                       
short onewire_reset()  
{                                         
    short res = FALSE; 
                     
    output_low(ONE_WIRE_PIN);
    delay_us(480);         
    output_float(ONE_WIRE_PIN);                      
    delay_us(60);                                      
    res = input(ONE_WIRE_PIN);
    delay_us(480);       
    
    return res; 
} 

                               
void onewire_write_bit(short bit_value)
{
   output_low(ONE_WIRE_PIN); 
      
   if(bit_value)
   {        
      delay_us(104);
      output_high(ONE_WIRE_PIN);   
   }                                                                                         
}     


short onewire_read_bit()        
{     
   output_low(ONE_WIRE_PIN);  
   output_high(ONE_WIRE_PIN);  
   delay_us(15);      
    
   return(input(ONE_WIRE_PIN));    
}

             
void onewire_write(unsigned char value) 
{                    
    unsigned char s = 0; 
    
    while(s < 8)    
    {                              
        if((value & (1 << s)))
        {
           output_low(ONE_WIRE_PIN); 
           delay_us(1);    
           output_float(ONE_WIRE_PIN);
           delay_us(60);   
        }       
                            
        else
        {
            output_low(ONE_WIRE_PIN);                                                               
           delay_us(60);           
           output_float(ONE_WIRE_PIN); 
           delay_us(1); 
        }
        s++;
    }
}                                      
        
          
unsigned char onewire_read()
{
    unsigned char s = 0x00;
    unsigned char value = 0x00;
   
    while(s < 8) 
    {
        output_low(ONE_WIRE_PIN);
        delay_us(2);                  
        output_float(ONE_WIRE_PIN);
        
        if(input(ONE_WIRE_PIN))  
        {                                                                             
            value |=  (1 << s);                         
        }        
         
        delay_us(60);                                                                 
        s++;
    }     
                                     
    return value;
}                       
