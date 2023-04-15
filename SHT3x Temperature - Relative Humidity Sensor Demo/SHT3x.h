#define I2C_bus_speed                           100000

#define SHT3x_I2C_address                       0x88

#define SHT3x_MEAS_HIGH_REP_CLK_STRETCH         0x2C06 
#define SHT3x_MEAS_MED_REP_CLK_STRETCH          0x2C0D
#define SHT3x_MEAS_LOW_REP_CLK_STRETCH          0x2C10
#define SHT3x_MEAS_HIGH_REP                     0x2400
#define SHT3x_MEAS_MED_REP                      0x240B
#define SHT3x_MEAS_LOW_REP                      0x2416 
#define SHT3x_READ_STATUS                       0xF32D  
#define SHT3x_CLEAR_STATUS                      0x3041
#define SHT3x_SOFT_RESET                        0x30A2
#define SHT3x_ENABLE_INTERNAL_HEATER            0x306D    
#define SHT3x_DISABLE_INTERNAL_HEATER           0x3066 

#define SHT3x_heater_on                         0x00
#define SHT3x_heater_off                        0x01

#define SHT3x_ERROR                             0x01
#define SHT3x_NO_ERROR                          0x00


unsigned long make_word(unsigned char hb, unsigned char lb);
void SHT3x_init(void);
void SHT3x_reset(void);
unsigned long SHT3x_read_status_reg(void);
void SHT3x_heater_state(unsigned char state);
void SHT3x_write_command(unsigned long value);
unsigned char SHT3x_calculate_crc8(unsigned char *value);
unsigned char SHT3x_read_temp_humidity(float *RH_value, float *T_value);
