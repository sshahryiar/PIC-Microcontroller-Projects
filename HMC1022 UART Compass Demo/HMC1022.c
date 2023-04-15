#include "HMC1022.h" 
                                                          
      
float read_heading()
{                         
     unsigned char s = 0;  
     unsigned long CRC = 0x00;
     register float heading = 0;
     
     unsigned char data_bytes[no_of_data_bytes_returned] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; 
                                                            
     putc(Get_Angular_Measurement);  
               
     for(s = 0; s < no_of_data_bytes_returned; s++)
     {                
        data_bytes[s] = getc();
        if(s < 7)   
        {
            CRC += data_bytes[s];
        }
     }                                                       
     
     CRC = (CRC & 0xFF); 
     
     if(CRC == data_bytes[7])
     {                                     
        for(s = 2; s < 7; s++)                        
        {                                       
        	data_bytes[s] -= 0x30;               
        }
        
        heading = (data_bytes[2] * 100.0);
    	heading += (data_bytes[3] * 10.0);                       
    	heading += (data_bytes[4] * 1.0);                 
    	heading += (data_bytes[6] * 0.1);  
     }     
     else
     {
        heading = -1;
     }
                                              
     return heading;
} 


void calibrate_compass()
{
	unsigned char s = 0x00;   	

	putc(Start_Calibration); 
   	
	for(s = 0; s < 60; s++)
	{
	    output_high(calibration_LED);   	
		delay_ms(100);
		output_low(calibration_LED);
		delay_ms(900);
	}  
   	
	for(s = 0; s < 60; s++)    
	{
	    output_high(calibration_LED);  	
		delay_ms(400);             
		output_low(calibration_LED);
		delay_ms(600);
	}              
   	
	putc(End_Calibration); 
}


void factory_reset()
{
	putc(0xA0); 
	putc(0xAA);
	putc(0xA5);  
	putc(0xC5); 
}   

                                               
void set_I2C_address(unsigned char i2c_address)  
{          
	putc(0xA0); 
	putc(0xAA); 
	putc(0xA5);  
	putc(i2c_address); 
}    


void set_declination_angle(unsigned long angle)
{ 
	unsigned long hb = 0;
	unsigned char lb = 0;
   	
	lb = (angle & 0x00FF);    
	                         
	hb = (angle & 0xFF00);
	hb >>= 8;  
	            
	putc(Set_Magnetic_Declination_High_Byte);   	
	putc(hb);
   	
	putc(Set_Magnetic_Declination_Low_Byte);
	putc(lb);
}
