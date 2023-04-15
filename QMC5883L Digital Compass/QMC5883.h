#use I2C(MASTER, SDA = PIN_C0, SCL = PIN_C1)   


#define QMC5883_I2C_address                 0x1A

#define QMC5883_output_X_LSB_reg            0x00
#define QMC5883_output_X_MSB_reg            0x01
#define QMC5883_output_Y_LSB_reg            0x02
#define QMC5883_output_Y_MSB_reg            0x03
#define QMC5883_output_Z_LSB_reg            0x04
#define QMC5883_output_Z_MSB_reg            0x05
#define QMC5883_status_reg                  0x06
#define QMC5883_T_output_LSB_reg            0x07
#define QMC5883_T_output_MSB_reg            0x08
#define QMC5883_control_reg_1               0x09
#define QMC5883_control_reg_2               0x0A
#define QMC5883_period_set_reset_reg        0x0B

#define QMC5883_DRDY_flag                   0x01
#define QMC5883_OVL_flag                    0x02
#define QMC5883_DOR_flag                    0x04

#define QMC5883_standby_mode                0x00
#define QMC5883_continuous_mode             0x01

#define QMC5883_output_data_rate_10Hz       0x00
#define QMC5883_output_data_rate_50Hz       0x04
#define QMC5883_output_data_rate_100Hz      0x08
#define QMC5883_output_data_rate_200Hz      0x0C

#define QMC5883_full_scale_range_2G         0x00
#define QMC5883_full_scale_range_8G         0x10

#define QMC5883_over_sample_ratio_512       0x00
#define QMC5883_over_sample_ratio_256       0x40
#define QMC5883_over_sample_ratio_128       0x80
#define QMC5883_over_sample_ratio_64        0xC0

#define QMC5883_interrupt_pin_disable       0x00
#define QMC5883_interrupt_pin_enable        0x01

#define QMC5883_I2C_pointer_roll_over_off   0x00
#define QMC5883_I2C_pointer_roll_over_on    0x40

#define QMC5883_soft_reset                  0x80

#define PI                                  3.142


void QMC5883_init(void);
void QMC5883_write_byte(unsigned char address, unsigned char value);
unsigned char QMC5883_read_byte(unsigned char address);
unsigned long QMC5883_read_word(unsigned char address);
float QMC5883_read_temperature(void);
void QMC5883_read_axes(signed long *x_axis, signed long *y_axis, signed long *z_axis);
float QMC5883_read_heading(signed long a, signed long b);
