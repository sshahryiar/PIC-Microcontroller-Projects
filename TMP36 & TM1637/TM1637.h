#define TM1637_CLK_PIN              PIN_A4
#define TM1637_DAT_PIN              PIN_A5 
                                                                    
#define TM1637_CLK_HIGH             output_high(TM1637_CLK_PIN)     
#define TM1637_CLK_LOW              output_low(TM1637_CLK_PIN) 

#define TM1637_DAT_HIGH             output_high(TM1637_DAT_PIN) 
#define TM1637_DAT_LOW              output_low(TM1637_DAT_PIN)
                                                                                   
#define TM1637_DELAY_US             4

#define TM1637_BRIGHTNESS_MIN       0
#define TM1637_BRIGHTNESS_1         1
#define TM1637_BRIGHTNESS_2         2
#define TM1637_BRIGHTNESS_3         3
#define TM1637_BRIGHTNESS_4         4
#define TM1637_BRIGHTNESS_5         5                                           
#define TM1637_BRIGHTNESS_6         6
#define TM1637_BRIGHTNESS_MAX       7
                                                             
#define TM1637_POSITION_MAX         4

#define TM1637_CMD_SET_DATA      0x40
#define TM1637_CMD_SET_ADDR      0xC0
#define TM1637_CMD_SET_DSIPLAY   0x80                           

#define TM1637_SET_DATA_WRITE    0x00
#define TM1637_SET_DATA_READ     0x02
#define TM1637_SET_DATA_A_ADDR   0x00
#define TM1637_SET_DATA_F_ADDR   0x04
#define TM1637_SET_DATA_M_NORM   0x00
#define TM1637_SET_DATA_M_TEST   0x10
#define TM1637_SET_DISPLAY_OFF   0x00    
#define TM1637_SET_DISPLAY_ON    0x08   


const unsigned char seg_data[10] =
{
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9                                                          
}; 


void TM1637_init(void);
void TM1637_start(void);
void TM1637_stop(void);
unsigned char TM1637_write_byte(unsigned char value);
void TM1637_send_command(unsigned char value);
void TM1637_clear(void);
void TM1637_display_segments(unsigned char position, unsigned char segment_value, unsigned char colon_state);

