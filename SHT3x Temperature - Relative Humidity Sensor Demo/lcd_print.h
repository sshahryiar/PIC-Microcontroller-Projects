#define no_of_custom_symbol     1
#define array_size_per_symbol   8
#define array_size              (array_size_per_symbol * no_of_custom_symbol)


void load_custom_symbol(void);
void print_symbol(unsigned char x_pos, unsigned char y_pos, unsigned char symbol_index);
void print_C(unsigned char x_pos, unsigned char y_pos, signed int value);
void print_I(unsigned char x_pos, unsigned char y_pos, signed long value);
void print_D(unsigned char x_pos, unsigned char y_pos, signed int value, unsigned char points);
void print_F(unsigned char x_pos, unsigned char y_pos, float value, unsigned char points);
