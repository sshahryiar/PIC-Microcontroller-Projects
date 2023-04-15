#include "MPR121.h"  


void MPR121_init()
{
	MPR121_write(GPIO_CTRL0, 0x00);  
	MPR121_write(GPIO_CTRL1, 0x00);  
	MPR121_write(GPIO_DIR, 0x00);
	MPR121_write(GPIO_EN, 0x00);     
	MPR121_write(MHD_R, 0x01);
	MPR121_write(NHD_R, 0x01);                          
	MPR121_write(NCL_R, 0x00);                 
	MPR121_write(FDL_R, 0x00);      
	MPR121_write(MHD_F, 0x01);
	MPR121_write(NHD_F, 0x01);             
	MPR121_write(NCL_F, 0x7F);
	MPR121_write(FDL_F, 0x09); 
	MPR121_write(ELE0_T, TOU_THRESH);
	MPR121_write(ELE0_R, REL_THRESH);
	MPR121_write(ELE1_T, TOU_THRESH);
	MPR121_write(ELE1_R, REL_THRESH);
	MPR121_write(ELE2_T, TOU_THRESH);
	MPR121_write(ELE2_R, REL_THRESH);
	MPR121_write(ELE3_T, TOU_THRESH);        
	MPR121_write(ELE3_R, REL_THRESH);  
	MPR121_write(AUTO_CFG0, 0x1B);         
	MPR121_write(AUTO_CFGU, 0xC9);   	
	MPR121_write(AUTO_CFGL, 0x82); 
	MPR121_write(AUTO_CFGT,0xB5);
	MPR121_write(ELE_CFG, 0x84);
} 

                 
unsigned char MPR121_read(unsigned char addr)                  
{                                        
   unsigned char value = 0;               
   
   I2C_Start();              
   I2C_Write(Write_MPR121);
   I2C_Write(addr);                             
   I2C_Start(); 
   I2C_Write(Read_MPR121);
   value = I2C_Read(0);                       
   I2C_Stop(); 
   
   return value;  
}           
           
        
void MPR121_write(unsigned char addr, unsigned char value)
{
   I2C_Start(); 
   I2C_Write(Write_MPR121);
   I2C_Write(addr);
   I2C_Write(value);
   I2C_Stop();
}                                          
