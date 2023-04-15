#use i2c(master, SDA = pin_B1, SCL = pin_B0)    

                     
#define PCF8563_read_address							                             0xA3
#define PCF8563_write_address														 0xA2
                 
#define PCF8563_TIMER   															 0x0F    
#define PCF8563_CONTROL1															 0x00     
#define PCF8563_CONTROL2															 0x01                                                                
#define PCF8563_SECONDS 															 0x02    
#define PCF8563_MINUTES 															 0x03     
#define PCF8563_HOURS   														     0x04    
#define PCF8563_DATE																 0x05     
#define PCF8563_WEEKDAYS															 0x06
#define PCF8563_MONTHS  															 0x07                   
#define PCF8563_YEARS   															 0x08 
#define PCF8563_MINUTE_ALARM														 0x09  
#define PCF8563_HOUR_ALARM  														 0x0A  
#define PCF8563_DAY_ALARM   														 0x0B  
#define PCF8563_WEEKDAY_ALARM														 0x0C   
#define PCF8563_CLKOUT  															 0x0D     
#define PCF8563_TCONTROL															 0x0E 

#define SUN																			 0x00
#define MON																			 0x01
#define TUE																			 0x02
#define WED																			 0x03
#define THR																			 0x04
#define FRI																			 0x05
#define SAT																			 0x06 

#define alarm_on																	 0x00
#define alarm_off																	 0x01
                                                                                           

unsigned char sec = 0;
unsigned char min = 0;
unsigned char hr = 0;
unsigned char dt = 0;
unsigned char wk = 0;
unsigned char mt = 0;
unsigned char yr = 0;         
unsigned char al_hr = 0;     
unsigned char al_min = 0; 
unsigned char al_state = 0; 



unsigned char bcd_to_decimal(unsigned char value);
unsigned char decimal_to_bcd(unsigned char value); 
void PCF8563_init(); 
void PCF8563_start_RTC();
void PCF8563_stop_RTC();
void PCF8563_write(unsigned char address, unsigned char value);
unsigned char PCF8563_read(unsigned char address);  
void read_time();                   
void read_date(); 
void read_al_time();                 
void write_time();  
void write_date();    
void write_al_time(); 
short check_alarm(); 
void clear_alarm();          
                                                      
