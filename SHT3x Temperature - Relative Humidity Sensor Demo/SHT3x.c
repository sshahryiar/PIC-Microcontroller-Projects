#include "SHT3x.h"


unsigned long make_word(unsigned char hb, unsigned char lb)
{
    unsigned long value = 0x0000;

    value = hb;
    value <<= 0x08;
    value |= lb;

    return value;
}


void SHT3x_init(void)
{
    I2C_init(TRUE); 
    delay_ms(100);
    SHT3x_reset();
    SHT3x_heater_state(SHT3x_heater_off);
}


void SHT3x_reset(void)
{
    SHT3x_write_command(SHT3x_SOFT_RESET);
    delay_ms(15);
}


unsigned long SHT3x_read_status_reg(void) 
{
    unsigned char crc = 0x00;
    unsigned char lb = 0x00;
    unsigned long hb = 0x0000;

    SHT3x_write_command(SHT3x_READ_STATUS);

    I2C_start();
    I2C_Write(SHT3x_I2C_address | 1);
    hb = I2C_Read(1);
    lb = I2C_Read(1);
    crc = I2C_Read(0);
    I2C_Stop();

    hb <<= 0x08;
    hb |= lb;

    return hb;
}


void SHT3x_heater_state(unsigned char state)
{
    switch(state)
    {
        case SHT3x_heater_on:
        {
            SHT3x_write_command(SHT3x_ENABLE_INTERNAL_HEATER);
            break;
        }

        default:
        {
            SHT3x_write_command(SHT3x_DISABLE_INTERNAL_HEATER);
            break;
        }
    };
}


void SHT3x_write_command(unsigned long value)
{
    unsigned char hb = 0x00;
    unsigned char lb = 0x00;

    lb = (value & 0x00FF);
    hb = ((value & 0xFF00) >> 0x08);
    
    I2C_Start();
    I2C_Write(SHT3x_I2C_address);
    I2C_Write(hb);
    I2C_Write(lb);
    I2C_Stop();
}


unsigned char SHT3x_calculate_crc8(unsigned char *value) 
{
    signed char i = 0x00;
    signed char j = 0x00;
    unsigned char crc_value = 0xFF;

    for(i = 0x00; i < 0x02; i++)
    {
        crc_value ^= value[i];

        for(j = 0x08; j > 0x00; --j)
        {
           if(crc_value & 0x80)
           {
               crc_value = ((crc_value << 1) ^ 0x131);
           }
           else
           {
               crc_value <<= 1;
           }
        };
    };

    return crc_value;
}


unsigned char SHT3x_read_temp_humidity(float *RH_value, float *T_value)
{
    unsigned char read_buffer[0x06] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    signed long long t_tmp = 0x000000000;
    signed long long h_tmp = 0x000000000;

    SHT3x_write_command(SHT3x_MEAS_HIGH_REP);
    delay_ms(20);
    
    I2C_Start();
    I2C_Write(SHT3x_I2C_address | 1);
    read_buffer[0x00] = I2C_Read(1);
    read_buffer[0x01] = I2C_Read(1);
    read_buffer[0x02] = I2C_Read(1);
    read_buffer[0x03] = I2C_Read(1);
    read_buffer[0x04] = I2C_Read(1);
    read_buffer[0x05] = I2C_Read(0);
    I2C_Stop();

    if((read_buffer[0x02] != SHT3x_calculate_crc8(read_buffer)) || (read_buffer[0x05] != SHT3x_calculate_crc8((read_buffer + 0x03))))
    {
        return SHT3x_ERROR;
    }

    t_tmp = make_word(read_buffer[0x00], read_buffer[0x01]);
    t_tmp = (((4375 * t_tmp) >> 14) - 4500.0);
    *T_value = (((float)t_tmp) / 100.0);

    h_tmp = make_word(read_buffer[0x03], read_buffer[0x04]);
    h_tmp = ((625 * h_tmp) >> 12);
    *RH_value = (((float)h_tmp) / 100.0);
    
    return SHT3x_NO_ERROR;
}
