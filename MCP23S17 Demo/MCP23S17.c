#include "MCP23S17.h"  


void setup_MCP23S17()
{                                                 
	MCP23S17_write(IOCON, 0x18); 
	MCP23S17_write(IODIRA, 0x00);  
	MCP23S17_write(IODIRB, 0xFF);  
	MCP23S17_write(GPPUA, 0x00);  
	MCP23S17_write(GPPUB, 0xFF); 
	MCP23S17_write(IPOLA, 0xFF);   
	MCP23S17_write(IPOLB, 0x00); 
}                                                                    
                                                        
                                                  
void MCP23S17_write(unsigned char address, unsigned char value) 
{                                                 
    output_low(CS); 
    spi_xfer(IO_DEVICE_ADDRESS_WRITE);
    spi_xfer(address);                           
    spi_xfer(value); 
    output_high(CS);                  
}                                                 
                                                  
                                                  
unsigned char MCP23S17_read(unsigned char address)                  
{                                                 
    unsigned char received_data = 0;
    output_low(CS); 
    spi_xfer(IO_DEVICE_ADDRESS_READ);            
    spi_xfer(address);                           
    received_data = spi_xfer(0);  
    output_high(CS);          
    return received_data;                                 
}   
