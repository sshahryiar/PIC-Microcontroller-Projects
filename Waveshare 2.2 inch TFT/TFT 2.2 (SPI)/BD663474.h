#define TFT_CS                                                                           pin_A1
#define TFT_RS                                                                           pin_A2
#define TFT_RST                                                                          pin_D0

#define White                                                                            0xFFFF
#define Black                                                                            0x0000
#define Grey                                                                             0xF7DE
#define Grey2                                                                            0x8430   
#define Light_Grey                                                                       0xC618  
#define Dark_Grey                                                                        0x8410   
#define Purple                                                                           0xF81F 
#define Grey_Blue                                                                        0x5458  
#define Blue                                                                             0x001F
#define Dark_Blue                                                                        0x01CF
#define Light_Blue                                                                       0x051F
#define Light_Blue_2                                                                     0x7D7C
#define Red                                                                              0xF800
#define Green                                                                            0x07E0
#define Cyan                                                                             0x7FFF
#define Yellow                                                                           0xFFE0
#define Orange                                                                           0xFC08

#define SQUARE                                                                           0
#define ROUND                                                                            1

#define NO                                                                               0
#define YES                                                                              1

#define ON                                                                               1
#define OFF                                                                              0

#define DAT                                                                              1
#define CMD                                                                              0

#define  MAX_X                                                                           240
#define  MAX_Y                                                                           320


void TFT_IO_init();
void TFT_reset();
void TFT_write_word(unsigned long value);
void TFT_write(unsigned long value, short mode);
void TFT_write_RAM_prepare();
void TFT_write_reg(unsigned long cmd_value, unsigned long data_value);
void TFT_init();
void TFT_set_display_address_window(signed long xs, signed long ys, signed long xe, signed long ye);
void TFT_set_cursor(signed long x_pos, signed long y_pos);
void TFT_fill(unsigned long colour);
unsigned long TFT_BGR2RGB(unsigned long colour);
unsigned long RGB565_converter(unsigned char r, unsigned char g, unsigned char b);
void swap_values(signed long *a, signed long *b);
void Draw_Pixel(signed long x_pos, signed long y_pos, unsigned long colour);
void Draw_Line(signed long x1, signed long y1, signed long x2, signed long y2, unsigned long colour);
void Draw_V_Line(signed long x1, signed long y1, signed long y2, unsigned colour);
void Draw_H_Line(signed long x1, signed long x2, signed long y1, unsigned colour);
void Draw_Triangle(signed long x1, signed long y1, signed long x2, signed long y2, signed long x3, signed long y3, short fill, unsigned long colour);
void Draw_Rectangle(signed long x1, signed long y1, signed long x2, signed long y2, short fill, unsigned char type, unsigned long colour, unsigned long back_colour);
void Draw_H_Bar(signed long x1, signed long x2, signed long y1, signed long bar_width, signed long bar_value, unsigned long border_colour, unsigned long bar_colour, unsigned long back_colour, unsigned char border);
void Draw_V_Bar(signed long x1, signed long y1, signed long y2, signed long bar_width, signed long bar_value, unsigned long border_colour, unsigned long bar_colour, unsigned long back_colour, unsigned char border);
void Draw_Circle(signed long xc, signed long yc, signed long radius, short fill, unsigned long colour);
void Draw_Font_Pixel(unsigned long x_pos, unsigned long y_pos, unsigned long colour, unsigned char pixel_size);
void print_char(unsigned long x_pos, unsigned long y_pos, unsigned char font_size, unsigned long colour, unsigned long back_colour, char ch);
void print_str(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned long colour, unsigned long back_colour, char *ch);
void print_C(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned long colour, unsigned long back_colour, signed long value);
void print_I(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned long colour, unsigned long back_colour, signed long value);
void print_D(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned long colour, unsigned long back_colour, unsigned long value, unsigned char points);
void print_F(unsigned char x_pos, unsigned char y_pos, unsigned char font_size, unsigned long colour, unsigned long back_colour, float value, unsigned char points);
void Draw_BMP(unsigned long x_pos, unsigned long y_pos, unsigned long H, unsigned long W, unsigned char *bitmap);
