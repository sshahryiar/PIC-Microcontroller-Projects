#define number_of_LEDs                          60
#define number_of_bits                          24
#define channels                                3

#define one                                     0xF8
#define zero                                    0xC0

#define HIGH                                    1
#define LOW                                     0

#define ON                                      1
#define OFF                                     0

#define YES                                     1
#define NO                                      0


unsigned char LED_colours[number_of_LEDs][channels];


void WS2811_init(void);
void WS2811_update(void);
void WS2811_reset(void);
void WS2811_send_data(unsigned char r, unsigned char g, unsigned char b);
void WS2811_set_common_colour_on_all_LEDs(unsigned char r, unsigned char g, unsigned char b);
void WS2811_set_colour_on_channel(unsigned int16 channel, unsigned char r, unsigned char g, unsigned char b);
