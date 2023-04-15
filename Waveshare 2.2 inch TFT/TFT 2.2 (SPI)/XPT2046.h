#define TP_INT                                                                           input(pin_A0)
#define TP_CS                                                                            pin_A4

#define CMD_RDY                                                                          0x90
#define CMD_RDX                                                                          0xD0

#define error                                                                            50.0

#define RL_min                                                                           200.0
#define RL_max                                                                           3800.0
#define RW_min                                                                           200.0
#define RW_max                                                                           3800.0

#define res_l                                                                            319.0
#define res_w                                                                            239.0


unsigned long tpx = 0x0000;
unsigned long tpy = 0x0000;


void tp_init();
void tp_IO_init();
unsigned long TP_read();
void TP_read_coordinates(unsigned long *x_pos, unsigned long *y_pos);
unsigned long filter_data(unsigned long value, unsigned char axis);
float map(float value, float x_min, float x_max, float y_min, float y_max);
float constrain(float value, float value_min, float value_max);
