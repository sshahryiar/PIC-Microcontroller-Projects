#define data_pin			PIN_A4
#define clock_pin			PIN_A5 


void P9813_init();
void P9813_clock(); 
void turn_off_strip();
void send_data(unsigned long long value); 
unsigned char make_anti_code(unsigned char value);
void set_colour(unsigned char R, unsigned char G, unsigned char B);
