#define CS_pin                            pin_C2
#define CLK_pin                           pin_C3
#define D_in_pin                          pin_C5
#define D_out_pin                         input(pin_C4)

#define start                             0x01

#define opcode_erase_all                  0x20
#define opcode_write_all                  0x10
#define opcode_write_and_erase_enable     0x30
#define opcode_write_and_erase_disable    0x00        
#define opcode_write                      0x40
#define opcode_read                       0x80
#define opcode_erase                      0xC0


//Command format  start bit 0x01 + (opcode (2 bits) + address) + data
//EEPROM only starts functioning when both clock and data lines are high, i.e when it received a start bit

void EE_93LC46B_init();
void write_value(unsigned char value);
unsigned char read_value();
void EE_93LC46B_write(unsigned char address, unsigned long value);
unsigned long EE_93LC46B_read(unsigned char address);
void EE_93LC46B_erase_and_write_enable();
void EE_93LC46B_erase_and_write_disable();
void EE_93LC46B_erase(unsigned char address);
void EE_93LC46B_erase_all();
