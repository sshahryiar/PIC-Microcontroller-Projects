#define LM75_I2C_Address                0x48    

#define LM75_write_cmd                  ((LM75_I2C_Address << 1) & 0xFE)     
#define LM75_read_cmd                   ((LM75_I2C_Address << 1) | 0x01)             


//Registers                                                                                                          
                                                                                                          
#define pointer_temp_reg                0x00
#define pointer_conf_reg                0x01
#define pointer_thyst_reg               0x02
#define pointer_tos_reg                 0x03
                                                                                 
//Configure Register Bits     

#define queue_value_1                   0x00
#define queue_value_2                   0x08
#define queue_value_4                   0x10
#define queue_value_6                   0x18

#define _9_bit_resolution               0x00
#define _10_bit_resolution              0x20
#define _11_bit_resolution              0x40
#define _12_bit_resolution              0x60

#define OS_POL_LOW                      0x00
#define OS_POL_HIGH                     0x04

#define OS_Comp                         0x00                   
#define OS_INT                          0x02
                                        
#define normal                          0x00                       
#define shutdown                        0x01

#define OS_mode_disable                 0x00
#define OS_mode_enable                  0x80
                                                                                 

void LM75_init(void);
void LM75_write_byte(unsigned char pointer, unsigned char value);
void LM75_write_bytes(unsigned char pointer, signed long value);
signed long LM75_read_byte(unsigned char pointer);     
signed long LM75_read_bytes(unsigned char pointer);
float LM75_get_temp(void);
