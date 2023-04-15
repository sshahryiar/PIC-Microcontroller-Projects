#define DS18x20_pin			 pin_B7
                           
#define reset_pulse	    		480   
#define bit_start_pulse	    	  2          
#define presence_delay      	 40   
#define post_presence_delay		480 
#define bit_read_delay		      5                        
#define bit_write_delay			100                        

#define temp_LSB			      0
#define temp_MSB    		      1
#define cnt_remain				  6
#define cnt_per_s				  7
#define scrpd_len			      9     

#define addr_len		          8

#define search_rom_cmd	       0xF0
#define read_rom_cmd           0x33
#define match_rom_cmd          0x55
#define skip_rom_cmd           0xCC
#define alarm_search_cmd       0xEC
#define convert_temp_cmd       0x44                           
#define write_mem_cmd	       0x4E
#define read_mem_cmd	       0xBE
#define copy_mem_cmd           0x48
#define recall_ee_cmd          0xB8

#define temp_res				256

#define DS18B20_family_code    0x28           
#define DS18S20_family_code    0x10   


short DS1820_reset();
short DS1820_read_bit();
void DS1820_write_bit(short bit_value);
unsigned char DS1820_read_byte();
void DS1820_write_byte(unsigned char value);    
void DS1820_device_addr(unsigned char addr_method);   
short DS1820_find_next_device();
short DS1820_find_first_device();
void DS1820_write_EEPROM(unsigned char THigh, unsigned char TLow);
signed long DS1820_get_raw_temp();
float DS1820_get_temp();


