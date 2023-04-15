#define CS_pin             pin_C0
#define SCK_pin            pin_C1
#define SO_pin             input(pin_C2)

#define T_min              0
#define T_max              1024

#define count_max          4096

#define no_of_pulses       16

#define deg_C              0
#define deg_F              1
#define tmp_K              2

#define open_contact       0x04
#define close_contact      0x00

#define scalar_deg_C       0.25
#define scalar_deg_F_1     1.8
#define scalar_deg_F_2     32.0
#define scalar_tmp_k       273.0

#define no_of_samples      16


void MAX6675_init();
unsigned char MAX6657_get_ADC(unsigned long *ADC_data);
float MAX6675_get_T(unsigned long ADC_value, unsigned char T_unit);





