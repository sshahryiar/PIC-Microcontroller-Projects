#include "fonts.c" 


#define SDA_pin                                pin_D1 
#define SCK_pin                                pin_D2 
#define CS_pin                                 pin_D3 
#define DC_pin                                 pin_D4 
#define RST_pin                                pin_D5
                                                            
#define D0_pin								   SCK_pin
#define D1_pin								   SDA_pin

#define DAT                                          1
#define CMD                                          0
         
#define Set_Lower_Column_Start_Address_CMD        0x00                      
#define Set_Higher_Column_Start_Address_CMD       0x10
#define Set_Memory_Addressing_Mode_CMD            0x20
#define Set_Column_Address_CMD                    0x21
#define Set_Page_Address_CMD                      0x22
#define Set_Display_Start_Line_CMD                0x40
#define Set_Contrast_Control_CMD                  0x81
#define Set_Charge_Pump_CMD                       0x8D
#define Set_Segment_Remap_CMD                     0xA0
#define Set_Entire_Display_ON_CMD                 0xA4
#define Set_Normal_or_Inverse_Display_CMD         0xA6
#define Set_Multiplex_Ratio_CMD                   0xA8
#define Set_Display_ON_or_OFF_CMD                 0xAE
#define Set_Page_Start_Address_CMD                0xB0
#define Set_COM_Output_Scan_Direction_CMD         0xC0                                                                  
#define Set_Display_Offset_CMD                    0xD3
#define Set_Display_Clock_CMD                     0xD5
#define Set_Pre_charge_Period_CMD                 0xD9
#define Set_Common_HW_Config_CMD                  0xDA
#define Set_VCOMH_Level_CMD                       0xDB
#define Set_NOP_CMD                               0xE3

#define Horizontal_Addressing_Mode                0x00
#define Vertical_Addressing_Mode                  0x01
#define Page_Addressing_Mode                      0x02

#define Disable_Charge_Pump                       0x00
#define Enable_Charge_Pump                        0x04
                                                                             
#define Column_Address_0_Mapped_to_SEG0           0x00
#define Column_Address_0_Mapped_to_SEG127         0x01

#define Normal_Display                            0x00
#define Entire_Display_ON                         0x01
                                                    
#define Non_Inverted_Display                      0x00
#define Inverted_Display                          0x01
                                               
#define Display_OFF                               0x00
#define Display_ON                                0x01

#define Scan_from_COM0_to_63                      0x00
#define Scan_from_COM63_to_0                      0x08

#define x_size                                    128
#define x_max                                     x_size
#define x_min                                     0
#define y_size                                    64
#define y_max                                     8
#define y_min                                     0

#define ON                                        1
#define OFF                                       0

#define ROUND                                     1
#define SQUARE                                    0

#define buffer_size                               1024//(x_max * y_max)


unsigned char buffer[buffer_size];


void OLED_init();
void OLED_reset_sequence();
void OLED_write(unsigned char value, unsigned char type);
void OLED_gotoxy(unsigned char x_pos, unsigned char y_pos);
void OLED_fill(unsigned char bmp_data);
void OLED_clear_screen();
void OLED_clear_buffer();
void OLED_cursor(unsigned char x_pos, unsigned char y_pos);
void OLED_draw_bitmap(unsigned char xb, unsigned char yb, unsigned char xe, unsigned char ye, unsigned char bmp_img[]);  
void OLED_print_char(unsigned char x_pos, unsigned char y_pos, unsigned char ch);
void OLED_print_string(unsigned char x_pos, unsigned char y_pos, unsigned char *ch);
void OLED_print_chr(unsigned char x_pos, unsigned char y_pos, signed long value);
void OLED_print_int(unsigned char x_pos, unsigned char y_pos, signed long long value);
void OLED_print_decimal(unsigned char x_pos, unsigned char y_pos, unsigned long value, unsigned char points);
void OLED_print_float(unsigned char x_pos, unsigned char y_pos, float value, unsigned char points); 
void Draw_Pixel(unsigned char x_pos, unsigned char y_pos, short colour);
void Draw_Line(signed long x1, signed long y1, signed long x2, signed long y2, short colour);
void Draw_Rectangle(signed long x1, signed long y1, signed long x2, signed long y2, short fill, short colour, short type);
void Draw_Circle(signed long xc, signed long yc, signed long radius, short fill, short colour);
