#define SHT_SCL_pin                             pin_C4
#define SHT_SDA_pin                             pin_C5
                                                        
#define SHT1x_address                           0x00

#define measure_T_cmd                           0x03
#define measure_RH_cmd                          0x05
#define write_status_reg_cmd                    0x06
#define read_status_reg_cmd                     0x07
#define soft_reset_cmd                          0x1E

#define no_LVD                                  0x00
#define LVD                                     0x40
#define LVD_status                              0x40

#define heater_on                               0x04
#define heater_off                              0x00
#define heater_status                           0x04

#define OPT_reload                              0x02
#define no_OPT_reload                           0x00

#define low_resolution                          0x01
#define high_resolution                         0x00

#define T1                                      0.01              // for 14 Bit
#define T2                                   0.00008              // for 14 Bit
#define C1                                      -4.0              // for 12 Bit
#define C2                                    0.0405              // for 12 Bit
#define C3                                -0.0000028              // for 12 Bit
#define NACK                                       0
#define ACK                                        1


void SHT1x_init(void); 
void SHT1x_wait(void);
void SHT1x_reset(void);                                            
void SHT1x_software_reset (void);    
void SHT1x_start_command(void);
unsigned char SHT1x_write(unsigned char value);
unsigned char SHT1x_read_byte(void);
void SHT1x_send_ack(unsigned char ack_val);
unsigned long SHT1x_read(void);
unsigned long SHT1x_get_temperature(void);                            
unsigned long SHT1x_get_relative_humidity(void);
void SHT1x_calculate_values(unsigned long temp, unsigned long humid, float &tc, float &rhlin, float &rhtrue);
void SHT1x_get_data(float &temp, float &truehumid);                                                                                            
