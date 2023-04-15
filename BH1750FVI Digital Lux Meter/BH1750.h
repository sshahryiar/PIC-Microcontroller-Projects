#use I2C (Master, SDA = PIN_A0, SCL = PIN_A1)   


#define	BH1750_address_w			0x46                               
#define	BH1750_address_r			0x47 
         
#define	power_down					0x00
#define 	power_up						0x01
#define  reset							0x07 
#define  cont_H_res_mode1  		0x10 
#define  cont_H_res_mode2  		0x11  
#define  cont_L_res_mode   		0x13                                          
#define  one_time_H_res_mode1  	0x20 
#define  one_time_H_res_mode2  	0x21
#define  one_time_L_res_mode  	0x23                                     
                        

void BH1750_init();
void BH1750_write(unsigned char cmd);                                           
unsigned long BH1750_read_word(); 
unsigned long get_lux_value(unsigned char mode, unsigned long delay_time);          
