#use SPI (MASTER, DO = PIN_B1, DI = PIN_B3, CLK = PIN_B0, BITS = 8, Mode = 0)


#define IO_DEVICE_ADDRESS_WRITE   0x40
#define IO_DEVICE_ADDRESS_READ    0x41

#define IODIRA        0x00      //Data Direction Register for PORTA    
#define IODIRB        0x01      //Data Direction Register for PORTB
#define IPOLA         0x02      //Input Polarity Register for PORTA 
#define IPOLB         0x03      //Input Polarity Register for PORTB 
#define GPINTENA      0x04      //Interrupt-on-change enable Register for PORTA 
#define GPINTENB      0x05      //Interrupt-on-change enable Register for PORTB 
#define DEFVALA       0x06      //Default Value Register for PORTA   
#define DEFVALB       0x07      //Default Value Register for PORTB     
#define INTCONA       0x08      //Interrupt-on-change control Register for PORTA  
#define INTCONB       0x09      //Interrupt-on-change control Register for PORTB     
#define IOCON         0x0A      //Configuration register for device                      
#define GPPUA         0x0C      //100kOhm pullup resistor register for PORTA (sets pin to input when set)   
#define GPPUB         0x0D      //100kOhm pullup resistor register for PORTB (sets pin to input when set)      
#define INTFA         0x0E      //Interrupt flag Register for PORTA       
#define INTFB         0x0F      //Interrupt flag Register for PORTB   
#define INTCAPA       0x10      //Interrupt captured value Register for PORTA  
#define INTCAPB       0x11      //Interrupt captured value Register for PORTB   
#define GPIOA         0x12      //General purpose I/O Register for PORTA  
#define GPIOB         0x13      //General purpose I/O Register for PORTB 
#define OLATA         0x14      //Output latch Register for PORTA
#define OLATB         0x15      //Output latch Register for PORTB  

#define CS			  pin_B2

                                                                          
void setup_MCP23S17();                            
void MCP23S17_write(unsigned char address, unsigned char value);          
unsigned char MCP23S17_read(unsigned char address); 
