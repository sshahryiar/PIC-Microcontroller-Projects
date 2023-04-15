#include "QMC5883.h"


void QMC5883_init(void)
{
    I2C_init(20000);
    QMC5883_write_byte(QMC5883_control_reg_2, QMC5883_soft_reset);
    delay_ms(10);
    QMC5883_write_byte(QMC5883_period_set_reset_reg, 0x01);    
    QMC5883_write_byte(QMC5883_control_reg_1, (QMC5883_continuous_mode \
                                             | QMC5883_output_data_rate_50Hz \
                                             | QMC5883_full_scale_range_8G \
                                             | QMC5883_over_sample_ratio_512));
}


void QMC5883_write_byte(unsigned char address, unsigned char value)
{
   I2C_Start();
   I2C_Write(QMC5883_I2C_address);
   I2C_Write(address);
   I2C_Write(value);
   I2C_Stop();
}


unsigned char QMC5883_read_byte(unsigned char address)
{
   unsigned char value = 0; 
   
   I2C_Start();
   I2C_Write(QMC5883_I2C_address);
   I2C_Write(address);
   I2C_Start();
   
   I2C_Write((QMC5883_I2C_address + 1));
   value = I2C_Read(0);                       
   I2C_Stop();
   
   return value;   
}


unsigned long QMC5883_read_word(unsigned char address)
{
   unsigned char LB = 0;
   unsigned char HB = 0;
   unsigned long value = 0;
   
   I2C_Start();
   I2C_Write(QMC5883_I2C_address);
   I2C_Write(address);
   I2C_Start();
   
   I2C_Write((QMC5883_I2C_address + 1));
   LB = I2C_Read(1);  
   HB = I2C_Read(0);                       
   I2C_Stop();
   
   value = HB;
   value <<= 8;
   value |= LB;
   
   return value;   
}


float QMC5883_read_temperature(void)
{
   signed long value = 0;
   float temp = 0.0;
   
   value = (signed long)QMC5883_read_word(QMC5883_T_output_LSB_reg);
   
   temp = ((float)(value) / -100.0);

   return temp;
}


void QMC5883_read_axes(signed long *x_axis, signed long *y_axis, signed long *z_axis)
{
   signed long value = 0;
   
   value = (signed long)QMC5883_read_word(QMC5883_output_X_LSB_reg);
   *x_axis = value;
   
   value = (signed long)QMC5883_read_word(QMC5883_output_Y_LSB_reg);
   *y_axis = value;
   
   value = (signed long)QMC5883_read_word(QMC5883_output_Z_LSB_reg);
   *z_axis = value;
}


float QMC5883_read_heading(signed long a, signed long b)
{
   float value = 0.0;
   
   value = atan2((float)a, (float)b);
   
   if(value < 0.0)
   {
      value += (2.0 * PI);
   }
    
   if(value > (2.0 * PI))               
   {                            
      value -= (2.0 * PI);
   }                    
                   
   value *= (180.0 / PI); 
                  
   return value;
}
