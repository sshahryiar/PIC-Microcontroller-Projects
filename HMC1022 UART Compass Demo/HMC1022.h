#define Default_I2C_Address												   0xE0 
                                                                          
#define Get_Angular_Measurement											   0x31
#define Start_Calibration												   0xC0  
#define End_Calibration													   0xC1                      
#define Set_Magnetic_Declination_High_Byte								   0x03   
#define Set_Magnetic_Declination_Low_Byte								   0x04
                                                                                                                                                               
#define no_of_data_bytes_returned               						   0x08

#define calibration_LED													 pin_C1


#use RS232(UART1, baud = 9600, stop = 1, parity = N, timeout = 1000)
                                                                       

float read_heading();     
void calibrate_compass();  
void factory_reset();
void set_I2C_address(unsigned char i2c_address);
void set_declination_angle(unsigned long angle);
