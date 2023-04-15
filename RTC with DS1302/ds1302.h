#define RST				pin_B7
#define SCK	    		pin_B6  
#define IO				pin_B5 
                       
#define sec_wreg		0x80
#define sec_rreg		0x81 
#define min_wreg		0x82
#define min_rreg		0x83
#define hr_wreg			0x84
#define hr_rreg			0x85
#define date_wreg		0x86 
#define date_rreg		0x87                    
#define month_wreg		0x88 
#define month_rreg		0x89
#define dow_wreg		0x8A 
#define dow_rreg		0x8B
#define year_wreg		0x8C
#define year_rreg		0x8D
#define control_wreg	0x8E
#define control_rreg	0x8F
#define charge_wreg		0x90  
#define charge_rreg		0x91   
                         
#define ramlocstartr	0xC1  
#define ramlocendr		0xFD    

#define ramlocstartw	0xC0  
#define ramlocendw		0xFC     
                                 
           

void DS1302_init(); 
void serial_write(unsigned char value);
unsigned char serial_read();
void DS1302_write(unsigned char addr, unsigned char value);                     
unsigned char DS1302_read(unsigned char addr);   
unsigned char read_ram(unsigned char addr);   
void write_ram(unsigned char addr, unsigned char value);
void get_time();                    
void get_date();
void set_time(unsigned char p1, unsigned char p2, unsigned char p3);
void set_date(unsigned char p1, unsigned char p2, unsigned char p3, unsigned char p4);
unsigned char bcd_to_decimal(unsigned char d); 
unsigned char decimal_to_bcd(unsigned char d);   
