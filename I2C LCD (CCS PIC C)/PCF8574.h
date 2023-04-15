#define PCF8574_address 		0x40 

#define PCF8574_write_cmd		PCF8574_address             
#define PCF8574_read_cmd		(PCF8574_address + 1)   

#define SDA_pin					pin_A5
#define SCL_pin                 pin_A4     


#use I2C(Master, SDA = SDA_pin, SCL = SCL_pin)

             
unsigned char PCF8574_read();                
void PCF8574_write(unsigned char data_byte);                                   
